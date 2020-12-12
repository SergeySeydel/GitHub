
#ifndef __AST_VIEW_H__
#define __AST_VIEW_H__

#include <QWidget>
#include <QList>
#include <QMultiHash>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimerEvent>
#include <QShowEvent>
#include <QResizeEvent>
#include "Sprites.h"
#include "KPowerup.h"

#define MAX_POWER_LEVEL          1000
#define TRUE true
#define FALSE false
class KAsteroidsView : public QWidget
{
    Q_OBJECT
public:
    KAsteroidsView( QWidget *parent = 0);
    virtual ~KAsteroidsView();

    int refreshRate;

    void reset();
    void setEraserSpeed( double rs ) { EraserSpeed = rs; }
    void addEraser( int num );
    void newGame();
    void endGame();
    void newShip();

    void rotateLeft( bool r ) { rotateL = r; rotateSlow = 5; }
    void rotateRight( bool r ) { rotateR = r; rotateSlow = 5; }
    void thrust( bool t ) { shouldReset = false; thrustShip = t && shipPower > 0; }
    void resetSpeed( ) { shouldReset = true; }
    void shoot( bool s ) { shootShip = s; shootDelay = 0; }
    void setShield( bool s );
    void teleport( bool te) { teleportShip = te && mTeleportCount; }
    void brake( bool b );
    void pause( bool p);

    void showText( const QString &text, const QColor &color, bool scroll=TRUE );
    void hideText();

    int shots() const { return shotsFired; }
    int hits() const { return shotsHit; }
    int power() const { return shipPower; }

    int teleportCount() const { return mTeleportCount; }
    int brakeCount() const { return mBrakeCount; }
    int shieldCount() const { return mShieldCount; }
    int shootCount() const { return mShootCount; }

signals:
    void shipKilled();
    void eraserHit( int size );
    void erasersRemoved();
    void updateVitals();


private slots:
    void hideShield();

public slots:
    void showCaraculaAnimation();
protected:
    bool readSprites();
    void wrapSprite( QGraphicsItem * );
    void eraserHit( AnimatedPixmapItem * );
    void reducePower( int val );
    void addExhaust( double x, double y, double dx, double dy, int count );
    void processMissiles();
    void processShip();
    void processPowerups();
    void processShield();
    double randDouble();
    int randInt( int range );

    virtual void resizeEvent( QResizeEvent *event );
    virtual void timerEvent( QTimerEvent * );

    void showEvent( QShowEvent * );

private:
    QGraphicsScene field;
    QGraphicsView view;
    QMap<int, QList<QPixmap> > animation;
    QList<AnimatedPixmapItem*> erasers;
    QList<KMissile*> missiles;
    QList<KBit*> bits;
    QList<KExhaust*> exhaust;
    QList<KPowerup*> powerups;
    KShield *shield;
    AnimatedPixmapItem *ship;
    AnimatedPixmapItem *caracula;
    QGraphicsTextItem *textSprite;

    bool rotateL;
    bool rotateR;
    bool thrustShip;
    bool shouldReset;
    bool shootShip;
    bool teleportShip;
    bool brakeShip;
    bool pauseShip;
    bool shieldOn;

    bool vitalsChanged;

    int  shipAngle;
    int  rotateSlow;
    int  rotateRate;
    int  shipPower;

    int shotsFired;
    int shotsHit;
    int shootDelay;

    int mBrakeCount;
    int mShieldCount;
    int mTeleportCount;
    int mShootCount;

    double shipDx;
    double shipDy;

    int  textDy;
    int  mFrameNum;
    bool mPaused;
    int  mTimerId;

    double EraserSpeed;
    double powerupSpeed;

    bool can_destroy_powerups;

    QTimer *shieldTimer;
    bool initialized;
};

#endif
