#ifndef PARAMSLIST_H
#define PARAMSLIST_H
#include "Param.h"
#include "qheaderview.h"

#include <QTableView>
#include <QTimer>
#define CParams QParams

class ParamsListModel : public QAbstractTableModel
  {
    Q_OBJECT
  public:
    CParams * pParams;
    bool IsData;
    static bool NoRefresh;
    int kParams;
    int kParams_old;
    QTimer * timer;
    explicit ParamsListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void timerHit();
};


 struct ParamsList : public QTableView
    {
    CParams * pParams;
    int kParams;
    ParamsListModel Model;
    ParamsList(QWidget * Parent ) : QTableView ( Parent )
    {
    pParams = NULL;
    kParams = 0;
    QHeaderView * headerV =  verticalHeader();
    headerV->setVisible(false);
    setModel( &Model );
    setEditTriggers(QAbstractItemView::DoubleClicked);
    }
  void contextMenuEvent(QContextMenuEvent *e);
  void show(CParams * pParams, int kParams);
  };

#endif // PARAMSLIST_H
