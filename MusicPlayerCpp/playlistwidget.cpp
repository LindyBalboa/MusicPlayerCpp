#include "playlistwidget.h"
#include <QApplication>
#include <QDebug>
#include <QDrag>
#include <QDragEnterEvent>
#include <QFont>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlField>
#include <QSqlResult>

PlaylistWidget::PlaylistWidget(QString playerSide, QSqlDatabase &libraryDb, QWidget *parent) : QTableView(parent)
{
    _playerSide = playerSide;
    setStyleSheet("QTableView:item:selected:active{\
                  color: black; \
                  background: qlineargradient(x1:1, y1:1, x2:1, y2:0, stop:0 #E0F0FF, stop:0.85 #E0F0FF, stop:1 white);\
                  border-top: 1px solid #A3D1FF;\
                  border-bottom: 1px solid #A3D1FF;\
                  }\
                  ");

    _libraryDb = libraryDb;
    playlistModel = new PlaylistModel(_playerSide, this, _libraryDb);
    playlistModel->setTable(_playerSide);
    setModel(playlistModel);
    playlistModel->select();
    viewport()->update();

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragDropMode(QAbstractItemView::DragDrop);
    //verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    horizontalHeader()->setHighlightSections(false);
    setFocusPolicy(Qt::StrongFocus);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setWordWrap(false);
    viewport()->setAcceptDrops(true);
    setAcceptDrops(true);
    setShowGrid(false);
    setEditTriggers(QAbstractItemView::SelectedClicked);
    show();
    }
void PlaylistWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
    isDragging = true;  //Used by PlaylistWidget::paintEvent to draw black drop line
}
void PlaylistWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    isDragging = false;
    viewport()->update();
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
    if (dropIndicatorPosition == QAbstractItemView::AboveItem) //***This looks pointless. What purpose?
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
    playlistModel->database().transaction();
    QSqlQuery query(_libraryDb);
    QString exec;
    QModelIndex dropIndex = indexAt(event->pos());
    int dropRow;
    QRect rect(0,\
               rowViewportPosition(dragHoverIndex.row()),\
               viewport()->width(),\
               15\
              );
    dropIndicatorPosition = position(event->pos(), rect, dropIndex);
        if (dropIndex.row()==-1)
        {
            dropRow = playlistModel->rowCount();
        }else if (dropIndicatorPosition == QAbstractItemView::AboveItem)
        {
            dropRow = dropIndex.row();
        } else if (dropIndicatorPosition == QAbstractItemView::BelowItem)
        {
           dropRow = dropIndex.row() + 1;
        }

    if (event->source() == this)
    {
        QModelIndexList selectedIndexesRows = this->selectionModel()->selectedRows();  //return 0 column QModelIndex for each row
        QList<int> selectedRows;
        foreach(QModelIndex index, selectedIndexesRows)
        {
            selectedRows << index.row();
        }
        qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>() );
        foreach(int row, selectedRows)
        {
            playlistModel->playlistIDList.removeAt(row);
            if (row < dropRow)
            {
                dropRow = dropRow - 1;
            }
        }
    }else
    {

    }

    const QMimeData *mimeData = event->mimeData();
    QByteArray data = mimeData->data("application/test");
    QDataStream outStream(data);
    QVariantList newIdList;
    outStream >> newIdList;
    playlistModel->playlistIDList = playlistModel->playlistIDList.mid(0, dropRow) + newIdList + playlistModel->playlistIDList.mid(dropRow,-1);
    exec = "delete from " + _playerSide; //***Clears and rebuilds the playlist table on drops. Easier to manipulate list order than maintain DB? How does it affect drag/drops if sorted by column in view?
    query.exec(exec);
    query.prepare("insert into " + _playerSide + " (IDSong) values (?)");
    query.addBindValue(playlistModel->playlistIDList);
    query.execBatch();

    playlistModel->select();
    isDragging = false;
    while(playlistModel->canFetchMore())
    {
        playlistModel->fetchMore();
    }
    this->viewport()->update();
    _libraryDb.commit();
}
void PlaylistWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)  //***Again, if sorted by column in view, this might break
    {
        _libraryDb.transaction();
        QSqlQuery query(_libraryDb);
        QModelIndexList selectedIndexesRows = this->selectionModel()->selectedRows();  //return 0 column QModelINdex for each row
        QList<int> selectedRows;
        foreach(QModelIndex index, selectedIndexesRows)
        {
            selectedRows << index.row();
        }
        qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>() );
        qDebug() << playlistModel->playlistIDList;
        foreach(int row, selectedRows)
        {
            playlistModel->playlistIDList.removeAt(row);
        }
        qDebug() << playlistModel->playlistIDList;

        query.exec("delete from " + _playerSide);
        query.prepare("insert into " + _playerSide + " (IDSong) values (?)");
        query.addBindValue(playlistModel->playlistIDList);
        query.execBatch();

        this->playlistModel->select();
        while(this->playlistModel->canFetchMore())
        {
            this->playlistModel->fetchMore();
        }
        this->viewport()->update();
        _libraryDb.commit();
    }else
    {
        event->ignore();
    }
}
void PlaylistWidget::mousePressEvent(QMouseEvent *event)
{
    startDragPos = event->pos();  //Used for Manhatten length to start dragging
    QTableView::mousePressEvent(event);
}

void PlaylistWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        int distance = (event->pos() - startDragPos).manhattanLength();
        if (distance >= QApplication::startDragDistance())
        {
            QDrag *drag = new QDrag(this);
            QModelIndexList selectedRows = this->selectionModel()->selectedRows();
            QMimeData *mimeData = this->model()->mimeData(selectedRows);
            drag->setMimeData(mimeData);
            drag->exec(Qt::CopyAction);
        }
    }

}
void PlaylistWidget::paintEvent(QPaintEvent *event)
{
    QTableView::paintEvent(event);
    if (isDragging==true)
    {
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

PlaylistModel::PlaylistModel(QString playerSide, QWidget *parent, QSqlDatabase libraryDb) : QSqlTableModel(parent, libraryDb)
{
    _playerSide = playerSide;
    setSort(0, Qt::AscendingOrder);
    QSqlQuery query(libraryDb);
    query.exec("SELECT * from " + _playerSide);
    while(query.next())
    {
        playlistIDList.append(query.value(1).toInt());  //Rebuild list from database upon loading
    }
}
PlaylistModel::~PlaylistModel()
{
}
Qt::ItemFlags PlaylistModel::flags(const QModelIndex &index) const
{
    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}
QMimeData* PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QList<int> rows;
    QVariantList IDSongList;
    foreach (QModelIndex index, indexes)
    {
        if (index.isValid())
        {
            if (rows.contains(index.row()) == false)
            {
                rows.append(index.row());
            }
        }
    }
    for (int i = rows.size()-1; i>=0; i--)
    {
        IDSongList << this->record(rows[i]).value("IDSong").toInt();
    }
    for(int k = 0; k < (IDSongList.size()/2); k++)
        IDSongList.swap(k,IDSongList.size()-(1+k));
    stream << IDSongList;

    mimeData->setData("application/test", encodedData);

    return mimeData;
}
QStringList PlaylistModel::mimeTypes() const
{
    return QStringList("application/test");
}
QString PlaylistModel::selectStatement() const
{
    return QString("SELECT * FROM " + _playerSide + " A LEFT JOIN Songs B ON A.IDSong = B.IDSong ORDER BY IDNowPlaying ASC");
}
Qt::DropActions PlaylistModel::supportedDragActions() const
{
    return (Qt::MoveAction | Qt::CopyAction);
}
Qt::DropActions PlaylistModel::supportedDropActions() const
{
    return (Qt::MoveAction | Qt::CopyAction);
}


