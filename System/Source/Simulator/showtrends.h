#ifndef SHOWTRENDS_H
#define SHOWTRENDS_H

#include <QWidget>
#include <QPen>
#include "crossplatform.h"
#include "CommProc.h"
#include <QSlider>
#include <QLineEdit>
#include "trendvarprop.h"

#define MAX_VAR 64

struct TrendVar
{
    CStr Name;
    double Min, Max, Nom;
    int Pres;
    QColor color;
    void * Addr;
    int Trend_ID;
    bool UseNom;
    char Type;
    bool Out( char Txt[256]);
};

class ShowTrends : public QWidget
{
    Q_OBJECT
  public:
    TrendVar Vars[MAX_VAR];
    int kVar;
    TrendVarProp VarProp;
    QPen Pen;
    int SetShift;
    int Shift;
    int StartPos;
    int Steps;
    int nWidth;
    int nHeight;
    double mTime; // x = Time * mTime
    double Mtime;
    bool Pause;
    bool ShowSteps;
    QSlider * mT;
    QLineEdit * step;
    QPoint pntMouse;
    QPoint pntDrug;
    int nSelected;
    bool PressMouse;
    bool Drug;
//
    explicit ShowTrends(QWidget *parent = nullptr);
    void paintEvent( QPaintEvent *event );
    void SetLimits( int nVar );
    void OnRbuttonDown(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    signals:
};

#endif // SHOWTRENDS_H
