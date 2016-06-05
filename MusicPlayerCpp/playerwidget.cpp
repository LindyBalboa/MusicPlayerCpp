#include "Playerwidget.h"

#include <QDebug>
#include <QTime>

PlayerWidget::PlayerWidget(QString playerSide, QSqlDatabase &libraryDb, QWidget *parent) : QWidget(parent)
{
    this->setStyleSheet("QSlider::handle:horizontal{ background: #ffffff;\
                                                            border: 1px solid #777;\
                                                            height: 10px;\
                                                            margin: -6px 0px;\
                                                            width: 3px;\
                                                            }\
                           QSlider::groove:horizontal {background: lightgray ;\
                                                       height: 2px;\
                                                       position: absolute;\
                                                       Left: 4px; Right: 4px;\
                                                       }\
                           QSlider::handle:vertical{ background: #ffffff;\
                                                     border: 1px solid #777;\
                                                     width: 10px;\
                                                     margin: 0 -6px 0;\
                                                     height: 3px;\
                                                    }\
                           QSlider::groove:vertical {background: lightgray ;\
                                                     width: 2px;\
                                                     position: absolute;\
                                                     bottom: 20px;\
                                                     height: 30px;\
                                                    }\
                          ");
    vlc = new VLC(this);

    QVBoxLayout *centralLayout = new QVBoxLayout();
    setLayout(centralLayout);
    playlist = new PlaylistWidget(playerSide, libraryDb, this);
        centralLayout->addWidget(playlist);
        connect(playlist, &PlaylistWidget::clicked, this, &trackSingleClicked);
        connect(playlist, &PlaylistWidget::doubleClicked, this, &trackDoubleClicked);
        PlaylistDelegate *delegate = new PlaylistDelegate(playlist);
        playlist->setItemDelegate(delegate);

    QVBoxLayout *buttonsSliders = new QVBoxLayout();
    QHBoxLayout *sliders = new QHBoxLayout();
    QHBoxLayout *buttons = new QHBoxLayout();
    playButton = new QPushButton("Play");
        playButton->setFocusPolicy(Qt::NoFocus);
        buttons->addWidget(playButton);
        connect(playButton, &QPushButton::released, this, &PlayerWidget::playClicked); //VLC::setPause() is responsible for switch between play and pause
    QPushButton *stopButton = new QPushButton("Stop");
        stopButton->setFocusPolicy(Qt::NoFocus);
        buttons->addWidget(stopButton);
        connect(stopButton, &QPushButton::released, this, &PlayerWidget::stopClicked);
    QPushButton *nextButton = new QPushButton("Next");
        nextButton->setFocusPolicy(Qt::NoFocus);
        buttons->addWidget(nextButton);
        //connect(nextButton, &QPushButton::released, this, &nextTrack);
    QPushButton *previousButton = new QPushButton("Prev");
        previousButton->setFocusPolicy(Qt::NoFocus);
        buttons->addWidget(previousButton);
        //connect(previousButton, &QPushButton::released, this, &previousTrack);
    buttonsSliders->addLayout(buttons);

    currentPositionLabel = new QLabel();
        currentPositionLabel->setText("00:00");
        sliders->addWidget(currentPositionLabel);
    positionSlider = new SliderClass(Qt::Horizontal);
        connect(vlc,&VLC::timeChanged, this, &PlayerWidget::updateSlider);
        connect(vlc, &VLC::lengthChanged, this, &PlayerWidget::lengthChanged);
        connect(vlc, &VLC::endReached, this, &PlayerWidget::loadNextTrack);
        connect(positionSlider, &SliderClass::sliderReleased, this, &PlayerWidget::positionSliderReleased);
        connect(positionSlider, &SliderClass::clicked, this, &PlayerWidget::positionSliderReleased);
        sliders->addWidget(positionSlider);
    totalLengthLabel = new QLabel();
        totalLengthLabel->setText("00:00");
        sliders->addWidget(totalLengthLabel);
    volumeSlider = new SliderClass(Qt::Vertical);
        volumeSlider->setMaximumHeight(50);
        volumeSlider->setMinimumHeight(50);
        volumeSlider->setRange(0,100);
        connect(volumeSlider, &SliderClass::sliderMoved, vlc, &VLC::setVolume);
        volumeSlider->setValue(50);
        sliders->addWidget(volumeSlider);
    buttonsSliders->addLayout(sliders);
    centralLayout->addLayout(buttonsSliders);
}
PlayerWidget::~PlayerWidget()
{
}
void PlayerWidget::keyReleaseEvent(QKeyEvent *event)
{
    event->ignore();
}
void PlayerWidget::lengthChanged(qint32 totalTime)
{
    positionSlider->setRange(0,totalTime);
    totalLengthLabel->setText(msToTimestamp(totalTime));
}
void PlayerWidget::loadNextTrack() {
    //qDebug() << "load";
    positionSlider->setValue(0);
    currentPositionLabel->setText("00:00");
    vlc->setPosition(0);
    int i = 0;
    while (i<=playlist->model()->rowCount() ){
        QModelIndex index = playlist->model()->index(i,0);  //Loop through the pl_id of every track (how can this be better?
        //qDebug() <<  playlist->currentTrackID << playlist->model()->data(index).toInt();  //If pl_id in loop matches current track, load the very next track. Later for shuffle this belongs in a switch.
        if (playlist->currentTrackID == playlist->model()->data(index).toInt() ) { //If pl_id in loop matches current track, load the very next track. Later for shuffle this belongs in a switch.
            index = playlist->model()->index(i+1,0);
            QSqlRecord record = playlist->playlistModel->record(index.row());
            playlist->currentTrackID = record.value("IDNowPlaying").toInt(); //Change to immediate next row
            vlc->changeTrack(record.value("path").toString());
            playlist->viewport()->update();
            break;  //Needed so you don't try to load every track in the playlist and then end up with nothing :P
        }
        i++;
    }
}
void PlayerWidget::playClicked()
{
    vlc->setPause();
    if (this->playButton->text()=="Play"){
        this->playButton->setText("Pause");
    }else{
        this->playButton->setText("Play");
    }

}
void PlayerWidget::stopClicked()
{
    vlc->stop();
    positionSlider->setValue(0);
    currentPositionLabel->setText("00:00");
    this->playButton->setText("Play");
}
void PlayerWidget::positionSliderReleased()
{
    vlc->setPosition(positionSlider->value());
}
void PlayerWidget::updateSlider(qint32 currentTime)
{
    currentPositionLabel->setText(msToTimestamp(currentTime));
    if (!positionSlider->isSliderDown() ){
        positionSlider->setValue(currentTime);
    }
}
void PlayerWidget::trackDoubleClicked(const QModelIndex &index)
{
    QSqlRecord record = playlist->playlistModel->record(index.row());
    vlc->changeTrack(record.value("path").toString());
    playlist->currentTrackID = record.value("IDNowPlaying").toInt();
    playlist->viewport()->repaint();
    this->playButton->setText("Pause");
}
void PlayerWidget::trackSingleClicked(const QModelIndex &index)
{
}

PlaylistDelegate::PlaylistDelegate(PlaylistWidget *parent) : QStyledItemDelegate(parent)
{
    _parent = parent;
}
void PlaylistDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if (index.model()->data(index.model()->index(index.row(),0)).toInt() == _parent->currentTrackID){
        QStyleOptionViewItem opt(option);
        opt.font.setBold(true);
        QStyledItemDelegate::paint(painter,opt,index);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QString PlayerWidget::msToTimestamp(int ms)
{
    int s = 0;
    int m = 0;
    int h = 0;
    s = ms/1000;
    if (s>60){
        m = s/60;
        s = s%60;
        if (m>60){
            h = m/60;
            m = m%60;
            return QString("%1:%2:%3").arg(h,2,10,QChar('0')).arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
        }
    }
    return QString("%1:%2").arg(m,2,10,QChar('0')).arg(s,2,10,QChar('0'));
}
