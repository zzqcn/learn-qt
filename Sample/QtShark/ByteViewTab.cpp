#include "ByteViewTab.h"
#include "QHexView.h"
#include <QDebug>
#include <QTabBar>
#include <cctype>

#if 0
ByteViewText::ByteViewText(QWidget *parent, Packet *pkt)
    : QTextEdit(parent), m_pkt(pkt), m_start(-1), m_end(-1) {
  setReadOnly(true);
  setUndoRedoEnabled(false);
  setLineWrapMode(QTextEdit::NoWrap);
  setFontFamily("Courier New");
  renderBytes();
}

void ByteViewText::renderBytes() {
  if (!m_pkt) {
    clear();
    return;
  }

  setUpdatesEnabled(false);
  textCursor().beginEditBlock();
  clear();

  for (uint32_t i = 0; i < m_pkt->dataLen; i += 16) {
    lineCommon(i);
  }
  textCursor().endEditBlock();

  // todo: XXX
  // scrollToByte(xxx);
  textCursor().setPosition(0);
  ensureCursorVisible();

  setUpdatesEnabled(true);
}

void ByteViewText::lineCommon(int off) {
  const char hexchars[16] = {
      '0', '1', '2', '3', '4', '5', '6', '7',
      '8', '9', 'a', 'b', 'c', 'd', 'e', 'f',
  };
  const uint8_t *pd;
  uint32_t i, cur;
  QString str;

  pd = m_pkt->data;

  // Line number
  str += QString("%1  ").arg(off, 4, 16, QChar('0'));

  // Hex data
  for (i = 0, cur = off; i < 16; ++i) {
    //    bool add_space = (i > 0) && (cur % 8);
    //    if (add_space) {
    //      str += ' ';
    //    } else if (i > 0 && cur < m_pkt->data_len)
    //      str += '-';

    if (i > 0) {
      if (i % 8 == 0 && cur < m_pkt->dataLen)
        str += '-';
      else
        str += ' ';
    }

    if (cur < m_pkt->dataLen) {
      // switch(m_format)
      str += hexchars[(pd[cur] & 0xf0) >> 4];
      str += hexchars[pd[cur] & 0x0f];
    } else
      str += "  ";

    cur++;
  }
  str += "   ";

  // ASCII characters
  for (i = 0, cur = off; i < 16; ++i) {
    bool add_space = (i > 0);
    if (add_space) {
      if ((cur % 8) == 0)
        str += ' ';
    }

    if (cur < m_pkt->dataLen)
      str += std::isprint(pd[cur]) ? QChar(pd[cur]) : '.';
    else
      str += ' ';

    cur++;
  }

  if (str.length() > 0)
    flushBytes(str);
  append("");
}

int ByteViewText::flushBytes(QString &str) {
  insertPlainText(str);
  str.clear();
  return str.length();
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////

ByteViewTab::ByteViewTab(QWidget *parent) : QTabWidget(parent) {
  tabBar()->hide();
  setTabPosition(QTabWidget::South);
  setDocumentMode(true);
}

void ByteViewTab::clear() {
  while (currentWidget())
    delete currentWidget();
}

void ByteViewTab::setPacket(Packet *pkt) {
  if (!pkt)
    return;

  clear();

  //  ByteViewText *page = new ByteViewText(this, pkt);
  QHexView *hexView = new QHexView(this);
  QByteArray arr((const char *)pkt->data, pkt->dataLen);
  hexView->setData(new QHexView::DataStorageArray(arr));
  QTabWidget::addTab(hexView, "data");

  setCurrentIndex(0);
}

void ByteViewTab::setRange(int begin, int end) {
  QHexView *hexView = dynamic_cast<QHexView *>(currentWidget());
  hexView->setSelected(begin, end - begin + 1);
}
