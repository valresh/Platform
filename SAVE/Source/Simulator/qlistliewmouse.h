#ifndef QLISTLIEWMOUSE_H
#define QLISTLIEWMOUSE_H

#include <QListView>
#include <QObject>

class QlistLiewMouse : public QListView
{
    Q_OBJECT
  public:
    QlistLiewMouse( QWidget * parent );
    void mousePressEvent( QMouseEvent * event);
};

#endif // QLISTLIEWMOUSE_H
