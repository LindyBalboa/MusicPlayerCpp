#include "playlistwidget.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QHeaderView>
#include <QDragEnterEvent>

PlaylistWidget::PlaylistWidget(QWidget *parent, QString playerSide) : QTableView(parent)
{
    setStyleSheet("QTableView:item:selected:active{\
                                                                color: black; \
                                                                background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                                                                border-top: 1px solid #A3D1FF;\
                                                                border-bottom: 1px solid #A3D1FF;\
                                                                }\
                  ");

    QSqlDatabase playlistDb = QSqlDatabase::addDatabase("QSQLITE", playerSide);
    playlistDb.setDatabaseName(":memory:");
    qDebug() << playlistDb.open();
    QSqlQuery query(playlistDb);
    query.exec("create table if not exists playlist(pl_id INTEGER PRIMARY KEY AUTOINCREMENT,\
                                                    id INTEGER, \
                                                    album TEXT, \
                                                    Album_artist TEXT,\
                                                    Artist TEXT,\
                                                    BPM INTEGER,\
                                                    Comment TEXT,\
                                                    Competition INTEGER,\
                                                    Composer TEXT,\
                                                    Custom_1 TEXT,\
                                                    Custom_2 TEXT,\
                                                    Custom_3 TEXT,\
                                                    Custom_4 TEXT,\
                                                    Custom_5 TEXT,\
                                                    Disc_numbers TEXT,\
                                                    Date TEXT,\
                                                    Filename TEXT,\
                                                    Genre TEXT,\
                                                    Grouping TEXT,\
                                                    Length REAL,\
                                                    Mood TEXT,\
                                                    Occasion TEXT,\
                                                    Original_date TEXT,\
                                                    Path TEXT,\
                                                    Play_count INTEGER,\
                                                    Quality TEXT,\
                                                    Rating INTEGER,\
                                                    Tempo TEXT,\
                                                    Title TEXT,\
                                                    Track_numbers TEXT,\
                                                    Track_total TEXT)");

    playlistDb.commit();

    //PlaylistModel* playlistModel = new PlaylistModel(this, playlistDb);
    PlaylistModel* playlistModel = new PlaylistModel(this, playlistDb);
    setModel(playlistModel);
    playlistModel->setTable("playlist");
    playlistModel->select();

    QSqlRecord record = playlistDb.record("playlist");
    for (int i=0; i<10; i++)
    {
        record.setValue(1, i);
        playlistModel->insertRecord(i,record);
    }
    qDebug() << record.value(1);
    playlistModel->select();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setDragEnabled(true);
    setShowGrid(false);
    setAcceptDrops(true);
    show();
}

void PlaylistWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void PlaylistWidget::dragMoveEvent(QDragMoveEvent *event)
{

}

void PlaylistWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    QByteArray data = mimeData->data("application/test");
    QDataStream outStream(data);
    QList<QString> trackData;
    while (!outStream.atEnd())
    {
        QString text;
        outStream >> trackData;
    }
    qDebug() << trackData.size();
    for (int i=0; i<trackData.size()-1; i+=2)
    {
        qDebug() << trackData[i] << trackData[i+1];
    }
}

void PlaylistWidget::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mpe";
    QTableView::mousePressEvent(event);
}


PlaylistModel::PlaylistModel(QWidget *parent, QSqlDatabase playlistDb) : QSqlTableModel(parent, playlistDb)
{

}

Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return (Qt::MoveAction | Qt::MoveAction);
}
Qt::DropActions PlaylistModel::supportedDragActions() const
{
    return (Qt::MoveAction | Qt::CopyAction);
}
QStringList PlaylistModel::mimeTypes() const
{
    return QStringList("application/test");
}
