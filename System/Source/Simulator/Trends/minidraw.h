#ifndef MINIDRAW_H
#define MINIDRAW_H

#include <QWidget>
#include <QSlider>
#include <QScrollBar>
#include "minitrend.h"

class minidraw : public QWidget
{
  public:
    minitrend * pMain;
    QPen Pen;
    int Vars[MAX_VAR];
    int kVars;
    int Shift;
    int Steps;
    QColor colVars[MAX_VAR];
    bool New_Pres;
    double Min[MAX_VAR], Max[MAX_VAR];
    double d;
    bool FirstDraw;
    QSlider * mT;
    QSlider * mD;
    int SetShift;
    bool Pause;
    int StartPos;
//
    NEW
    minidraw( QWidget * pMain );
    void paintEvent( QPaintEvent *event );
    void MM( int N, double V );
};

#endif // MINIDRAW_H
