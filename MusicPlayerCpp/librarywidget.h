#ifndef MODELS_H
#define MODELS_H
#include <QtSql>
class LibrarySqlTableModel : public QSqlTableModel {
    Q_OBJECT

    public:
        LibrarySqlTableModel(QWidget *parent = 0, QSqlDatabase database = QSqlDatabase());
        ~LibrarySqlTableModel();
        Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;

};

#endif // MODELS_H
