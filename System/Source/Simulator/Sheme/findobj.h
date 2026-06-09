#ifndef FINDOBJ_H
#define FINDOBJ_H

#include <QDialog>
#include <QTableView>
#include "BaseModel.h"

class QTableViewM : public QTableView
{
  Q_OBJECT
  public:
  QTableViewM( QWidget * parent );
  void mousePressEvent( QMouseEvent * event);
  signals:
  void Show_Sheme();
  void Show_Params();
};

namespace Ui {
class FindObj;
}

struct FindData
{
    IBaseModel* pObj;
    const char * Shema;
};

class FindObjModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    class FindObj * pDial;
		CMem<FindData,32,64> List;
    explicit FindObjModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool Add ( IBaseModel* pObj );
};

class FindObj : public QDialog
{
    Q_OBJECT

public:
    FindObjModel Model;
 //
    explicit FindObj(QWidget *parent = nullptr);
    ~FindObj();
    void SetList ( char *szFilter);
    bool FiltrName( char * szFilter, char * Name );

private slots:
    void on_Find_clicked();

    void on_OK_clicked();

    void on_OnSheme_checkStateChanged(const Qt::CheckState &arg1);

    void on_Modeles_checkStateChanged(const Qt::CheckState &arg1);
    void Show_Sheme();
    void Show_Params();
private:
    Ui::FindObj *ui;
signals:
    void ShowSheme  ( const char * Sheme,  const char * Selected  );
};

#endif // FINDOBJ_H
