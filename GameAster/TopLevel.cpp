
#include <QAction>
#include <QLabel>
#include <QLayout>
#include <QLCDNumber>
#include <QPushButton>

#include <QApplication>
#include <QHBoxLayout>
#include <QShowEvent>
#include <QFrame>
#include <QPixmap>
#include <QHideEvent>
#include <QKeyEvent>
#include <QVBoxLayout>

#include "TopLevel.h"
#include "LedMeasurer.h"

#define SB_SCORE        1
#define SB_LEVEL        2
#define SB_SHIPS        3

struct SLevel
{
    int    nerasers;
    double eraserSpeed;
};

#define MAX_LEVELS      16

SLevel levels[MAX_LEVELS] =
{
    { 1, 0.4 },
    { 1, 0.6 },
    { 2, 0.5 },
    { 2, 0.7 },
    { 2, 0.8 },
    { 3, 0.6 },
    { 3, 0.7 },
    { 3, 0.8 },
    { 4, 0.6 },
    { 4, 0.7 },
    { 4, 0.8 },
    { 5, 0.7 },
    { 5, 0.8 },
    { 5, 0.9 },
    { 5, 1.0 }
};

const char *soundEvents[] =
{
    "ShipDestroyed",
    "eraserDestroyed",
    0
};

const char *soundDefaults[] =
{
    "Explosion.wav",
    "ploop.wav",
    0
};

KAstTopLevel::KAstTopLevel( QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *border = new QWidget( this );

    QPalette palette;
    palette.setColor(border->backgroundRole(), Qt::black);
    border->setPalette(palette);

    setCentralWidget( border );

    QVBoxLayout *borderLayout = new QVBoxLayout( border );
    borderLayout->addStretch( 1 );

    QWidget *mainWin = new QWidget( border );
#if defined(Q_WS_MAEMO_5)
    mainWin->setFixedSize(800, 430);
#elif defined(Q_OS_SYMBIAN)
    mainWin->setFixedSize(640, 340);
#else
    mainWin->setFixedSize(1024, 768);
#endif
    borderLayout->addWidget( mainWin, 0, Qt::AlignHCenter );

    borderLayout->addStretch( 1 );

    view = new KAsteroidsView( mainWin );
    view->setFocusPolicy( Qt::StrongFocus );
    connect( view, SIGNAL(shipKilled()), SLOT(slotShipKilled()) );
    connect( view, SIGNAL(eraserHit(int)), SLOT(slotEraserHit(int)) );
    connect( view, SIGNAL(erasersRemoved()), SLOT(slotErasersRemoved()) );
    connect( view, SIGNAL(updateVitals()), SLOT(slotUpdateVitals()) );

    QVBoxLayout *vb = new QVBoxLayout( mainWin );
    QHBoxLayout *hb = new QHBoxLayout;
    QHBoxLayout *hbd = new QHBoxLayout;
    vb->addLayout( hb );

#if defined(Q_OS_SYMBIAN)
    QFont labelFont( "helvetica", 8 );
#else
    QFont labelFont( "helvetica", 24 );
#endif

    QPalette pal(Qt::darkGreen, Qt::black, QColor( 128, 128, 128 ), QColor( 64, 64, 64 ), Qt::black, Qt::darkGreen, Qt::black);

    mainWin->setPalette( pal );

    hb->addSpacing( 10 );

    QLabel *label;
    label = new QLabel( tr("Points"), mainWin );
    label->setFont( labelFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hb->addWidget( label );

    scoreLCD = new QLCDNumber( 6, mainWin );
    scoreLCD->setFrameStyle( QFrame::NoFrame );
    scoreLCD->setSegmentStyle( QLCDNumber::Flat );
    scoreLCD->setFixedWidth( 150 );
    scoreLCD->setPalette( pal );
    hb->addWidget( scoreLCD );
    hb->addStretch( 10 );

    label = new QLabel( tr("Level"), mainWin );
    label->setFont( labelFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hb->addWidget( label );

    levelLCD = new QLCDNumber( 2, mainWin );
    levelLCD->setFrameStyle( QFrame::NoFrame );
    levelLCD->setSegmentStyle( QLCDNumber::Flat );
    levelLCD->setFixedWidth( 70 );
    levelLCD->setPalette( pal );
    hb->addWidget( levelLCD );
    hb->addStretch( 10 );

    label = new QLabel( tr("Lives"), mainWin );
    label->setFont( labelFont );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hb->addWidget( label );

    shipsLCD = new QLCDNumber( 1, mainWin );
    shipsLCD->setFrameStyle( QFrame::NoFrame );
    shipsLCD->setSegmentStyle( QLCDNumber::Flat );
    shipsLCD->setFixedWidth( 40 );
    shipsLCD->setPalette( pal );
    hb->addWidget( shipsLCD );

    hb->addStrut( 30 );

    vb->addWidget( view, 10 );


    vb->addLayout( hbd );

#if defined(Q_OS_SYMBIAN)
    QFont smallFont( "helvetica", 6 );
#else
    QFont smallFont( "helvetica", 14 );
#endif
    hbd->addSpacing( 10 );

    QString sprites_prefix = ":/asteroidsgame/sprites/";

    label = new QLabel( tr( "T" ), mainWin );
    label->setFont( smallFont );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    teleportsLCD = new QLCDNumber( 1, mainWin );
    teleportsLCD->setFrameStyle( QFrame::NoFrame );
    teleportsLCD->setSegmentStyle( QLCDNumber::Flat );
    teleportsLCD->setPalette( pal );
    teleportsLCD->setFixedHeight( 20 );
    hbd->addWidget( teleportsLCD );

    hbd->addSpacing( 10 );

    QPixmap pm( sprites_prefix + "powerups/brake.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    brakesLCD = new QLCDNumber( 1, mainWin );
    brakesLCD->setFrameStyle( QFrame::NoFrame );
    brakesLCD->setSegmentStyle( QLCDNumber::Flat );
    brakesLCD->setPalette( pal );
    brakesLCD->setFixedHeight( 20 );
    hbd->addWidget( brakesLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shield.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    shieldLCD = new QLCDNumber( 1, mainWin );
    shieldLCD->setFrameStyle( QFrame::NoFrame );
    shieldLCD->setSegmentStyle( QLCDNumber::Flat );
    shieldLCD->setPalette( pal );
    shieldLCD->setFixedHeight( 20 );
    hbd->addWidget( shieldLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shoot.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    shootLCD = new QLCDNumber( 1, mainWin );
    shootLCD->setFrameStyle( QFrame::NoFrame );
    shootLCD->setSegmentStyle( QLCDNumber::Flat );
    shootLCD->setPalette( pal );
    shootLCD->setFixedHeight( 20 );
    hbd->addWidget( shootLCD );

    hbd->addStretch( 1 );

    label = new QLabel( tr( "Fuel" ), mainWin );
    label->setFont( smallFont );
    label->setFixedWidth( label->sizeHint().width() + 10 );
    label->setPalette( pal );
    hbd->addWidget( label );

    powerMeter = new KALedMeter( mainWin );
    powerMeter->setFrameStyle( QFrame::Box | QFrame::Plain );
    powerMeter->setRange( MAX_POWER_LEVEL );
    powerMeter->addColorRange( 10, Qt::darkRed );
    powerMeter->addColorRange( 20, QColor(160, 96, 0) );
    powerMeter->addColorRange( 70, Qt::darkGreen );
    powerMeter->setCount( 40 );
    powerMeter->setPalette( pal );
    powerMeter->setFixedSize( 200, 12 );
    hbd->addWidget( powerMeter );
    animationButton = new QPushButton("Caracula");
    connect(animationButton, &QPushButton::clicked, view, &KAsteroidsView::showCaraculaAnimation);
    hbd->addWidget( animationButton );
    shipsRemain = 3;
    showHiscores = FALSE;

    actions.insert( Qt::Key_Up, Thrust );
    actions.insert( Qt::Key_Left, RotateLeft );
    actions.insert( Qt::Key_Right, RotateRight );
    actions.insert( Qt::Key_Space, Shoot );
    actions.insert( Qt::Key_Z, Teleport );
    actions.insert( Qt::Key_X, Brake );
    actions.insert( Qt::Key_S, Shield );
    actions.insert( Qt::Key_P, Pause );
    actions.insert( Qt::Key_L, Launch );
    actions.insert( Qt::Key_N, NewGame );

#if defined(Q_OS_SYMBIAN)
    actions.insert( 122, Teleport );
    actions.insert( 120, Brake );
    actions.insert( 115, Shield );
    actions.insert( 112, Pause );
    actions.insert( 108, Launch );
    actions.insert( 110, NewGame );
#endif

    view->showText( tr( "Press N to start playing" ), Qt::green );
}

KAstTopLevel::~KAstTopLevel()
{
}

void KAstTopLevel::playSound( const char * )
{

}

void KAstTopLevel::keyPressEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }

    Action a = actions[ event->key() ];

    switch ( a )
    {
        case RotateLeft:
            view->rotateLeft( TRUE );
            break;

        case RotateRight:
            view->rotateRight( TRUE );
            break;

        case Thrust:
            view->thrust( TRUE );
            break;
    case ResetSpeed:
        view->resetSpeed( );
        break;
        case Shoot:
            view->shoot( TRUE );
            break;

        case Shield:
            view->setShield( TRUE );
            break;

        case Teleport:
            view->teleport( TRUE );
            break;

        case Brake:
            view->brake( TRUE );
            break;

        default:
            event->ignore();
            return;
    }
    event->accept();
}

void KAstTopLevel::keyReleaseEvent( QKeyEvent *event )
{
    if ( event->isAutoRepeat() || !actions.contains( event->key() ) )
    {
        event->ignore();
        return;
    }

    Action a = actions[ event->key() ];

    switch ( a )
    {
        case RotateLeft:
            view->rotateLeft( FALSE );
            break;

        case RotateRight:
            view->rotateRight( FALSE );
            break;

        case Thrust:
            view->thrust( FALSE );
            //view->resetSpeed();
            break;

        case Shoot:
            view->shoot( FALSE );
            break;

        case Brake:
            view->brake( FALSE );
            break;

        case Shield:
            view->setShield( FALSE );
            break;

        case Teleport:
            view->teleport( FALSE );
            break;

        case Launch:
            if ( waitShip )
            {
                view->newShip();
                waitShip = FALSE;
                view->hideText();
            }
            else
            {
                event->ignore();
                return;
            }
            break;

        case NewGame:
            slotNewGame();
            break;

        default:
            event->ignore();
            return;
    }

    event->accept();
}

void KAstTopLevel::showEvent( QShowEvent *e )
{
    QMainWindow::showEvent( e );
    view->pause( FALSE );
    view->setFocus();
}

void KAstTopLevel::hideEvent( QHideEvent *e )
{
    QMainWindow::hideEvent( e );
    view->pause( TRUE );
}

void KAstTopLevel::slotNewGame()
{
    score = 0;
    shipsRemain = SB_SHIPS;
    scoreLCD->display( 0 );
    level = 0;
    levelLCD->display( level+1 );
    shipsLCD->display( shipsRemain-1 );
    view->newGame();
    view->setEraserSpeed( levels[0].eraserSpeed );
    view->addEraser( levels[0].nerasers );
    view->newShip();
    waitShip = FALSE;
    view->hideText();
    isPaused = FALSE;
}

void KAstTopLevel::slotShipKilled()
{
    shipsRemain--;
    shipsLCD->display( shipsRemain-1 );

    playSound( "ShipDestroyed" );

    if ( shipsRemain )
    {
        waitShip = TRUE;
        view->showText( tr( "Ship Destroyed. Press L to launch."), Qt::green );
    }
    else
    {
        view->showText( tr("Game Over!"), Qt::red );
        view->endGame();
        doStats();
    }
}

void KAstTopLevel::slotEraserHit( int size )
{
    switch ( size )
    {
        case 0:
            score += 10;
             break;

        case 1:
            score += 20;
            break;

        default:
            score += 40;
      }

    playSound( "EraserDestroyed" );

    scoreLCD->display( score );
}

void KAstTopLevel::slotErasersRemoved()
{
    level++;

    if ( level >= MAX_LEVELS )
        level = MAX_LEVELS - 1;

    view->setEraserSpeed( levels[level-1].eraserSpeed );
    view->addEraser( levels[level-1].nerasers );

    levelLCD->display( level+1 );
}

void KAstTopLevel::doStats()
{
    QString r( "0.00" );
    if ( view->shots() )
         r = QString::number( (double)view->hits() / view->shots() * 100.0,
                             'g', 2 );

    view->showText( "Game Over.Press N for a new game.", Qt::green, FALSE );
}

void KAstTopLevel::slotUpdateVitals()
{
    brakesLCD->display( view->brakeCount() );
    shieldLCD->display( view->shieldCount() );
    shootLCD->display( view->shootCount() );

    powerMeter->setValue( view->power() );
}
