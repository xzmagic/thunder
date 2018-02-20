#ifndef OBJECTHIERARCHYMODEL_H
#define OBJECTHIERARCHYMODEL_H

#include <QAbstractItemModel>

class AObject;

class ObjectHierarchyModel : public QAbstractItemModel {
public:
    ObjectHierarchyModel        (QObject *parent);

    void                        setRoot                     (AObject *scene);

    AObject                    *root                        () const { return m_rootItem; }

    AObject                    *findObject                  (const QString &ref);

    void                        reset                       ();

    QVariant                    data                        (const QModelIndex &index, int role) const;

    bool                        setData                     (const QModelIndex &index, const QVariant &value, int role);

    QVariant                    headerData                  (int section, Qt::Orientation orientation, int role) const;

    int                         columnCount                 (const QModelIndex &parent = QModelIndex()) const;

    int                         rowCount                    (const QModelIndex &parent = QModelIndex()) const;

    QModelIndex                 parent                      (const QModelIndex &index) const;

    QModelIndex                 index                       (int row, int column, const QModelIndex &parent = QModelIndex()) const;

    Qt::ItemFlags               flags                       (const QModelIndex &index) const;

protected:
    AObject                    *m_rootItem;

};

#endif // OBJECTHIERARCHYMODEL_H