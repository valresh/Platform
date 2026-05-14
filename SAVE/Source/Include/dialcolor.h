#ifndef DIALCOLOR_H
#define DIALCOLOR_H

#include <QWidget>
#include <QPaintEvent>

#ifdef UTILS_EXPORTS
#define INDLL Q_DECL_EXPORT
#else
#define INDLL Q_DECL_IMPORT
#endif

class INDLL DialColor : public QWidget
{
    Q_OBJECT
  public:
    QColor color;
    explicit DialColor( QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
  signals:
};

#endif // DIALCOLOR_H
