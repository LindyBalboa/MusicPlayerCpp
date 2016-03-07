#include <QTableWidget>
#include <QSqlTableModel>
#include <QSqlDatabase>
#include <QMimeData>
#ifndef PLAYLISTWIDGET_H
#define PLAYLISTWIDGET_H


class PlaylistWidget : public QTableView
{
    public:
        PlaylistWidget(QWidget *parent = 0, QString playerSide = "none");
    private:

    protected:
        void dragEnterEvent(QDragEnterEvent *event) Q_DECL_OVERRIDE;
        void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;
        void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

class PlaylistModel : public QSqlTableModel
{
    public:
        PlaylistModel(QWidget *parent =0, QSqlDatabase playlistDb=QSqlDatabase());
    private:
       Qt::ItemFlags flags(const QModelIndex &index) const;
       Qt::DropActions supportedDragActions() const;
       Qt::DropActions supportedDropActions() const;
       QStringList mimeTypes() const;
};


#endif // PLAYLISTWIDGET_H
