#include "Playerwidget.h"
#include <QDebug>

PlayerWidget::PlayerWidget(QString playerSide, QWidget *parent) : QWidget(parent)
{
    vlc = new VLC(this);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    setLayout(centralLayout);
    playlist = new PlaylistWidget(playerSide);
    centralLayout->addWidget(playlist);
        connect(playlist, &PlaylistWidget::clicked, this, &trackSingleClicked);
        connect(playlist, &PlaylistWidget::doubleClicked, this, &trackDoubleClicked);

    PlaylistDelegate *delegate = new PlaylistDelegate(playlist);
    playlist->setItemDelegate(delegate);

    QHBoxLayout *buttons = new QHBoxLayout();
    QPushButton *playButton = new QPushButton("play");
        buttons->addWidget(playButton);
        connect(playButton, &QPushButton::clicked, vlc, &VLC::setPause); //VLC::setPause() is responsible for switch between play and pause
    QPushButton *stopButton = new QPushButton("stop");
        buttons->addWidget(stopButton);
        connect(stopButton, &QPushButton::clicked, vlc, &VLC::stop);
    QPushButton *nextButton = new QPushButton("next");
        buttons->addWidget(nextButton);
        //connect(nextButton, &QPushButton::clicked, this, &nextTrack);
    QPushButton *previousButton = new QPushButton("prev");
        buttons->addWidget(previousButton);
        //connect(previousButton, &QPushButton::clicked, this, &previousTrack);
    centralLayout->addLayout(buttons);

    positionSlider = new SliderClass(Qt::Horizontal);
        connect(vlc,&VLC::timeChanged, this, &PlayerWidget::updateSlider);
        connect(vlc, &VLC::lengthChanged, this, &PlayerWidget::lengthChanged);
        connect(vlc, &VLC::endReached, this, &PlayerWidget::loadNextTrack);
        connect(positionSlider, &SliderClass::sliderReleased, this, &PlayerWidget::positionSliderReleased);
        connect(positionSlider, &SliderClass::clicked, this, &PlayerWidget::positionSliderReleased);
    centralLayout->addWidget(positionSlider);

}

PlayerWidget::~PlayerWidget()
{
}

void PlayerWidget::trackDoubleClicked(const QModelIndex &index)
{
    QSqlRecord record = playlist->playlistModel->record(index.row());
    qDebug() << record.value("pl_id");
    vlc->changeTrack(record.value("path").toString());
    playlist->currentTrackID = record.value("pl_id").toInt();
    playlist->viewport()->repaint();
}

void PlayerWidget::trackSingleClicked(const QModelIndex &index)
{
    qDebug() << index.column();
}

void PlayerWidget::updateSlider(qint64 currentTime)
{
    positionSlider->setValue(currentTime);
}

void PlayerWidget::lengthChanged(quint64 totalTime)
{
    positionSlider->setRange(0,totalTime);
}

void PlayerWidget::positionSliderReleased()
{
    vlc->setPosition(positionSlider->value());
}

void PlayerWidget::loadNextTrack()
{
    int id;
    positionSlider->setValue(0);
    vlc->setPosition(0);
    int i = 0;
    while (i<= playlist->model()->rowCount())
    {
        QModelIndex index = playlist->model()->index(i,0); //Loop through the pl_id of every track (how can this be better?
        if (playlist->currentTrackID == playlist->model()->data(index).toInt() ) //If pl_id in loop matches current track, load the very next track. Later for shuffle this belongs in a switch.
        {
            playlist->currentTrackID = playlist->model()->data(playlist->model()->index(i+1,0)).toInt(); //Change to immediate next row
            vlc->changeTrack(playlist->model()->data(playlist->model()->index(i+1,23)).toString()); //Order VLC to change track. !!!Need to be careful about playback modes!!!
            playlist->viewport()->update();
            break; //Needed so you don't try to load every track in the playlist and then end up with nothing :P
        }
        i++;
    }
}

PlaylistDelegate::PlaylistDelegate(PlaylistWidget *parent) : QItemDelegate(parent)
{
    _parent = parent;
}

void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.model()->data(index.model()->index(index.row(),0)).toInt() == _parent->currentTrackID)
    {
        QStyleOptionViewItem opt(option);
        opt.font.setBold(true);
        QItemDelegate::paint(painter,opt,index);
    }else
    {
        QItemDelegate::paint(painter, option, index);
    }
}


