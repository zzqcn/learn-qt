#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QItemSelection>
#include <QMainWindow>
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

private:
  Ui::MainWindow *ui;

  QStandardItemModel *model;
};
#endif // MAINWINDOW_H
