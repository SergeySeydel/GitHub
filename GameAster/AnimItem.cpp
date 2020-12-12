
#include "AnimItem.h"

AnimatedPixmapItem::AnimatedPixmapItem(const QList<QPixmap> &animation,
                                       QGraphicsScene *scene, int scaleToWidth, int scaleToHeight)
    : QGraphicsItem(0), currentFrame(0), vx(0), vy(0)
{
    for (int i = 0; i < animation.size(); ++i) {
        QPixmap pixmap = (scaleToHeight != 0 && scaleToWidth != 0) ? animation.at(i).scaled(scaleToHeight, scaleToWidth, Qt::KeepAspectRatio):animation.at(i);

        Frame frame;
        frame.pixmap = pixmap;
        frame.shape = QPainterPath();
        frame.boundingRect = pixmap.rect();
        frames << frame;
    }

    scene->addItem(this);
}

void AnimatedPixmapItem::setFrame(int frame)
{
    if (!frames.isEmpty()) {
        prepareGeometryChange();
        currentFrame = frame % frames.size();
    }


}

void AnimatedPixmapItem::advance(int phase)
{
    if (phase == 1)
        moveBy(vx, vy);
}

QRectF AnimatedPixmapItem::boundingRect() const
{
    return frames.at(currentFrame).boundingRect;
}

QPainterPath AnimatedPixmapItem::shape() const
{
    const Frame &f = frames.at(currentFrame);
    if (f.shape.isEmpty()) {
        QPainterPath path;
        path.addRegion(f.pixmap.createHeuristicMask());
        const_cast<Frame &>(f).shape = path;
    }
    return f.shape;
}

void AnimatedPixmapItem::paint(QPainter *painter, const QStyleOptionGraphicsItem * /*option*/,
                               QWidget * /*widget*/)
{
    painter->drawPixmap(0, 0, frames.at(currentFrame).pixmap);
}
