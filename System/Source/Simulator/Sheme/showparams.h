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

struct ShowParams_W
{
  QRect WinRect;
  int w_split[2];
  int cx_0;
  void Init_W( const char * Name );
  void Save_W( const char * Name );
};

class ShowParams : public QMainWindow, public ShowParams_W
{
    Q_OBJECT

  public:
    explicit ShowParams(QWidget *parent, IBaseModel * pObj );
    ~ShowParams();
    IBaseModel * pObj;
    CParams * pParams;
    int kParams;
    CStr Name;
//    ParamsListModel ParamsModel;
    QSplitter *splitter;
    int Sign;
    void on_Change( double Delta );
    void resizeEvent(QResizeEvent *event);
    void moveEvent(QMoveEvent *event);
    void SaveToDB();
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

private slots:
    void on_Step1_clicked();

    void on_splitter_splitterMoved(int pos, int index);

    void on_ParamTree_collapsed(const QModelIndex &index);

    void on_ParamTree_expanded(const QModelIndex &index);

private:
    Ui::ShowParams *ui;
};

#endif // SHOWPARAMS_H
