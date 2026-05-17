#ifndef DIALCOLOR_H
#define DIALCOLOR_H

#include <QWidget>
#include <QPaintEvent>

#ifdef UTILS_EXPORTS
#define INDLL Q_DECL_EXPORT
#else
#define INDLL Q_DECL_IMPORT
#endif

class DialColor : public QWidget
{
    Q_OBJECT
  public:
    QColor color;
    explicit DialColor( QWidget *parent = nullptr);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
  signals:
};

#endif // DIALCOLOR_H
