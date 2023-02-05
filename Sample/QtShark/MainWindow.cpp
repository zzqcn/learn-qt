#include "MainWindow.h"
#include "ByteViewTab.h"
#include "ui_MainWindow.h"
#include <QItemSelectionModel>
#include <QScreen>
#include <QStringList>

struct RawData {
  uint8_t *data;
  size_t dataLen;
};

uint8_t data0[] = {
    0xb8, 0xf8, 0x83, 0x2d, 0x5d, 0xf4, 0x84, 0xa9, 0x3e, 0x7c, 0x54,
    0x4f, 0x08, 0x00, 0x45, 0x00, 0x00, 0x34, 0xec, 0x6c, 0x40, 0x00,
    0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x67, 0xd1, 0x33, 0xbc,
    0x94, 0x19, 0x6b, 0x00, 0x50, 0xa7, 0x4c, 0x44, 0x8a, 0x00, 0x00,
    0x00, 0x00, 0x80, 0x02, 0xfa, 0xf0, 0x4f, 0xfe, 0x00, 0x00, 0x02,
    0x04, 0x05, 0xb4, 0x01, 0x03, 0x03, 0x08, 0x01, 0x01, 0x04, 0x02,
};
uint8_t data1[] = {
    0x84, 0xa9, 0x3e, 0x7c, 0x54, 0x4f, 0xb8, 0xf8, 0x83, 0x2d, 0x5d,
    0xf4, 0x08, 0x00, 0x45, 0x00, 0x00, 0x34, 0x00, 0x00, 0x40, 0x00,
    0x29, 0x06, 0x01, 0xed, 0xd1, 0x33, 0xbc, 0x94, 0xc0, 0xa8, 0x01,
    0x67, 0x00, 0x50, 0x19, 0x6b, 0x23, 0x6c, 0xfe, 0xec, 0xa7, 0x4c,
    0x44, 0x8b, 0x80, 0x12, 0x72, 0x10, 0x85, 0x41, 0x00, 0x00, 0x02,
    0x04, 0x05, 0xa0, 0x01, 0x01, 0x04, 0x02, 0x01, 0x03, 0x03, 0x07,
};
uint8_t data2[] = {
    0xb8, 0xf8, 0x83, 0x2d, 0x5d, 0xf4, 0x84, 0xa9, 0x3e, 0x7c, 0x54,
    0x4f, 0x08, 0x00, 0x45, 0x00, 0x00, 0x28, 0xec, 0x6e, 0x40, 0x00,
    0x80, 0x06, 0x00, 0x00, 0xc0, 0xa8, 0x01, 0x67, 0xd1, 0x33, 0xbc,
    0x94, 0x19, 0x6b, 0x00, 0x50, 0xa7, 0x4c, 0x44, 0x8b, 0x23, 0x6c,
    0xfe, 0xed, 0x50, 0x10, 0x02, 0x05, 0x4f, 0xf2, 0x00, 0x00,
};

RawData g_rawData[3] = {
    {data0, sizeof(data0)},
    {data1, sizeof(data1)},
    {data2, sizeof(data2)},
};
Packet *g_pkts[3];
ProtoTree *g_protoTrees[3];

void generatePacketModel(QStandardItemModel *model) {
  int ret = 0;
  Packet *pkt;
  ProtoTree *tree;
  QStandardItem *item;
  unsigned i;

  for (i = 0; i < 3; i++) {
    pkt = new Packet;
    tree = new ProtoTree;
    ret = dissectPacket(g_rawData[i].data, g_rawData[i].dataLen, pkt, tree);
    if (ret != 0) {
      delete pkt;
      delete tree;
      continue;
    }
    pkt->index = i;

    g_pkts[i] = pkt;
    g_protoTrees[i] = tree;

    QList<QStandardItem *> items;
    item = new QStandardItem(QString("%1").arg(pkt->index + 1));
    items.append(item);
    item = new QStandardItem(ip_to_string(pkt->srcIP).c_str());
    items.append(item);
    item = new QStandardItem(ip_to_string(pkt->dstIP).c_str());
    items.append(item);
    item = new QStandardItem(port_to_string(pkt->srcPort).c_str());
    items.append(item);
    item = new QStandardItem(port_to_string(pkt->dstPort).c_str());
    items.append(item);
    item = new QStandardItem(proto_to_string(pkt->proto).c_str());
    items.append(item);
    item = new QStandardItem(QString("%1").arg(pkt->dataLen));
    items.append(item);
    item = new QStandardItem(pkt->info.c_str());
    items.append(item);
    model->appendRow(items);
  }
}

void ProtoTreeToItem(ProtoTree *tree, QStandardItem *parent) {
  QStandardItem *item;
  ProtoField *field = tree->field;

  if (tree->field != nullptr) {
    item = new QStandardItem(
        QString("%1 : %2").arg(field->key.c_str(), field->value.c_str()));
    QSize sz(tree->field->beginOff, tree->field->endOff);
    item->setData(sz);
  } else {
    item = new QStandardItem("Unknown");
    item->setData(QSize(-1, -1));
  }

  for (size_t i = 0; i < tree->childs.size(); i++) {
    ProtoTreeToItem(tree->childs[i], item);
  }

  parent->appendRow(item);
}

QStandardItemModel *MainWindow::generateProtoTreeModel(const Packet *pkt) {
  ProtoTree *root = g_protoTrees[pkt->index];
  QStandardItemModel *model = new QStandardItemModel;

  ProtoTreeToItem(root, model->invisibleRootItem());

  return model;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setWindowTitle("QtShark");

  // underlying data model
  QStringList headers;
  headers << "No."
          << "SrcIP"
          << "DstIP"
          << "SrcPort"
          << "DstPort"
          << "Protocol"
          << "Length"
          << "Info";
  QStandardItemModel *pktModel = new QStandardItemModel;
  pktModel->setHorizontalHeaderLabels(headers);
  // dissect raw data to generate packet list model and proto trees
  generatePacketModel(pktModel);

  // packet list view
  ui->pktList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->pktList->setModel(pktModel);
  ui->pktList->setAlternatingRowColors(true);
  for (int i = 0; i < pktModel->columnCount(); i++) {
    ui->pktList->resizeColumnToContents(i);
  }

  // proto tree view
  ui->protoTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->protoTree->setHeaderHidden(true);
  QStandardItemModel *model = generateProtoTreeModel(g_pkts[0]);
  ui->protoTree->setModel(model);
  QModelIndex rootIndex =
      model->index(0, 0, model->invisibleRootItem()->index());
  ui->protoTree->setRootIndex(rootIndex);
  ui->protoTree->expandAll();

  // select row 1 in packet list by default
  QModelIndex selectIndex = pktModel->index(0, 0);
  ui->pktList->selectionModel()->select(
      selectIndex, QItemSelectionModel::Select | QItemSelectionModel::Rows);
  ui->byteView->setPacket(g_pkts[0]);

  // set UI layout
  // 在保持比例的情况下，绝对值要尽量大
  int largeWidth = QGuiApplication::primaryScreen()->size().width();
  ui->hsplit->setSizes(QList<int>({largeWidth, largeWidth}));

  // setup signal/slot connection
  connect(ui->pktList->selectionModel(), &QItemSelectionModel::selectionChanged,
          this, &MainWindow::handlePktListSelectionChanged);
  //  connect(ui->protoTree->selectionModel(),
  //          &QItemSelectionModel::selectionChanged, this,
  //          &MainWindow::handleProtoTreeSelectionChanged);
  connect(ui->protoTree, &QTreeView::clicked, this,
          &MainWindow::handleProtoTreeClicked);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handlePktListSelectionChanged(
    const QItemSelection &selected, const QItemSelection &deselected) {
  if (!selected.indexes().isEmpty()) {
    QModelIndex index = selected.indexes().first();
    int row = index.row();
    QStandardItemModel *model = generateProtoTreeModel(g_pkts[row]);
    ui->protoTree->setModel(model);
    QModelIndex rootIndex =
        model->index(0, 0, model->invisibleRootItem()->index());
    ui->protoTree->setRootIndex(rootIndex);
    ui->protoTree->expandAll();

    ui->byteView->setPacket(g_pkts[row]);
  }
}

// void MainWindow::handleProtoTreeSelectionChanged(
//     const QItemSelection &selected, const QItemSelection &deselected) {
//   if (!selected.indexes().isEmpty()) {
//     QModelIndex index = selected.indexes().first();
//     QSize sz = ui->protoTree->model()->data(index, Qt::UserRole +
//     1).toSize(); qDebug() << sz; ui->byteView->setRange(sz.width(),
//     sz.height());
//   } else {
//     qDebug() << "tree view empty select";
//   }
// }

void MainWindow::handleProtoTreeClicked(const QModelIndex &index) {
  QSize sz = ui->protoTree->model()->data(index, Qt::UserRole + 1).toSize();
  //  qDebug() << sz;
  ui->byteView->setRange(sz.width(), sz.height());
}
