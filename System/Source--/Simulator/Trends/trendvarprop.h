#ifndef TRENDVARPROP_H
#define TRENDVARPROP_H

#include <QDialog>
//#include "showtrends.h"
#include "dialcolor.h"
#include "trendsvar.h"
#include <QAbstractButton>

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
    TrendVar * pVar;
private slots:
    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::TrendVarProp *ui;
};

#endif // TRENDVARPROP_H
