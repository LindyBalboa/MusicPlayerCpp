#include "sliderclass.h"
#include "globals.h"

SliderClass::SliderClass(Qt::Orientation orientation, QWidget *parent) : QSlider(orientation, parent)
{
    setFocusPolicy(Qt::NoFocus);

}
SliderClass::~SliderClass()
{

}

void SliderClass::mousePressEvent(QMouseEvent * event )
{
    if (event->button() == Qt::LeftButton){
        if (orientation() == Qt::Vertical)
            setValue(minimum() + ((maximum()-minimum()) * (height()-event->y())) / height() ) ;
        else
            setValue(minimum() + ((maximum()-minimum()) * event->x()) / width() ) ;

        event->accept();
    }
    emit clicked(minimum() + ((maximum()-minimum()) * event->x()) / width() );
    QSlider::mousePressEvent(event);
  }
