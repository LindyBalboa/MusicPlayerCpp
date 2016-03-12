#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include "sliderclass.h"
#include <playlistwidget.h>
#include <vlc.h>
#include <QItemDelegate>

class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    VLC *vlc;
    PlayerWidget(QString playerSide, QWidget *parent = 0);
    ~PlayerWidget();

private:
    PlaylistWidget *playlist;
    SliderClass *positionSlider;
    void loadNextTrack();
public slots:
    void trackSingleClicked(const QModelIndex &index);
    void trackDoubleClicked(const QModelIndex &index);
    void positionSliderReleased();
    //void previousTrack();
protected:
private slots:
    void updateSlider(qint64 time);
    void lengthChanged(quint64 totalTime);
};

class PlaylistDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    explicit PlaylistDelegate(PlaylistWidget *parent = 0);
    PlaylistWidget *_parent;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PLAYERWIDGET_H
