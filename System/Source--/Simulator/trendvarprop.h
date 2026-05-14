#ifndef TRENDVARPROP_H
#define TRENDVARPROP_H

#include <QDialog>
//#include "showtrends.h"
#include "dialcolor.h"
#include "trendsvar.h"

namespace Ui {
class TrendVarProp;
}

class TrendVarProp : public QDialog
{
    Q_OBJECT

  public:
    explicit TrendVarProp(QWidget *parent = nullptr);
    ~TrendVarProp();
    bool Dial( TrendVar * pVar );
  private:
    Ui::TrendVarProp *ui;
};

#endif // TRENDVARPROP_H
