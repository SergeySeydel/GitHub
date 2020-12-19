#include <stdlib.h>
#include <math.h>
#include <qapplication.h>
#include <qnamespace.h>
#include <QAction>
#include <QMessageBox>
#include <QScrollArea>
#include <QDir>
#include <QGraphicsItem>
#include <QTimerEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QShowEvent>
#include <QtDebug>

#include "View.h"

#define IMG_BACKGROUND ":/asteroidsgame/bg.png"

#define REFRESH_DELAY           33
#define SHIP_SPEED              0.3
#define MISSILE_SPEED           10
#define SHIP_STEPS              64
#define ROTATE_RATE             2
#define SHIELD_ON_COST          1
#define SHIELD_HIT_COST         30
#define BRAKE_ON_COST           4

#define MAX_ERASER_SPEED          2.5
#define MAX_POWERUP_SPEED       1.5
#define MAX_SHIP_SPEED          12
#define MAX_BRAKES              5
#define MAX_SHIELDS             5
#define MAX_FIREPOWER           5

#define TEXT_SPEED              4

#define PI_X_2                  6.283185307
#ifndef M_PI
#define M_PI 3.141592654
#endif

static struct
{
    int id;
    const char *path;
    int frames;
}
kas_animations [] =
{
    { ID_ERASER_LARGE,       "rock1/rock1%1.png",       32 },
    { ID_ERASER_MEDIUM,      "rock2/rock2%1.png",       32 },
    { ID_ERASER_SMALL,       "rock3/rock3%1.png",       32 },
    { ID_SHIP,             "ship/ship%1.png",         32 },
    { ID_MISSILE,          "missile/missile.png",      1 },
    { ID_BIT,              "bits/bits%1.png",         16 },
    { ID_EXHAUST,          "exhaust/exhaust.png",      1 },
    { ID_ENERGY_POWERUP,   "powerups/energy.png",      1 },
    { ID_TELEPORT_POWERUP, "powerups/teleport%1.png", 12 },
    { ID_BRAKE_POWERUP,    "powerups/brake.png",       1 },
    { ID_SHIELD_POWERUP,   "powerups/shield.png",      1 },
    { ID_SHOOT_POWERUP,    "powerups/shoot.png",       1 },
    { ID_SHIELD,           "shield/shield%1.png",      6 },
    { ID_CARACULA,         "caracula/caracula%1.png",   2},
    { 0,                   0,                          0 }
};

KAsteroidsView::KAsteroidsView( QWidget *parent)
    : QWidget( parent),
      field(0, 0, 1024, 768),
      view(&field, this)
{
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setOptimizationFlags(QGraphicsView::DontClipPainter
                              | QGraphicsView::DontSavePainterState
                              | QGraphicsView::DontAdjustForAntialiasing);
    view.viewport()->setFocusProxy( this );

    QPixmap pm( IMG_BACKGROUND );
    field.setBackgroundBrush( pm );

    textSprite = new QGraphicsTextItem( 0 );
    field.addItem(textSprite);
    QFont font( "helvetica", 18 );
    textSprite->setFont( font );
    textSprite->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    shield = 0;
    shieldOn = FALSE;
    refreshRate = REFRESH_DELAY;

    initialized = readSprites();

    shieldTimer = new QTimer( this );
    connect( shieldTimer, SIGNAL(timeout()), this, SLOT(hideShield()) );
    mTimerId = -1;

    shipPower = MAX_POWER_LEVEL;
    vitalsChanged = TRUE;
    can_destroy_powerups = FALSE;

    mPaused = TRUE;

    if ( !initialized ) {
        textSprite->setHtml( tr("<font color=red>Error: Cannot read sprite images</font>") );
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            (field.height()-textSprite->boundingRect().height()) / 2 );
    }
}



KAsteroidsView::~KAsteroidsView()
{
    qDeleteAll(erasers);     erasers.clear();
    qDeleteAll(missiles);  missiles.clear();
    qDeleteAll(bits);      bits.clear();
    qDeleteAll(powerups);  powerups.clear();
    qDeleteAll(exhaust);   exhaust.clear();
}



void KAsteroidsView::reset()
{
    if ( !initialized )
        return;
    qDeleteAll(erasers);      erasers.clear();
    qDeleteAll(missiles);   missiles.clear();
    qDeleteAll(bits);       bits.clear();
    qDeleteAll(powerups);   powerups.clear();
    qDeleteAll(exhaust);    exhaust.clear();

    shotsFired = 0;
    shotsHit = 0;

    EraserSpeed = 1.0;
    powerupSpeed = 1.0;
    mFrameNum = 0;
    mPaused = FALSE;

    ship->hide();
    shield->hide();
}



void KAsteroidsView::newGame()
{
    if ( !initialized )
        return;
    if ( shieldOn )
    {
      shield->hide();
      shieldOn = FALSE;
    }
    reset();
    if ( mTimerId < 0 )
        mTimerId = startTimer( REFRESH_DELAY );
    emit updateVitals();
}


void KAsteroidsView::endGame()
{
    qDeleteAll(erasers);     erasers.clear();
    qDeleteAll(missiles);  missiles.clear();
    qDeleteAll(bits);      bits.clear();
    qDeleteAll(powerups);  powerups.clear();
    qDeleteAll(exhaust);   exhaust.clear();
}

void KAsteroidsView::pause( bool p )
{
    if ( !initialized )
        return;
    if ( !mPaused && p ) {
        if ( mTimerId >= 0 ) {
            killTimer( mTimerId );
            mTimerId = -1;
        }
    } else if ( mPaused && !p )
        mTimerId = startTimer( REFRESH_DELAY );
    mPaused = p;
}


void KAsteroidsView::newShip()
{
    if ( !initialized )
        return;
    ship->setPos( width()/2, height()/2 );
    ship->setFrame( 0 );
    shield->setPos( width()/2, height()/2 );
    shield->setFrame( 0 );
    ship->setVelocity( 0.0, 0.0 );
    shipDx = 0;
    shipDy = 0;
    shipAngle = 0;
    rotateL = FALSE;
    rotateR = FALSE;
    thrustShip = FALSE;
    shootShip = FALSE;
    brakeShip = FALSE;
    teleportShip = FALSE;
    shieldOn = TRUE;
    shootDelay = 0;
    shipPower = MAX_POWER_LEVEL;
    rotateRate = ROTATE_RATE;
    rotateSlow = 0;

    mBrakeCount = 0;
    mTeleportCount = 0;
    mShootCount = 0;

    ship->show();
    shield->show();
    mShieldCount = 1;
    shieldTimer->start(1000);
}

void KAsteroidsView::setShield( bool s )
{
    if ( !initialized )
        return;
    if ( shieldTimer->isActive() && !s ) {
        shieldTimer->stop();
        hideShield();
    } else {
        shieldOn = s && mShieldCount;
    }
}

void KAsteroidsView::brake( bool b )
{
    if ( !initialized )
        return;
    if ( mBrakeCount )
    {
        if ( brakeShip && !b )
        {
            rotateL = FALSE;
            rotateR = FALSE;
            thrustShip = FALSE;
            rotateRate = ROTATE_RATE;
        }

        brakeShip = b;
    }
}



bool KAsteroidsView::readSprites()
{
    QString sprites_prefix = ":/asteroidsgame/sprites/";

    int i = 0;
    while ( kas_animations[i].id )
    {
        QList<QPixmap> anim;
        QString wildcard = sprites_prefix + kas_animations[i].path;
        wildcard.replace("%1", "*");
        QFileInfo fi(wildcard);
        foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
            anim << QPixmap(fi.path() + "/" + entry);
        animation.insert( kas_animations[i].id, anim );
        i++;
    }

    ship = new AnimatedPixmapItem( animation[ID_SHIP], &field, 45, 45 );
    caracula = new AnimatedPixmapItem ( animation[ID_CARACULA], &field, 45, 33);


    //QRect Geo = this->geometry();
    //auto r = this->size();

    caracula->hide();
    ship->hide();

    shield = new KShield( animation[ID_SHIELD], &field );
    shield->hide();

    return (!ship->image(0).isNull() && !shield->image(0).isNull());
}



void KAsteroidsView::addEraser( int num )
{
    if ( !initialized )
        return;
    for ( int i = 0; i < num; i++ )
    {
        KEraser *eraser = new KEraser( animation[ID_ERASER_LARGE], &field,
                             ID_ERASER_LARGE, randInt(2), randInt(2) ? -1 : 1 );
        double dx = (2.0 - randDouble()*4.0) * EraserSpeed;
        double dy = (2.0 - randDouble()*4.0) * EraserSpeed;
        eraser->setVelocity( dx, dy );
        eraser->setFrame( randInt( eraser->frameCount() ) );
        if ( dx > 0 )
        {
            if ( dy > 0 )
                eraser->setPos( 5, 5 );
            else
                eraser->setPos( 5, field.height() - 25 );
            eraser->setFrame( 0 );
        }
        else
        {
            if ( dy > 0 )
                eraser->setPos( field.width() - 25, 5 );
            else
                eraser->setPos( field.width() - 25, field.height() - 25 );
            eraser->setFrame( 0 );
        }
        eraser->show();
        erasers.append( eraser );
    }
}



void KAsteroidsView::showText( const QString &text, const QColor &color, bool scroll )
{
    if ( !initialized )
        return;
    textSprite->setHtml( QString("<font color=#%1%2%3>%4</font>")
                         .arg(color.red(), 2, 16, QLatin1Char('0'))
                         .arg(color.green(), 2, 16, QLatin1Char('0'))
                         .arg(color.blue(), 2, 16, QLatin1Char('0'))
                         .arg(text) );
    Q_UNUSED(color);


    if ( scroll ) {
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            -textSprite->boundingRect().height() );
        textDy = TEXT_SPEED;
    } else {
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            (field.height()-textSprite->boundingRect().height()) / 2 );
        textDy = 0;
    }
    textSprite->show();
}



void KAsteroidsView::hideText()
{
    textDy = -TEXT_SPEED;
}



void KAsteroidsView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    field.setSceneRect(0, 0, width()-4, height()-4);
    view.resize(width(),height());
}


void KAsteroidsView::timerEvent( QTimerEvent * )
{
    field.advance();


    foreach(AnimatedPixmapItem *eraser, erasers) {
        ((KEraser *)eraser)->nextFrame();
        wrapSprite( eraser );
    }

    wrapSprite( ship );


    processMissiles();


    qDeleteAll(exhaust);
    exhaust.clear();



    processShip();



    if ( textSprite->isVisible() )
    {
        if ( textDy < 0 &&
             textSprite->boundingRect().y() <= -textSprite->boundingRect().height() ) {
            textSprite->hide();
        } else {
            textSprite->moveBy( 0, textDy );
        }

        if ( textSprite->sceneBoundingRect().y() > (field.height()-textSprite->boundingRect().height())/2 )
            textDy = 0;
    }

    if ( vitalsChanged && !(mFrameNum % 10) ) {
        emit updateVitals();
        vitalsChanged = FALSE;
    }

    mFrameNum++;
}

void KAsteroidsView::wrapSprite( QGraphicsItem *s )
{
    int x = int(s->x() + s->boundingRect().width() / 2);
    int y = int(s->y() + s->boundingRect().height() / 2);

    if ( x > field.width() )
        s->setPos( s->x() - field.width(), s->y() );
    else if ( x < 0 )
        s->setPos( field.width() + s->x(), s->y() );

    if ( y > field.height() )
        s->setPos( s->x(), s->y() - field.height() );
    else if ( y < 0 )
        s->setPos( s->x(), field.height() + s->y() );
}



void KAsteroidsView::eraserHit( AnimatedPixmapItem *hit )
{
    KPowerup *nPup = 0;
    int rnd = int(randDouble()*30.0) % 30;
    switch( rnd )
    {
      case 4:
      case 5:
          nPup = new KPowerup( animation[ID_ENERGY_POWERUP], &field,
                               ID_ENERGY_POWERUP );
        break;
      case 10:
        nPup = new KPowerup( animation[ID_TELEPORT_POWERUP], &field,
                          ID_TELEPORT_POWERUP );
        break;
      case 15:
          nPup = new KPowerup( animation[ID_BRAKE_POWERUP], &field,
                               ID_BRAKE_POWERUP );
        break;
      case 20:
          nPup = new KPowerup( animation[ID_SHIP], &field,
                               ID_SHIP );
        break;
      case 24:
      case 25:
          nPup = new KPowerup( animation[ID_SHOOT_POWERUP], &field,
                               ID_SHOOT_POWERUP );
        break;
    }
    if ( nPup )
    {
        double r = 0.5 - randDouble();
        nPup->setPos( hit->x(), hit->y() );
        nPup->setFrame( 0 );
        nPup->setVelocity( hit->xVelocity() + r, hit->yVelocity() + r );
        powerups.append( nPup );
    }

    if ( hit->type() == ID_ERASER_LARGE || hit->type() == ID_ERASER_MEDIUM )
    {

        double addx[4] = { 1.0, 1.0, -1.0, -1.0 };
        double addy[4] = { -1.0, 1.0, -1.0, 1.0 };

        double dx = hit->xVelocity();
        double dy = hit->yVelocity();

        double maxEraserSpeed = MAX_ERASER_SPEED * EraserSpeed;
        if ( dx > maxEraserSpeed )
            dx = maxEraserSpeed;
        else if ( dx < -maxEraserSpeed )
            dx = -maxEraserSpeed;
        if ( dy > maxEraserSpeed )
            dy = maxEraserSpeed;
        else if ( dy < -maxEraserSpeed )
            dy = -maxEraserSpeed;

        AnimatedPixmapItem *neraser;

        for ( int i = 0; i < 4; i++ )
        {
            double r = EraserSpeed/2 - randDouble()*EraserSpeed;
            if ( hit->type() == ID_ERASER_LARGE )
            {
                neraser = new KEraser( animation[ID_ERASER_MEDIUM], &field,
                                   ID_ERASER_MEDIUM, randInt(2), randInt(2) ? -1 : 1 );
                emit eraserHit( 0 );
            }
            else
            {
                neraser= new KEraser( animation[ID_ERASER_SMALL], &field,
                                   ID_ERASER_SMALL, randInt(2), randInt(2) ? -1 : 1 );
                emit eraserHit( 1 );
            }

            neraser->setPos( hit->x(), hit->y() );
            neraser->setFrame( 0 );
            neraser->setVelocity( dx+addx[i]*EraserSpeed+r, dy+addy[i]*EraserSpeed+r );
            neraser->setFrame( randInt( neraser->frameCount() ) );
            erasers.append( neraser );
        }
    }
    else if ( hit->type() == ID_ERASER_SMALL )
        emit eraserHit( 2 );

    for(QList<AnimatedPixmapItem*>::iterator it = erasers.begin(); it != erasers.end(); it++)
    {
        if((*it) == hit) {
            delete *it;
            it = erasers.erase(it);
            break;
        }
    }

    if ( erasers.count() == 0 )
        emit erasersRemoved();
}

void KAsteroidsView::reducePower( int val )
{
    shipPower -= val;
    if ( shipPower <= 0 )
    {
        shipPower = 0;
        thrustShip = FALSE;
        if ( shieldOn )
        {
            shieldOn = FALSE;
            shield->hide();
        }
    }
    vitalsChanged = TRUE;
}

void KAsteroidsView::addExhaust( double x, double y, double dx,
                                 double dy, int count )
{
    for ( int i = 0; i < count; i++ )
    {
        KExhaust *e = new KExhaust( animation[ID_EXHAUST], &field );
        e->setPos( x + 2 - randDouble()*4, y + 2 - randDouble()*4 );
        e->setVelocity( dx, dy );
        exhaust.append( e );
    }
}

void KAsteroidsView::processMissiles()
{
    QList<KMissile*>::iterator itMissile = missiles.begin();
    while(itMissile != missiles.end())
    {
        (*itMissile)->growOlder();

        if ( (*itMissile)->expired() )
        {
            delete (*itMissile);
            itMissile = missiles.erase(itMissile);
            continue;
        }

        wrapSprite(*itMissile);

        bool missileErased = false;
        QList<QGraphicsItem*> hits = (*itMissile)->collidingItems(Qt::IntersectsItemBoundingRect);
        QList<QGraphicsItem*>::iterator itHit = hits.begin();

        while (itHit != hits.end())
        {
            if ( (*itHit)->type() >= ID_ERASER_LARGE &&
                 (*itHit)->type() <= ID_ERASER_SMALL && (*itHit)->collidesWithItem(*itMissile) )
            {
                shotsHit++;
                eraserHit( static_cast<AnimatedPixmapItem *>(*itHit) );
                delete *itMissile;
                itMissile = missiles.erase(itMissile);
                missileErased = true;
                break;
            }
            itHit++;
        }

        if(!missileErased)
            itMissile++;
    }
}


void KAsteroidsView::processShip()
{
    if ( ship->isVisible() )
    {
        if ( shieldOn )
        {
            shield->show();
            reducePower( SHIELD_ON_COST );
            static int sf = 0;
            sf++;

            if ( sf % 2 )
                shield->setFrame( (shield->frame()+1) % shield->frameCount() );
            shield->setPos( ship->x() - 9, ship->y() - 9 );

            QList<QGraphicsItem *> hits = shield->collidingItems(Qt::IntersectsItemBoundingRect);
            QList<QGraphicsItem *>::Iterator it;
            for ( it = hits.begin(); it != hits.end(); ++it )
            {
                if ( (*it)->type() >= ID_ERASER_LARGE &&
                     (*it)->type() <= ID_ERASER_SMALL && (*it)->collidesWithItem(shield) )
                {
                    int factor;
                    switch ( (*it)->type() )
                    {
                        case ID_ERASER_LARGE:
                            factor = 3;
                            break;

                        case ID_ERASER_MEDIUM:
                            factor = 2;
                            break;

                        default:
                            factor = 1;
                    }

                    if ( factor > mShieldCount )
                    {

                        shieldOn = FALSE;
                        break;
                    }
                    eraserHit( static_cast<AnimatedPixmapItem *>(*it) );

                    reducePower( factor * (SHIELD_HIT_COST - mShieldCount*2) );
                }
            }
        }

        if ( !shieldOn )
        {
            shield->hide();
            QList<QGraphicsItem *> hits = ship->collidingItems(Qt::IntersectsItemBoundingRect);
            QList<QGraphicsItem *>::Iterator it;
            for ( it = hits.begin(); it != hits.end(); ++it )
            {
                if ( (*it)->type() >= ID_ERASER_LARGE &&
                     (*it)->type() <= ID_ERASER_SMALL && (*it)->collidesWithItem(ship))
                {
                 KBit *bit;
                    for ( int i = 0; i < 12; i++ )
                    {
                     bit = new KBit( animation[ID_BIT], &field );
                     bit->setPos( ship->x() + 5 - randDouble() * 10,
                                   ship->y() + 5 - randDouble() * 10 );
                     bit->setFrame( randInt(bit->frameCount()) );
                     bit->setVelocity( 1-randDouble()*2,
                                        1-randDouble()*2 );
                     bit->setDeath( 60 + randInt(60) );
                     bits.push_back( bit );
                    }
                    ship->hide();
                    shield->hide();
                    emit shipKilled();
                    break;
                }
            }
        }

        if ( rotateSlow )
            rotateSlow--;

        if ( rotateL )
        {
            shipAngle -= rotateSlow ? 1 : rotateRate;
            if ( shipAngle < 0 )
                shipAngle += SHIP_STEPS;
        }

        if ( rotateR )
        {
            shipAngle += rotateSlow ? 1 : rotateRate;
            if ( shipAngle >= SHIP_STEPS )
                shipAngle -= SHIP_STEPS;
        }

        double angle = shipAngle * PI_X_2 / SHIP_STEPS;
        double cosangle = cos( angle );
        double sinangle = sin( angle );

        if ( brakeShip )
        {
            thrustShip = FALSE;
            rotateL = FALSE;
            rotateR = FALSE;
            rotateRate = ROTATE_RATE;
            if ( fabs(shipDx) < 2.5 && fabs(shipDy) < 2.5 )
            {
                shipDx = 0.0;
                shipDy = 0.0;
                ship->setVelocity( shipDx, shipDy );
                brakeShip = FALSE;
            }
            else
            {
                double motionAngle = atan2( -shipDy, -shipDx );
                if ( angle > M_PI )
                    angle -= PI_X_2;
                double angleDiff = angle - motionAngle;
                if ( angleDiff > M_PI )
                    angleDiff = PI_X_2 - angleDiff;
                else if ( angleDiff < -M_PI )
                    angleDiff = PI_X_2 + angleDiff;
                double fdiff = fabs( angleDiff );
                if ( fdiff > 0.08 )
                {
                    if ( angleDiff > 0 )
                        rotateL = TRUE;
                    else if ( angleDiff < 0 )
                        rotateR = TRUE;
                    if ( fdiff > 0.6 )
                        rotateRate = mBrakeCount + 1;
                    else if ( fdiff > 0.4 )
                        rotateRate = 2;
                    else
                        rotateRate = 1;

                    if ( rotateRate > 5 )
                        rotateRate = 5;
                }
                else if ( fabs(shipDx) > 1 || fabs(shipDy) > 1 )
                {
                    thrustShip = TRUE;

                    shipDx += cosangle/6 * (mBrakeCount - 1);
                    shipDy += sinangle/6 * (mBrakeCount - 1);
                    reducePower( BRAKE_ON_COST );
                    addExhaust( ship->x() + 20 - cosangle*22,
                                ship->y() + 20 - sinangle*22,
                                shipDx-cosangle, shipDy-sinangle,
                                mBrakeCount+1 );
                }
            }
        }

        if (shouldReset)
        {
            ship->setVelocity( 0, 0 );
        }
        if ( thrustShip )
        {

            double thrustx = cosangle/4;
            double thrusty = sinangle/4;
            if ( fabs(shipDx + thrustx) < MAX_SHIP_SPEED )
                shipDx += thrustx;
            if ( fabs(shipDy + thrusty) < MAX_SHIP_SPEED )
                shipDy += thrusty;
            ship->setVelocity( shipDx, shipDy );
            reducePower( 1 );
            addExhaust( ship->x() + 20 - cosangle*20,
                        ship->y() + 20 - sinangle*20,
                        shipDx-cosangle, shipDy-sinangle, 3 );
        }

        ship->setFrame( shipAngle >> 1 );

        if ( shootShip )
        {
            if ( !shootDelay && (int)missiles.size() < mShootCount + 7 )
            {
              KMissile *missile = new KMissile( animation[ID_MISSILE], &field );
              missile->setPos( 21+ship->x()+cosangle*21,
                             21+ship->y()+sinangle*21 );
              missile->setFrame( 0 );
              missile->setVelocity( shipDx + cosangle*MISSILE_SPEED,
                                    shipDy + sinangle*MISSILE_SPEED );
              missiles.push_back( missile );
              shotsFired++;
              reducePower( 1 );

              shootDelay = 5;
            }

            if ( shootDelay )
              shootDelay--;
        }

        if ( teleportShip )
        {
            int ra = qrand() % 10;
            if( ra == 0 )
            ra += qrand() % 20;
            int xra = ra * 60 + ( (qrand() % 20) * (qrand() % 20) );
            int yra = ra * 50 - ( (qrand() % 20) * (qrand() % 20) );
            ship->setPos( xra, yra );
        }

        vitalsChanged = TRUE;
    }
}

void KAsteroidsView::processPowerups()
{

    QList<KPowerup*>::iterator itPup = powerups.begin();

    while(itPup != powerups.end())
    {
        (*itPup)->growOlder();

        if((*itPup)->expired())
        {
            delete *itPup;
            itPup = powerups.erase(itPup);
            continue;
        }

        wrapSprite(*itPup);

        bool pupErased = false;

        QList<QGraphicsItem *> hits = (*itPup)->collidingItems();
        for(QList<QGraphicsItem *>::Iterator itHits = hits.begin(); itHits != hits.end(); itHits++)
        {
            if ( (*itHits) == ship )
            {
                switch( (*itPup)->type() )
                {
                  case ID_ENERGY_POWERUP:
                    shipPower += 150;
                    if ( shipPower > MAX_POWER_LEVEL )
                        shipPower = MAX_POWER_LEVEL;
                    break;
                  case ID_TELEPORT_POWERUP:
                    mTeleportCount++;
                    break;
                  case ID_BRAKE_POWERUP:
                    if ( mBrakeCount < MAX_BRAKES )
                        mBrakeCount++;
                    break;
                  case ID_SHIELD_POWERUP:
                    if ( mShieldCount < MAX_SHIELDS )
                        mShieldCount++;
                    break;
                  case ID_SHOOT_POWERUP:
                    if ( mShootCount < MAX_FIREPOWER )
                        mShootCount++;
                    break;
                }

                delete *itPup;
                itPup = powerups.erase(itPup);
                pupErased = true;
                vitalsChanged = TRUE;
                break;
            }
            else if((*itHits) == shield )
            {
                delete *itPup;
                itPup = powerups.erase(itPup);
                pupErased = true;
                break;
            }
            else if ( (*itHits)->type() == ID_MISSILE )
            {
                if ( can_destroy_powerups )
                {
                    delete *itPup;
                    itPup = powerups.erase(itPup);
                    pupErased = true;
                    break;
                }
            }
        }

        if(!pupErased)
            itPup++;
    }
}


void KAsteroidsView::hideShield()
{
    shield->hide();
    mShieldCount = 0;
    shieldOn = FALSE;
}

void KAsteroidsView::showCaraculaAnimation()
{
    auto geometry = this->geometry();
    caracula->setPos(10, geometry.y() - 50);
    caracula->show();
}
double KAsteroidsView::randDouble()
{
    int v = qrand();
    return (double)v / (double)RAND_MAX;
}

int KAsteroidsView::randInt( int range )
{
    return qrand() % range;
}

void KAsteroidsView::showEvent( QShowEvent *e )
{
#if defined( QT_LICENSE_PROFESSIONAL )
    static bool wasThere = FALSE;

    if ( !wasThere ) {
        wasThere = TRUE;
        QMessageBox::information( this, tr("QGraphicsView demo"),
                                        tr("This game has been implemented using the QGraphicsView class.\n"
                                           "The QGraphicsView class is not part of the Light Platform Edition. Please \n"
                                           "contact Nokia if you want to upgrade to the Full Platform Edition.") );
    }
#endif

    QWidget::showEvent( e );
}
