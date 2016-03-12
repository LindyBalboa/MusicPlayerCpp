#ifndef SLIDERCLASS_H
#define SLIDERCLASS_H

#include <QWidget>
#include <QSlider>
#include <QMouseEvent>

class SliderClass : public QSlider
{
    Q_OBJECT

public:
    explicit SliderClass(Qt::Orientation, QWidget *parent = 0);

signals:
    clicked(qint64);
public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
};

#endif // SLIDERCLASS_H
