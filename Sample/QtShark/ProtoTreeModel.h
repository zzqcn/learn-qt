#ifndef __PROTO_TREE_MODEL_H__
#define __PROTO_TREE_MODEL_H__

#include <QAbstractItemModel>

#include "Packet.h"

// see: https://doc.qt.io/qt-6/qtwidgets-itemviews-simpletreemodel-example.html

class ProtoTreeModel : public QAbstractItemModel {
  Q_OBJECT

public:
  explicit ProtoTreeModel(QObject *parent = 0);
  ~ProtoTreeModel();

  int setProtoTree(ProtoTree* tree);

  Qt::ItemFlags flags(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &index) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:

private:
  ProtoTree* root;
};

#endif
