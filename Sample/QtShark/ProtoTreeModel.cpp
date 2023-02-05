#include <QDebug>

#include "ProtoTreeModel.h"

ProtoTreeModel::ProtoTreeModel(QObject *parent) : QAbstractItemModel(parent) {}

ProtoTreeModel::~ProtoTreeModel() {}

int ProtoTreeModel::setProtoTree(ProtoTree *tree) {
  beginResetModel();
  root = tree;
  endResetModel();
  return 0;
}

Qt::ItemFlags ProtoTreeModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

int ProtoTreeModel::rowCount(const QModelIndex &parent) const {
  if (parent.column() > 0)
    return 0;

  ProtoTree *parentTree;
  if (!parent.isValid())
    parentTree = root;
  else
    parentTree = static_cast<ProtoTree *>(parent.internalPointer());

  return parentTree->childCount();
}

int ProtoTreeModel::columnCount(const QModelIndex &parent) const {
  if (parent.isValid())
    return static_cast<ProtoTree *>(parent.internalPointer())->columnCount();

  if (root)
    return root->columnCount();

  return 0;
}

QVariant ProtoTreeModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  return QVariant();
}

QModelIndex ProtoTreeModel::index(int row, int column,
                                  const QModelIndex &parent) const {
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  ProtoTree *parentTree;
  ProtoTree *child;

  if (!parent.isValid())
    parentTree = root;
  else
    parentTree = static_cast<ProtoTree *>(parent.internalPointer());
  child = parentTree->child(row);
  if (child)
    return createIndex(row, column, child);

  return QModelIndex();
}

QModelIndex ProtoTreeModel::parent(const QModelIndex &index) const {
  if (!index.isValid())
    return QModelIndex();

  ProtoTree *childTree = static_cast<ProtoTree *>(index.internalPointer());
  ProtoTree *parentTree = childTree->parent;
  if (parentTree == root)
    return QModelIndex();

  return createIndex(parentTree->row(), 0, parentTree);
}

QVariant ProtoTreeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() || root == nullptr)
    return QVariant();

  ProtoTree *tree = static_cast<ProtoTree *>(index.internalPointer());

  switch (role) {
  case Qt::DisplayRole:
    if (tree->field != nullptr) {
      return QString("%1 : %2").arg(tree->field->key.c_str(),
                                    tree->field->value.c_str());
    } else {
      return "Unknown";
    }
    // break;
  case Qt::BackgroundRole:
    // if(pkt->mark_flag & 0x1) {
    //   return QBrush(QColor(0xf4, 0xcc, 0xcc));
    // }
    break;
  default:
    break;
  }

  return QVariant();
}
