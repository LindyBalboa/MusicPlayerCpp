#include "playlistwidget.h"
#include <QSqlTableModel>
#include <QTableView>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QHeaderView>
#include <QDragEnterEvent>
#include <QPainter>
#include <QFont>

PlaylistWidget::PlaylistWidget(QString playerSide, QWidget *parent) : QTableView(parent)
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
    playlistModel = new PlaylistModel(this, playlistDb);
    setModel(playlistModel);
    playlistModel->setTable("playlist");
    playlistModel->select();

    QSqlRecord record = playlistDb.record("playlist");
    for (int i=0; i<10; i++)
    {
        record.setValue(1, i);
        playlistModel->insertRecord(i,record);
    }
    //qDebug() << record.value(1);
    playlistModel->select();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setDragEnabled(true);
    setShowGrid(false);
    setAcceptDrops(true);
    setEditTriggers(QAbstractItemView::SelectedClicked);
    show();
    }
void PlaylistWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    isDragging = true;
}
void PlaylistWidget::dragMoveEvent(QDragMoveEvent *event)
{
    dragHoverIndex = indexAt(event->pos());
    QRect rect(0,\
                        rowViewportPosition(dragHoverIndex.row()),\
                        viewport()->width(),\
                        15\
               );
    dropIndicatorPosition = position(event->pos(), rect, dragHoverIndex);
    //qDebug() << "DIP" << dropIndicatorPosition;
    if (dropIndicatorPosition == QAbstractItemView::AboveItem)
    {
        event->accept();
    } else if (dropIndicatorPosition == QAbstractItemView::BelowItem)
    {
        event->accept();
    }
    viewport()->update();
}
void PlaylistWidget::dropEvent(QDropEvent *event)
{

    QModelIndex dropIndex = indexAt(event->pos());
    QRect rect(0,\
                        rowViewportPosition(dragHoverIndex.row()),\
                        viewport()->width(),\
                        15\
               );
    dropIndicatorPosition = position(event->pos(), rect, dropIndex);
    const QMimeData *mimeData = event->mimeData();
    QByteArray data = mimeData->data("application/test");
    QDataStream outStream(data);
    while (!outStream.atEnd())
    {
        QMap<QString, QVariant> trackRecord;
        outStream >> trackRecord;
        if (!trackRecord.empty())
        {
            QSqlRecord record;
            QMapIterator<QString, QVariant> i(trackRecord);
            while (i.hasNext())
            {
                i.next();
                QSqlField field(i.key());
                record.append(field);
                record.setValue(i.key(),i.value());
            }

            if (dropIndicatorPosition == QAbstractItemView::AboveItem)
            {
                playlistModel->insertRecord(indexAt(event->pos()).row(),record);
            } else if (dropIndicatorPosition == QAbstractItemView::BelowItem)
            {
                playlistModel->insertRecord(indexAt(event->pos()).row()+1,record);
            }
            //viewport()->update();

        }
    }
    //QTableView::dropEvent(event);
    isDragging = false;
}
void PlaylistWidget::mousePressEvent(QMouseEvent *event)
{
    QTableView::mousePressEvent(event);
}
void PlaylistWidget::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    if (isDragging==true)
    {
        //qDebug() << "DHI" << dragHoverIndex;
        //qDebug() << dragHoverIndex;
        QPainter qP(viewport());
        QPen pen(Qt::black, 1.5, Qt::SolidLine);
        qP.setPen(pen);
        if (dragHoverIndex == QModelIndex())
        {
            qP.drawLine(0,\
                            rowViewportPosition(playlistModel->rowCount()-1)+15,\
                            viewport()->width(),\
                            rowViewportPosition(playlistModel->rowCount()-1)+15\
                            );
        }
        else if(dropIndicatorPosition == QAbstractItemView::AboveItem)
        {
            qP.drawLine(0,\
                            rowViewportPosition(dragHoverIndex.row()),\
                            viewport()->width(),\
                            rowViewportPosition(dragHoverIndex.row())\
                            );
        }
        else if (dropIndicatorPosition == QAbstractItemView::BelowItem)
        {
            qP.drawLine(0,\
                            rowViewportPosition(dragHoverIndex.row())+15,\
                            viewport()->width(),\
                            rowViewportPosition(dragHoverIndex.row())+15\
                            );
        }
    }
}
QAbstractItemView::DropIndicatorPosition PlaylistWidget::position(const QPoint &pos, const QRect &rect, const QModelIndex &index) const
{
    QAbstractItemView::DropIndicatorPosition r;

    if (pos.y() < rect.top()+.5*(rect.bottom()-rect.top()))
    {
        r = QAbstractItemView::AboveItem;
    } else if (pos.y() >= rect.top()+.5*(rect.bottom()-rect.top()))
    {
        r = QAbstractItemView::BelowItem;
    } else if (rect.contains(pos, true))
    {
        r = QAbstractItemView::OnItem;
    }

    return r;
}


PlaylistModel::PlaylistModel(QWidget *parent, QSqlDatabase playlistDb) : QSqlTableModel(parent, playlistDb)
{

}
Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QStringList PlaylistModel::mimeTypes() const
{
    return QStringList("application/test");
}
Qt::DropActions PlaylistModel::supportedDragActions() const
{
    return (Qt::MoveAction | Qt::CopyAction);
}
Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return (Qt::MoveAction | Qt::CopyAction);
}
