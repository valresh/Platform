#include "qlistliewmouse.h"
#include <QMouseEvent>

QlistLiewMouse::QlistLiewMouse( QWidget * parent ) : QListView ( parent )
{
  setContextMenuPolicy(Qt::CustomContextMenu);
}

void QlistLiewMouse::mousePressEvent( QMouseEvent * event)
{
  QModelIndex I = indexAt(event->pos());
  if ( event->button() == Qt::LeftButton )
  {
    int row = I.row();
//    pDraw->nSelected = row;
  }
  if ( event->button() == Qt::RightButton )
  {
    int row = I.row();
//    pDraw->nSelected = row;
//    pDraw->OnRbuttonDown( event );
  }
}
