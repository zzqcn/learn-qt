#ifndef __PACKET_H__
#define __PACKET_H__

#include <cstdint>
#include <string>
#include <vector>

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
  size_t beginOff, endOff;
};

struct ProtoTree {
  ProtoTree(ProtoField *field = nullptr) { this->field = field; }
  ~ProtoTree();

  void addChild(ProtoTree *child);

  std::vector<ProtoTree *> childs;
  ProtoField *field;
};

int dissectPacket(const uint8_t *data, size_t dataLen, Packet *pkt,
                  ProtoTree *tree);

#endif
