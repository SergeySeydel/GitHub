#ifndef KMISSILE_H
#define KMISSILE_H
#define ID_MISSILE              1030

#define MAX_SHIELD_AGE          350
#define MAX_MISSILE_AGE         100

#include "Defines.h"
#include "AnimItem.h"


class KMissile : public AnimatedPixmapItem
{
public:
    KMissile( const QList<QPixmap> &s, QGraphicsScene *c ) : AnimatedPixmapItem( s, c )
        { myAge = 0; }

    virtual int type() const { return ID_MISSILE; }

    void growOlder() { myAge++; }
    bool expired() { return myAge > MAX_MISSILE_AGE; }

private:
    int myAge;
};
#endif
