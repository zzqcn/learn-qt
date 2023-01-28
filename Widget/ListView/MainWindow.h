#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>

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

private slots:
  void handleSelectionChanged(const QItemSelection &selected,
                              const QItemSelection &deselected);

  void on_btnOrder_clicked();

  void on_btnAdd_clicked();

  void on_btnEdit_clicked();

  void on_btnDelete_clicked();

private:
  Ui::MainWindow *ui;

  QStandardItemModel *model;
  QSortFilterProxyModel *proxyModel;
  Qt::SortOrder sort_order;
};

#endif // MAINWINDOW_H
