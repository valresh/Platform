#ifndef SHOWPARAMS_H
#define SHOWPARAMS_H
#include "BaseModel.h"
#include <QMainWindow>
#include "Param.h"
#include "paramslist.h"
#include <QSplitter>

namespace Ui {
class ShowParams;
}

class ShowParams : public QMainWindow
{
    Q_OBJECT

  public:
    explicit ShowParams(QWidget *parent, IBaseModel * pObj );
    ~ShowParams();
    IBaseModel * pObj;
    CParams * pParams;
    int kParams;
//    ParamsListModel ParamsModel;
    QSplitter *splitter;
    int Sign;
    void on_Change( double Delta );

  public slots:
    void on_ParamTree_clicked(const QModelIndex &index);

    void on_P_1_clicked();

    void on_M_1_clicked();

    void on_P_01_clicked();

    void on_M_01_clicked();

    void on_P_001_clicked();

    void on_M_001_clicked();

    void on_Sign_clicked();

    void on_bp_checkStateChanged(const Qt::CheckState &arg1);

private:
    Ui::ShowParams *ui;
};

#endif // SHOWPARAMS_H
