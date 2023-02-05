#include "Packet.h"

#include <sstream>

using namespace std;

#define IP1(addr) (unsigned)((unsigned char *)&addr)[0]
#define IP2(addr) (unsigned)((unsigned char *)&addr)[1]
#define IP3(addr) (unsigned)((unsigned char *)&addr)[2]
#define IP4(addr) (unsigned)((unsigned char *)&addr)[3]

uint16_t ntoh2(uint16_t val) {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
  return ((((val)&0x00ff) << 8) | (((val)&0xff00) >> 8));
#else
  return val;
#endif
}

uint32_t ntoh4(uint32_t val) {
#if Q_BYTE_ORDER == Q_LITTLE_ENDIAN
  return ((((val)&0x000000ff) << 24) | (((val)&0x0000ff00) << 8) |
          (((val)&0x00ff0000) >> 8) | (((val)&0xff000000) >> 24));
#else
  return val;
#endif
}

string ip_to_string(uint32_t ip) {
  stringstream ss;
  ss << IP1(ip) << "." << IP2(ip) << "." << IP3(ip) << "." << IP4(ip);
  return ss.str();
}

string port_to_string(uint16_t port) { return to_string(ntoh2(port)); }

string proto_to_string(uint8_t proto) {
  switch (proto) {
  case 6:
    return "TCP";
  case 13:
    return "UDP";
  default:
    return "Unknown";
  }
}

ProtoTree::~ProtoTree() {
  size_t i, count;

  if (field != nullptr)
    delete field;

  count = childs.size();
  for (i = 0; i < count; i++) {
    delete childs[i];
  }
}

void ProtoTree::addChild(ProtoTree *child) { childs.push_back(child); }

int dissectPacket(const uint8_t *data, size_t dataLen, Packet *pkt,
                  ProtoTree *tree) {
  int ret = 0;
  ProtoTree *child, *child2;
  ProtoField *field;
  size_t off = 0;

  pkt->data = data;
  pkt->dataLen = dataLen;

  // l2
  field = new ProtoField;
  field->key = "L2";
  field->value = "Ethernet";
  field->pos = off;
  field->length = 14;
  child = new ProtoTree(field, tree);
  tree->addChild(child);
  off += 14;

  // l3
  field = new ProtoField;
  field->key = "L3";
  field->value = "IPv4";
  field->pos = off;
  field->length = 20;
  child = new ProtoTree(field, tree);
  // source ip
  pkt->srcIP = *(uint32_t *)(data + off + 12);
  field = new ProtoField;
  field->key = "srcIP";
  field->value = ip_to_string(pkt->srcIP);
  field->pos = off + 12;
  field->length = 4;
  child2 = new ProtoTree(field, child);
  child->addChild(child2);
  // destination ip
  pkt->dstIP = *(uint32_t *)(data + off + 16);
  field = new ProtoField;
  field->key = "srcIP";
  field->value = ip_to_string(pkt->dstIP);
  field->pos = off + 16;
  field->length = 4;
  child2 = new ProtoTree(field, child);
  child->addChild(child2);
  tree->addChild(child);
  pkt->proto = *(data + off + 9);
  off += 20;

  // l4
  field = new ProtoField;
  field->key = "L4";
  field->value = proto_to_string(pkt->proto);
  field->pos = off;
  field->length = 20;
  child = new ProtoTree(field, tree);
  // source port
  pkt->srcPort = *(uint16_t *)(data + off);
  field = new ProtoField;
  field->key = "srcPort";
  field->value = port_to_string(pkt->srcPort);
  field->pos = off;
  field->length = 2;
  child2 = new ProtoTree(field, child);
  child->addChild(child2);
  // destination port
  pkt->dstPort = *(uint16_t *)(data + off + 2);
  field = new ProtoField;
  field->key = "dstPort";
  field->value = port_to_string(pkt->dstPort);
  field->pos = off + 2;
  field->length = 2;
  child2 = new ProtoTree(field, child);
  child->addChild(child2);
  // tcp flags
  uint16_t flags = *(uint16_t *)(data + off + 12);
  if (flags & 0x0200)
    pkt->info += "SYN ";
  if (flags & 0x1000)
    pkt->info += "ACK ";
  tree->addChild(child);

  return ret;
}
