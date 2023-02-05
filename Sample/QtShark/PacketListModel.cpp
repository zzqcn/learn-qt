#include <QDebug>

#include "PacketListModel.h"

static PacketListPrefs *generatePacketListPrefs(void) {
  PacketListPrefs *prefs = new PacketListPrefs;
  QList<PacketListColumnInfo> &list = prefs->colInfos;

  // Index
  list.append({PacketListColumnType::Index, "No."});
  // Timestamp,
  // list.append({PacketListColumnType::Timestamp, "Time"});
  // SrcAddr,
  list.append({PacketListColumnType::SrcAddr, "SrcAddr"});
  // DstAddr,
  list.append({PacketListColumnType::DstAddr, "DstAddr"});
  // SrcPort,
  list.append({PacketListColumnType::SrcPort, "SrcPort"});
  // DstPort,
  list.append({PacketListColumnType::DstPort, "DstPort"});
  // Protocol,
  list.append({PacketListColumnType::Protocol, "Protocol"});
  // Length,
  list.append({PacketListColumnType::Length, "Length"});
  // Info,
  list.append({PacketListColumnType::Info, "Info"});

  return prefs;
}

PacketListModel::PacketListModel(QObject *parent) : QAbstractItemModel(parent) {
  prefs = generatePacketListPrefs();
}

PacketListModel::~PacketListModel() {
  // 不要试图在这里m_pkts->clear()
  if (prefs != nullptr)
    delete prefs;
}

int PacketListModel::addPacket(Packet *pkt) {
  this->beginInsertRows(QModelIndex(), pkts.size(), pkts.size() + 1);
  pkts.push_back(pkt);
  this->endInsertRows();

  return 0;
}

Qt::ItemFlags PacketListModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::NoItemFlags;

  return QAbstractItemModel::flags(index);
}

int PacketListModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return pkts.size();
}

int PacketListModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return prefs->colInfos.size();
}

QVariant PacketListModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();
  if (orientation != Qt::Horizontal)
    return QVariant();

  return prefs->colInfos[section].title;
}

QModelIndex PacketListModel::index(int row, int col,
                                   const QModelIndex &parent) const {
  Q_UNUSED(parent);

  if (pkts.empty())
    return QModelIndex();

  if (row >= pkts.size() || col >= prefs->colInfos.size())
    return QModelIndex();

  Packet *pkt = pkts[row];

  return createIndex(row, col, pkt);
}

QModelIndex PacketListModel::parent(const QModelIndex &child) const {
  Q_UNUSED(child);
  return QModelIndex();
}

QVariant PacketListModel::packetColumnData(Packet *pkt, int col) const {
  if (col > prefs->colInfos.size())
    return QVariant();

  PacketListColumnType type = prefs->colInfos[col].type;
  switch (type) {
  case PacketListColumnType::Index:
    return pkt->index + 1;
  // case PacketListColumnType::Timestamp:
  //   break;
  case PacketListColumnType::SrcAddr:
    return QString::fromStdString(ip_to_string(pkt->srcIP));
  case PacketListColumnType::DstAddr:
    return QString::fromStdString(ip_to_string(pkt->dstIP));
  case PacketListColumnType::SrcPort:
    return ntoh2(pkt->srcPort);
  case PacketListColumnType::DstPort:
    return ntoh2(pkt->dstPort);
  case PacketListColumnType::Protocol:
    return QString::fromStdString(proto_to_string(pkt->proto));
  case PacketListColumnType::Length:
    return pkt->dataLen;
  case PacketListColumnType::Info:
    return QString::fromStdString(pkt->info);

  default:
    return QVariant();
  }
}

QVariant PacketListModel::data(const QModelIndex &index, int role) const {
  if (pkts.empty())
    return QVariant();

  // Packet* pkt = static_cast<Packet*>(index.internalPointer());
  // if(!pkt || !pkt->visible_flag)
  //   return QVariant();

  Packet *pkt = pkts.at(index.row());

  switch (role) {
  case Qt::DisplayRole:
    return packetColumnData(pkt, index.column());
    // break;
  case Qt::BackgroundRole:
    // if(pkt->mark_flag & 0x1) {
    //   return QBrush(QColor(0xf4, 0xcc, 0xcc));
    // }
    break;
  default:
    break;
  }

  return QVariant();
}
