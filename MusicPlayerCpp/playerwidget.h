#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <playlistwidget.h>
#include "sliderclass.h"
#include <vlc.h>

#include <QHBoxLayout>
#include <QStyledItemDelegate>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class PlayerWidget : public QWidget
{
    Q_OBJECT

    public:
        PlayerWidget(QString playerSide, QSqlDatabase &libraryDb, QWidget *parent = 0);
        ~PlayerWidget();

        PlaylistWidget *playlist;
        VLC *vlc;

        void stopClicked();
public slots:
        void positionSliderReleased();
        //void previousTrack();
        void trackDoubleClicked(const QModelIndex &index);
        void trackSingleClicked(const QModelIndex &index);
    protected:
        void keyReleaseEvent(QKeyEvent *event);
    private:
        QPushButton *playButton;
        SliderClass *positionSlider;
        SliderClass *volumeSlider;
        void loadNextTrack();
        QLabel *currentPositionLabel;
        QLabel *totalLengthLabel;
        QString msToTimestamp(int ms);
    private slots:
        void lengthChanged(qint32 totalTime);
        void playClicked();
        void updateSlider(qint32 time);
};

class PlaylistDelegate : public QStyledItemDelegate
{
    Q_OBJECT

    public:
        explicit PlaylistDelegate(PlaylistWidget *parent = 0);
        PlaylistWidget *_parent;
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // PLAYERWIDGET_H
