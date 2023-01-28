#include "MainWindow.h"
#include "./ui_MainWindow.h"
#include <QMessageBox>
#include <QStringList>

// validate data
// https://stackoverflow.com/questions/18470445/qlistview-qstandarditemmodel-check-text-before-editing-row

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  sort_order = Qt::AscendingOrder;

  QStringList str_list;
  str_list << "AAAAA"
           << "CCCCC"
           << "BBBBB"
           << "DDDDD";
  // 也可以直接使用QStringListModel
  model = new QStandardItemModel;
  QStandardItem *item;
  for (int i = 0; i < str_list.size(); i++) {
    item = new QStandardItem(str_list[i]);
    model->appendRow(item);
  }

  proxyModel = new QSortFilterProxyModel;
  proxyModel->setSourceModel(model);
  ui->listView->setModel(proxyModel);

  connect(ui->listView->selectionModel(),
          &QItemSelectionModel::selectionChanged, this,
          &MainWindow::handleSelectionChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleSelectionChanged(const QItemSelection &selected,
                                        const QItemSelection &deselected) {
  if (selected.indexes().isEmpty()) {
  } else {
    int row = selected.indexes().first().row();
    ui->label->setText(tr("选中第%1行").arg(row));
  }
}

void MainWindow::on_btnOrder_clicked() {
  proxyModel->sort(0, sort_order);
  sort_order = (sort_order == Qt::AscendingOrder ? Qt::DescendingOrder
                                                 : Qt::AscendingOrder);
}

void MainWindow::on_btnAdd_clicked() {
  int row = model->rowCount();
  //  QMessageBox::information(this, "information", tr("row count
  //  %1").arg(row));
  QStandardItem *item = new QStandardItem;
  model->appendRow(item);

  QModelIndex index = proxyModel->index(row, 0);
  ui->listView->setCurrentIndex(index);
  ui->listView->edit(index);
}

void MainWindow::on_btnEdit_clicked() {
  QModelIndexList selectedRows = ui->listView->selectionModel()->selectedRows();
  if (selectedRows.empty()) {
    QMessageBox::warning(this, "warning", "请先选中一项");
    return;
  }
  QModelIndex index = selectedRows.first();
  if (index.isValid()) {
    ui->listView->edit(index);
  }
}

void MainWindow::on_btnDelete_clicked() {
  QModelIndexList selectedRows = ui->listView->selectionModel()->selectedRows();
  if (selectedRows.empty()) {
    QMessageBox::warning(this, "warning", "请先选中一项");
    return;
  }
  QModelIndex index = selectedRows.first();
  if (index.isValid()) {
    model->removeRow(index.row());
  }
}
