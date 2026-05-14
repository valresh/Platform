#ifndef LIST_H
#define LIST_H

#include <QDialog>
#include <QListView>
#include "clistview.h"

#define MAX_VAR_IN_LIST 64
#define MAX_VAR_NAME 64

class ListModel : public QAbstractTableModel
  {
    Q_OBJECT
  public:
    char Vars[MAX_VAR_IN_LIST][MAX_VAR_NAME];
    int kVars;
    bool IsData;
    explicit ListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
  };

 namespace Ui {
 class List;
 }

class List : public QDialog
{
    Q_OBJECT

  public:
    explicit List(QWidget *parent = nullptr);
    ~List();
  ListModel Model;
  int Selected;
  void Init( const char * Title );
  bool Add ( const char * Item );
  void Show();
private slots:
  void on_OK_clicked();

  void on_Cancel_clicked();

  void on_Tab_doubleClicked(const QModelIndex &index);

private:
    Ui::List * ui;
};

#endif // LIST_H
