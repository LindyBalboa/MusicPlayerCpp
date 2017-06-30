#ifndef SLIDERCLASS_H
#define SLIDERCLASS_H

#include <QMouseEvent>
#include <QSlider>
#include <QWidget>

class SliderClass : public QSlider
{
    Q_OBJECT

    public:
        explicit SliderClass(Qt::Orientation, QWidget *parent = 0);
        ~SliderClass();
    signals:
        void clicked(qint64);
    protected:
        void mousePressEvent(QMouseEvent *event);
};

#endif // SLIDERCLASS_H
