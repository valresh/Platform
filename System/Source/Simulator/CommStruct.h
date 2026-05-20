#ifndef COMMSTRUCT_H
#define COMMSTRUCT_H
#include "CommProc.h"

#include <QSlider>
#include <QLineEdit>
class WSlider : public QSlider
{
  public:
  WSlider(QWidget*parent):QSlider(parent)
    {}
  NEW
};

class WLineEdit : public QLineEdit
{
  public:
  WLineEdit(QWidget*parent):QLineEdit(parent)
  {}
  NEW
};

#endif // COMMSTRUCT_H
