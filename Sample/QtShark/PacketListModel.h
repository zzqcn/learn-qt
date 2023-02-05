#ifndef __PACKET_LIST_MODEL_H__
#define __PACKET_LIST_MODEL_H__

#include <QAbstractItemModel>

#include "Packet.h"

enum PacketListColumnType {
  Index,
  Timestamp,
  SrcAddr,
  DstAddr,
  SrcPort,
  DstPort,
  Protocol,
  Length,
  Info,
  Max,
};

struct PacketListColumnInfo {
  PacketListColumnType type;
  QString title;
  // int width;
};

struct PacketListPrefs {
  QList<PacketListColumnInfo> colInfos;
};

class PacketListModel : public QAbstractItemModel {
  Q_OBJECT

public:
  explicit PacketListModel(QObject *parent = 0);
  ~PacketListModel();

  Qt::ItemFlags flags(const QModelIndex &index) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

  Packet *packet(int idx) const {
    if (idx >= 0 && idx < pkts.size())
      return pkts[idx];
    else
      return 0;
  }

  Packet *firstPacket(void) const {
    if (pkts.empty())
      return 0;
    else
      return pkts.front();
  }

  Packet *lastPacket(void) const {
    if (pkts.empty())
      return 0;
    else
      return pkts.back();
  }

  int addPacket(Packet *pkt);

private:
  QVariant packetColumnData(Packet *pkt, int col) const;

public slots:

private:
  QList<Packet *> pkts;
  PacketListPrefs *prefs;
};

#endif
