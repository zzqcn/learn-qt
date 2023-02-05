#ifndef __BYTE_VIEW_TAB_H__
#define __BYTE_VIEW_TAB_H__

#include "Packet.h"
#include <QTabWidget>
#include <QTextEdit>

#if 0
enum ByteViewType { BYTES_HEX, BYTES_BITS };

class ByteViewText : public QTextEdit {
  Q_OBJECT

public:
  explicit ByteViewText(QWidget *parent = 0, Packet *pkt = 0);

  void renderBytes();

private:
  void lineCommon(int off);
  int flushBytes(QString &str);

private:
  Packet *m_pkt;
  int m_start, m_end;
};
#endif

////////////////////////////////////////////////////////////

class ByteViewTab : public QTabWidget {
  Q_OBJECT

public:
  explicit ByteViewTab(QWidget *parent = 0);

  void clear();
  void setPacket(Packet *pkt);
  void setRange(int begin, int end);

signals:

public slots:

};

#endif
