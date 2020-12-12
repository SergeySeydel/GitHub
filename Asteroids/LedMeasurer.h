
#ifndef __LEDMETER_H__
#define __LEDMETER_H__

#include <QFrame>
#include <QList>
#include <QResizeEvent>

class KALedMeter : public QFrame
{
    Q_OBJECT
public:
    KALedMeter( QWidget *parent );
    ~KALedMeter();

    int range()  { return mRange; }
    void setRange( int r );

    int count()  { return mCount; }
    void setCount( int c );

    int value () const { return mValue; }

    void addColorRange( int pc, const QColor &c );

public slots:
    void setValue( int v );

protected:
    virtual void resizeEvent( QResizeEvent * );
    virtual void paintEvent(QPaintEvent *event);
    void calcColorRanges();

protected:
    struct ColorRange
    {
        int mPc;
        int mValue;
        QColor mColor;
    };

    int mRange;
    int mCount;
    int mCurrentCount;
    int mValue;
    QList<ColorRange*> mCRanges;
};

#endif
