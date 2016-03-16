#include "playlistwidget.h"
#include <QDebug>
#include <QSqlField>
#include <QHeaderView>
#include <QDragEnterEvent>
#include <QFont>
#include <QSqlError>
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
    memDb = QSqlDatabase::addDatabase("QSQLITE", _playerSide);
    memDb.setDatabaseName(":memory:");
    memDb.open();
    QSqlQuery query(memDb);
    query.exec("ATTACH DATABASE \"database.db\" as \"library\" ");
    _libraryDb.transaction();
    _libraryDb.commit();
    memDb.transaction();
    query.exec(QString("create table left(IDNowPlaying INTEGER,\
                                        IDSong INTEGER)"));
    query.exec("INSERT INTO left SELECT * FROM library.left");
    memDb.commit();
    query.clear();
    playlistModel = new PlaylistModel(_playerSide, this, _libraryDb);
    playlistModel->setTable(_playerSide);
    setModel(playlistModel);
    playlistModel->select();
    viewport()->update();
    //playlistModel->setQuery(QString("SELECT * FROM %1 A\
      //                              LEFT JOIN table1 B\
        //                            ON A.IDSong = B.IDSong").arg(_playerSide), libraryDb);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDropIndicatorShown(true);
    viewport()->setAcceptDrops(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    //verticalHeader()->hide();
    verticalHeader()->setDefaultSectionSize(15);
    horizontalHeader()->setSectionsMovable(true);
    horizontalHeader()->setFixedHeight(20);
    horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    setFocusPolicy(Qt::StrongFocus);
    setDragEnabled(true);
    setShowGrid(false);
    setAcceptDrops(true);
    setEditTriggers(QAbstractItemView::SelectedClicked);
    show();
    }
QMimeData* PlaylistModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);
    QList<int> rows;
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
    qSort(rows.begin(),rows.end());
    stream << rows.size() ;
    for (int i = rows.size()-1; i>=0; i--)
    {
        stream << this->record(rows[i]).value("IDSong").toInt();
    }

    mimeData->setData("application/test", encodedData);

    return mimeData;
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

void PlaylistWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    isDragging = false;
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
            qDebug() << "APPEND";
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
        QModelIndexList selectedIndexesRows = this->selectionModel()->selectedRows(); //return 0 column QModelINdex for each row
        QList<int> selectedRows;
        foreach(QModelIndex index, selectedIndexesRows)
        {
            selectedRows << index.row();
        }
        qDebug() << selectedRows;
        qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>() );
        foreach(int row, selectedRows)
        {
            exec = QString("DELETE FROM %1 WHERE IDNowPlaying = %2").arg(_playerSide,QString::number(row+1));
            query.exec(exec);
            exec = QString("UPDATE %1 SET IDNowPlaying = IDNowPlaying - 1 WHERE IDNowPlaying > %2").arg(_playerSide,QString::number(row+1));
            query.exec(exec);

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
    int numberOfRows;
    outStream >> numberOfRows;
    qDebug() << "#Ro" << numberOfRows;
    exec = QString("UPDATE %1 SET IDNowPlaying = -(IDNowPlaying + %2) where IDNowPlaying > %3").arg(_playerSide,QString::number(numberOfRows),QString::number(dropRow));
    query.exec(exec);
    exec = QString("UPDATE %1 SET IDNowPlaying = -IDNowPlaying WHERE IDNowPlaying<0").arg(_playerSide);
    query.exec(exec);

    int j=numberOfRows; //Doing it this way is necessary to maintiain the proper insertion order
    while (j>0)
    {
        int IDSong;
        outStream >> IDSong;
        exec = QString("INSERT INTO %1 (IDNowPlaying, IDSong) VALUES (%2, %3)").arg(_playerSide, QString::number(dropRow+j), QString::number(IDSong));
        query.exec(exec);
        j--;
    }
    _libraryDb.commit();
    playlistModel->select();
    isDragging = false;
    this->viewport()->update();
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


PlaylistModel::PlaylistModel(QString playerSide, QWidget *parent, QSqlDatabase libraryDb) : QSqlTableModel(parent, libraryDb)
{
    _playerSide = playerSide;
    setSort(0, Qt::AscendingOrder);
}
PlaylistModel::~PlaylistModel()
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
QString PlaylistModel::selectStatement() const
{
    return QString("SELECT * FROM " + _playerSide + " A LEFT JOIN table1 B ON A.IDSong = B.IDSong ORDER BY IDNowPlaying ASC");
}
void PlaylistWidget::saveNowPlaying()
{
}

void PlaylistWidget::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        _libraryDb.transaction();
        QSqlQuery query(_libraryDb);
        QModelIndexList selectedIndexesRows = this->selectionModel()->selectedRows(); //return 0 column QModelINdex for each row
        QList<int> selectedRows;
        foreach(QModelIndex index, selectedIndexesRows)
        {
            selectedRows << index.row();
        }
        qSort(selectedRows.begin(), selectedRows.end(), qGreater<int>() );
        foreach(int row, selectedRows)
        {
            query.exec(QString("DELETE FROM %1 WHERE IDNowPlaying = %2").arg(_playerSide,QString::number(row+1)));
            query.exec(QString("UPDATE %1 SET IDNowPlaying = IDNowPlaying - 1 WHERE IDNowPlaying > %2").arg(_playerSide,QString::number(row+1)));
        }
        this->playlistModel->select();
        while(this->playlistModel->canFetchMore())
        {
            this->playlistModel->fetchMore();
        }
        _libraryDb.commit();

    }else
    {
    event->ignore();
    }
}

