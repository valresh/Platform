#ifndef PARAMSLIST_H
#define PARAMSLIST_H
#include "Param.h"
#include "qheaderview.h"
#include <QItemDelegate>
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
    struct ParamsList * pTable;
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
  Q_OBJECT
public:
  CParams * pParams;
  int kParams;
  class ShowParams * pMain;
  ParamsListModel Model;
  ParamsList(QWidget * Parent );
  void contextMenuEvent(QContextMenuEvent *e);
  void mousePressEvent( QMouseEvent * e );
  void show(CParams * pParams, int kParams);
  private slots:
    void sectionResized( int logicalIndex, int oldSize, int newSize);
  };

#endif // PARAMSLIST_H
