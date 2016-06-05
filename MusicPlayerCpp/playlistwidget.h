#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <playlistwidget.h>

#include <QMap>
#include <QMapIterator>
#include <QMimeData>
#include <QPainter>
#include <QPoint>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QTableView>

class PlaylistModel : public QSqlTableModel
{
    Q_OBJECT

    public:
        QVariantList playlistIDList;
        PlaylistModel(QString playerSide, QWidget *parent =0, QSqlDatabase playlistDb=QSqlDatabase());
        ~PlaylistModel();
    protected:
        QMimeData *mimeData(const QModelIndexList &indexes) const;
        virtual QString selectStatement() const;
    private:
        QString _playerSide;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QStringList mimeTypes() const;
        Qt::DropActions supportedDragActions() const;
        Qt::DropActions supportedDropActions() const;
};

class PlaylistWidget : public QTableView
{
    Q_OBJECT
    public:
        PlaylistWidget(QString playerSide, QSqlDatabase &libraryDb,  QWidget *parent = 0);
        PlaylistModel* playlistModel;
        int currentTrackID;
    public slots:
        void dragLeaveEvent(QDragLeaveEvent *event);
    private:
        QPoint startDragPos;
        bool isDragging = false;
        QSqlDatabase _libraryDb;
        QSqlDatabase memDb;
        QString _playerSide;
        QAbstractItemView::DropIndicatorPosition dropIndicatorPosition;
        QAbstractItemView::DropIndicatorPosition position(const QPoint &pos, const QRect &rect, const QModelIndex &index) const;
        QModelIndex dragHoverIndex;
        void paintEvent(QPaintEvent *event) ;
        void dragMoveEvent(QDragMoveEvent *event) ;
        void dragEnterEvent(QDragEnterEvent *event) ;
        void mousePressEvent(QMouseEvent *event) ;
        void mouseMoveEvent(QMouseEvent *event) ;
        void dropEvent(QDropEvent *event);
    protected:
        void keyReleaseEvent(QKeyEvent *);
};


#endif // PLAYLISTWIDGET_H
