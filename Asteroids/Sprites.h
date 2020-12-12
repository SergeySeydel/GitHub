#ifndef __SPRITES_H__
#define __SPRITES_H__

#include "Defines.h"
#include "AnimItem.h"
#include "KMissile.h"
#define ID_ERASER_LARGE           1024
#define ID_ERASER_MEDIUM          1025
#define ID_ERASER_SMALL           1026



#define ID_BIT                  1040
#define ID_EXHAUST              1041

#define ID_ENERGY_POWERUP       1310
#define ID_TELEPORT_POWERUP     1311
#define ID_BRAKE_POWERUP        1312
#define ID_SHIELD_POWERUP       1313
#define ID_SHOOT_POWERUP        1314

#define ID_SHIELD               1351
#define ID_CARACULA             1352





class KBit : public AnimatedPixmapItem
{
public:
    KBit( const QList<QPixmap> &s, QGraphicsScene *c ) : AnimatedPixmapItem( s, c )
        {  death = 7; }

    virtual int type() const {  return ID_BIT; }

    void setDeath( int d ) { death = d; }
    void growOlder() { death--; }
    bool expired() { return death <= 0; }

private:
    int death;
};

class KExhaust : public AnimatedPixmapItem
{
public:
    KExhaust( const QList<QPixmap> &s, QGraphicsScene *c ) : AnimatedPixmapItem( s, c )
        {  death = 1; }

    virtual int type() const {  return ID_EXHAUST; }

    void setDeath( int d ) { death = d; }
    void growOlder() { death--; }
    bool expired() { return death <= 0; }

private:
    int death;
};



class KEraser : public AnimatedPixmapItem
{
public:
    KEraser (const QList<QPixmap> &s, QGraphicsScene *c, int t, int sk, int st) : AnimatedPixmapItem( s, c )
        { _type = t; skip = cskip = sk; step = st; }

    void nextFrame()
        {
            if (cskip-- <= 0) {
                setFrame( (frame()+step+frameCount())%frameCount() );
                cskip = qAbs(skip);
            }
        }

    virtual int type() const { return _type; }

private:
    int _type;
    int skip;
    int cskip;
    int step;
};

class KShield : public AnimatedPixmapItem
{
public:
  KShield( QList<QPixmap> &s, QGraphicsScene *c )
      : AnimatedPixmapItem( s, c ) {}

  virtual int type() const { return ID_SHIELD; }
};

#endif
