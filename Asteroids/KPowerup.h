#ifndef KPOWERUP_H
#define KPOWERUP_H

#include "Defines.h"
#include "AnimItem.h"



class KPowerup : public AnimatedPixmapItem
{
public:
  KPowerup( const QList<QPixmap> &s, QGraphicsScene *c, int t ) : AnimatedPixmapItem( s, c, t == ID_SHIP?150:0, t == ID_SHIP?150:0),
        myAge( 0 ), _type(t)
  {

   }

  virtual int type() const { return _type; }

  void growOlder() { myAge++; }
  bool expired() const { return myAge > MAX_POWERUP_AGE; }

protected:
  int myAge;
  int _type;
};

#endif
