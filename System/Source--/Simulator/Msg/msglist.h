#ifndef MSGLIST_H
#define MSGLIST_H

#include <QDialog>
#include <QStandardItemModel>
#include "CommProc.h"

class MsgListModel : public  QAbstractTableModel //QAbstractListModel
  {
    Q_OBJECT
  public:
//    struct TrendVar * pVars;
    CMem<CStr,32,256> Msg;
    explicit MsgListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
//    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
};

namespace Ui {
class MsgList;
}

class MsgList : public QDialog
{
    Q_OBJECT

  public:
    MsgListModel Model;
    explicit MsgList(QWidget *parent = nullptr);
    ~MsgList();
    void Add ( const char * Txt );
  private:
    Ui::MsgList *ui;
};

#endif // MSGLIST_H
