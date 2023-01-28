#include "MainWindow.h"
#include "./ui_MainWindow.h"

struct Profile {
  QString name;
  uint32_t age;
  QStringList hobbies;
};

// clang-format off
const Profile g_profiles[] = {
    {"Tom",  22, {"编程", "玩游戏", "看电视"}},
    {"Jim",  38, {"旅游", "钓鱼"}},
    {"Lucy", 23, {"看电影", "购物"}}
};
// clang-format on

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  model = new QStandardItemModel;
  const QStringList headers = {"Name", "Age", "Hobbies"};
  QStandardItem *item;
  const Profile *profile;

  model->setHorizontalHeaderLabels(headers);
  size_t count = sizeof(g_profiles) / sizeof(Profile);
  for (uint32_t i = 0; i < count; i++) {
    QList<QStandardItem *> items;
    profile = &g_profiles[i];
    item = new QStandardItem(profile->name);
    items.append(item);
    item = new QStandardItem(tr("%1").arg(profile->age));
    items.append(item);
    item = new QStandardItem(profile->hobbies.join(','));
    items.append(item);
    model->appendRow(items);
  }
  ui->treeView->setModel(model);
}

MainWindow::~MainWindow() { delete ui; }
