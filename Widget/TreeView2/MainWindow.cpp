#include "MainWindow.h"
#include "./ui_MainWindow.h"

struct Provice {
  QString name;
  QStringList cities;
};

struct Country {
  QString name;
  QList<Provice> provices;
};

// clang-format off
const Country g_countries[] = {
    {"中国", {
         {"北京"},
         {"山西", {"太原", "运城"}},
         {"浙江", {"杭州", "义乌"}}
     }},
    {"美国", {
         {"纽约州", {"纽约", "布法罗"}},
         {"加利福尼亚州", {"洛杉矶", "圣地亚哥", "圣何塞"}}
     }}
};
// clang-format on

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  model = new QStandardItemModel;
  QStandardItem *item, *item2, *item3;
  const Country *country;
  const Provice *provice;
  uint32_t i, j, k;

  size_t count = sizeof(g_countries) / sizeof(Country);
  for (i = 0; i < count; i++) {
    country = &g_countries[i];
    item = new QStandardItem(country->name);
    for (j = 0; j < country->provices.size(); j++) {
      provice = &g_countries[i].provices[j];
      item2 = new QStandardItem(provice->name);

      for (k = 0; k < provice->cities.size(); k++) {
        item3 = new QStandardItem(provice->cities[k]);
        item2->appendRow(item3);
      }

      item->appendRow(item2);
    }
    model->appendRow(item);
  }
  ui->treeView->setModel(model);
  ui->treeView->expandAll();

  ui->label->setText("未选中任何项");

  connect(ui->treeView->selectionModel(),
          &QItemSelectionModel::selectionChanged, this,
          &MainWindow::handleSelectionChanged);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleSelectionChanged(const QItemSelection &selected,
                                        const QItemSelection &deselected) {
  if (selected.indexes().isEmpty()) {
  } else {
    QModelIndex root = ui->treeView->rootIndex();
    QModelIndex index = selected.indexes().first();
    QStringList str_list;
    while (index != root) {
      str_list.prepend(model->data(index).toString());
      index = index.parent();
    }
    //    int row = index.row();
    ui->label->setText(tr("选中了: %1").arg(str_list.join('.')));
  }
}
