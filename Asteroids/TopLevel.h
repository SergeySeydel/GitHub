#ifndef __KAST_TOPLEVEL_H__
#define __KAST_TOPLEVEL_H__

#include <QMainWindow>
#include <QMultiHash>
#include <QMap>
#include <QShowEvent>
#include <QHideEvent>
#include <QKeyEvent>
#include <QPushButton>
#include "View.h"

class KALedMeter;
class QLCDNumber;

class KAstTopLevel : public QMainWindow
{
    Q_OBJECT
public:
    KAstTopLevel( QWidget *parent = 0);
    virtual ~KAstTopLevel();

private:
    void playSound( const char *snd );
    void readSoundMapping();
    void doStats();

protected:
    virtual void showEvent( QShowEvent * );
    virtual void hideEvent( QHideEvent * );
    virtual void keyPressEvent( QKeyEvent *event );
    virtual void keyReleaseEvent( QKeyEvent *event );

private slots:
    void slotNewGame();

    void slotShipKilled();
    void slotEraserHit( int size );
    void slotErasersRemoved();

    void slotUpdateVitals();

private:
    KAsteroidsView *view;
    QLCDNumber *scoreLCD;
    QLCDNumber *levelLCD;
    QLCDNumber *shipsLCD;

    QLCDNumber *teleportsLCD;

    QLCDNumber *brakesLCD;
    QLCDNumber *shieldLCD;
    QLCDNumber *shootLCD;
    KALedMeter *powerMeter;
    QPushButton *animationButton;
    bool   sound;



    bool waitShip;
    bool isPaused;

    int shipsRemain;
    int score;
    int level;
    bool showHiscores;

    enum Action { Launch, Thrust, RotateLeft, RotateRight, Shoot, Teleport,
                    Brake, Shield, Pause, NewGame, ResetSpeed  };

    QMap<int, Action> actions;
};

#endif
