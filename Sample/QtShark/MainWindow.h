#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QStandardItemModel>

#include "Packet.h"
#include "PacketListModel.h"
#include "ProtoTreeModel.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
//  QStandardItemModel *generateProtoTreeModel(const Packet *pkt);

private slots:
  void handlePktListSelectionChanged(const QItemSelection &selected,
                                     const QItemSelection &deselected);
//  void handleProtoTreeSelectionChanged(const QItemSelection &selected,
//                                       const QItemSelection &deselected);
  void handleProtoTreeClicked(const QModelIndex &index);

private:
  Ui::MainWindow *ui;
  PacketListModel* pktListModel;
  ProtoTreeModel* protoTreeModel;
};
#endif // MAINWINDOW_H
