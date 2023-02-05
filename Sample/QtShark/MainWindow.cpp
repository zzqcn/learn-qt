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

void generatePacketListModel(PacketListModel *model) {
  int ret = 0;
  Packet *pkt;
  ProtoTree *tree;
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
    model->addPacket(pkt);
  }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  this->setWindowTitle("QtShark");

  // underlying data model
  pktListModel = new PacketListModel;
  generatePacketListModel(pktListModel);
  protoTreeModel = new ProtoTreeModel;

  // packet list view
  ui->pktList->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->pktList->setItemsExpandable(false);
  ui->pktList->setRootIsDecorated(false);
  //  ui->pktList->setUniformRowHeights(true);
  ui->pktList->setAlternatingRowColors(true);
  ui->pktList->setModel(pktListModel);
  for (int i = 0; i < pktListModel->columnCount(); i++) {
    ui->pktList->resizeColumnToContents(i);
  }

  // proto tree view
  ui->protoTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
  ui->protoTree->setHeaderHidden(true);
  protoTreeModel->setProtoTree(g_protoTrees[0]);
  ui->protoTree->setModel(protoTreeModel);
  ui->protoTree->expandAll();

  // select row 1 in packet list by default
  QModelIndex selectIndex = pktListModel->index(0, 0);
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
    protoTreeModel->setProtoTree(g_protoTrees[row]);
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
  ProtoTree *tree = static_cast<ProtoTree *>(index.internalPointer());
  if (tree && tree->field) {
    //    qDebug() << tree->field->beginOff << tree->field->endOff;
    ui->byteView->setRange(tree->field->pos, tree->field->length);
  }
}
