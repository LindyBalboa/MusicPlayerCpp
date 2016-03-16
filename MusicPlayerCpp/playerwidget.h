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
    PlayerWidget(QString playerSide, QSqlDatabase &libraryDb, QWidget *parent = 0);
    ~PlayerWidget();
    PlaylistWidget *playlist;
    VLC *vlc;

private:
    SliderClass *positionSlider;
    SliderClass *volumeSlider;
    QPushButton *playButton;
    void loadNextTrack();
public slots:
    void trackSingleClicked(const QModelIndex &index);
    void trackDoubleClicked(const QModelIndex &index);
    void positionSliderReleased();
    //void previousTrack();
protected:
    void keyReleaseEvent(QKeyEvent *event);
private slots:
    void updateSlider(qint64 time);
    void lengthChanged(quint64 totalTime);
    void playClicked();
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
