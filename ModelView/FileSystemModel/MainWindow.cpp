#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QFileSystemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  QFileSystemModel *model = new QFileSystemModel;
  // Note: This function does not change the structure of the model or modify
  // the data available to views. In other words, the "root" of the model is not
  // changed to include only files and directories within the directory
  // specified by newPath in the file system.
  model->setRootPath(QDir::homePath());
  ui->treeView->setModel(model);
  // 要仅显示某个目录内容, 需要调用view->setRootIndex()
  ui->treeView->setRootIndex(model->index(QDir::homePath()));
}

MainWindow::~MainWindow() { delete ui; }
