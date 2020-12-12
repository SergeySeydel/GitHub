#ifndef ANIMATEDPIXMAPITEM_H
#define ANIMATEDPIXMAPITEM_H

#include <QGraphicsItem>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QGraphicsScene>

class AnimatedPixmapItem : public QGraphicsItem
{
public:
    AnimatedPixmapItem(const QList<QPixmap> &animation, QGraphicsScene *scene = 0, int scaleToWidth = 0, int scaleToHeight = 0);

    void setFrame(int frame);
    inline int frame() const                        { return currentFrame; }
    inline int frameCount() const                   { return frames.size(); }
    inline QPixmap image(int frame) const           { return frames.isEmpty() ? QPixmap() : frames.at(frame % frames.size()).pixmap; }
    inline void setVelocity(qreal xvel, qreal yvel) { vx = xvel; vy = yvel; }
    inline qreal xVelocity() const                  { return vx; }
    inline qreal yVelocity() const                  { return vy; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void advance(int phase);

private:
    struct Frame {
        QPixmap pixmap;
        QPainterPath shape;
        QRectF boundingRect;
    };

    int currentFrame;
    QList<Frame> frames;
    qreal vx, vy;
};

#endif
