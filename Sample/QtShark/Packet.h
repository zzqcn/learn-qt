#ifndef __PACKET_H__
#define __PACKET_H__

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

std::string ip_to_string(uint32_t ip);
std::string port_to_string(uint16_t port);
std::string proto_to_string(uint8_t proto);

uint16_t ntoh2(uint16_t val);
uint32_t ntoh4(uint32_t val);

struct Packet {
  uint32_t index;
  uint32_t srcIP;
  uint32_t dstIP;
  uint16_t srcPort;
  uint16_t dstPort;
  uint8_t proto;
  const uint8_t *data;
  size_t dataLen;
  std::string info;
};

struct ProtoField {
  std::string key, value;
  size_t pos, length;
};

struct ProtoTree {
  ProtoTree(ProtoField *field = nullptr, ProtoTree* parent = nullptr) :
  parent(parent), field(field) {}
  ~ProtoTree();

  void addChild(ProtoTree *child);

  ProtoTree* child(int row) {
      if(row<0 || row>= childs.size())
          return nullptr;
      return childs.at(row);
  }
  int childCount() const {
      return childs.size();
  }
  int columnCount() const {return 1;}
  int row() const {
      if(parent) {
        std::vector<ProtoTree*>::iterator itor = std::find(parent->childs.begin(),
        parent->childs.end(), this);
        if(itor != parent->childs.end())
            return std::distance(parent->childs.begin(), itor);
      }
      return 0;
  }

  ProtoTree* parent;
  std::vector<ProtoTree *> childs;
  ProtoField *field;
};

int dissectPacket(const uint8_t *data, size_t dataLen, Packet *pkt,
                  ProtoTree *tree);

#endif
