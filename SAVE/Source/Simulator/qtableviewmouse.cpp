#include "qtableviewmouse.h"
#include <QMouseEvent>

QTableViewMouse::QTableViewMouse( QWidget * parent ) :
               QTableView(parent)
  {
  setContextMenuPolicy(Qt::CustomContextMenu);
  }

void QTableViewMouse::mousePressEvent( QMouseEvent * event)
  {
  QModelIndex I = indexAt(event->pos());
  if ( event->button() == Qt::LeftButton )
    {
    int row = I.row();
    pDraw->nSelected = row;
    }
  if ( event->button() == Qt::RightButton )
    {
    int row = I.row();
    pDraw->nSelected = row;
    pDraw->OnRbuttonDown( event );
    }
  }
