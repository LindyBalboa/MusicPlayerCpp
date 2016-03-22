#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <playlistwidget.h>

#include <QMap>
#include <QMapIterator>
#include <QMimeData>
#include <QPainter>
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
        PlaylistModel(QString playerSide, QWidget *parent =0, QSqlDatabase playlistDb=QSqlDatabase());
        ~PlaylistModel();
    protected:
        QMimeData *mimeData(const QModelIndexList &indexes) const;
        virtual QString selectStatement() const; Q_DECL_OVERRIDE
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
        void saveNowPlaying();
    public slots:
        void dragLeaveEvent(QDragLeaveEvent *event);
    private:
        bool isDragging = false;
        QSqlDatabase _libraryDb;
        QSqlDatabase memDb;
        QString _playerSide;
        QAbstractItemView::DropIndicatorPosition dropIndicatorPosition;
        QAbstractItemView::DropIndicatorPosition position(const QPoint &pos, const QRect &rect, const QModelIndex &index) const;
        QModelIndex dragHoverIndex;
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
        void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
        void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void dropEvent(QDropEvent *event);
    protected:
        void keyReleaseEvent(QKeyEvent *);
};


#endif // PLAYLISTWIDGET_H
