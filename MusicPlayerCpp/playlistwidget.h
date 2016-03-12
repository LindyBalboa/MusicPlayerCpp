#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H

#include <QTableWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QMimeData>
#include <QMap>
#include <QMapIterator>
#include <Qpainter>
#include <playlistwidget.h>

class PlaylistModel : public QSqlTableModel
{
    Q_OBJECT

public:
    PlaylistModel(QWidget *parent =0, QSqlDatabase playlistDb=QSqlDatabase());
protected:
private:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDragActions() const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
};


class PlaylistWidget : public QTableView
{
    Q_OBJECT

    public:
        PlaylistWidget(QString playerSide = "none", QWidget *parent = 0);
        PlaylistModel* playlistModel;
        int currentTrackID;
    private:
        bool isDragging = false;
        QAbstractItemView::DropIndicatorPosition dropIndicatorPosition;
        QAbstractItemView::DropIndicatorPosition position(const QPoint &pos, const QRect &rect, const QModelIndex &index) const;
        QModelIndex dragHoverIndex;
        void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
        void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
        void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void dropEvent(QDropEvent *event);
protected:
};


#endif // PLAYLISTWIDGET_H
