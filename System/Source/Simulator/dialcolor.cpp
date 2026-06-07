#include "dialcolor.h"
#include <QColorDialog>
#include <QPainter>
#include "Err.h"

DialColor::DialColor( QWidget *_parent )
	: QWidget{NULL}
{
}

void DialColor::mousePressEvent(QMouseEvent *event)
	{
	QColor Res = QColorDialog::getColor(
						color, this, "Цвет тренда" );
	if (Res.isValid())
	{
		color = Res;
		repaint();
	}
	}

void DialColor::paintEvent(QPaintEvent *event)
	{
	QPainter * P = new QPainter( this );
	QRect rc = rect();
	P->fillRect( rc, color );
	delete P;
	}
