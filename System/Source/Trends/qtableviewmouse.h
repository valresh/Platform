#ifndef QTABLEVIEWMOUSE_H
#define QTABLEVIEWMOUSE_H

#include <QObject>
#include <QTableView>
#include <QWidget>
#include "showtrends.h"

class QTableViewMouse : public QTableView
{
    Q_OBJECT
  public:
    ShowTrends * pDraw;
    QTableViewMouse( QWidget * parent );
    void Recv ( ShowTrends * _pDraw ){ pDraw = _pDraw; };
    void mousePressEvent( QMouseEvent * event);
};

#endif // QTABLEVIEWMOUSE_H
