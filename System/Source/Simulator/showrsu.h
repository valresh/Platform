#ifndef SHOWRSU_H
#define SHOWRSU_H
#include "Param.h"
#include "qheaderview.h"

#include <QTableView>
#include <QTimer>
#define CParams QParams

#include <QDialog>

namespace Ui {
class ShowRSU;
}

#include <QAbstractTableModel>

class ObjListModel : public QAbstractTableModel
{
public:
    Q_OBJECT
public:
    static bool NoRefresh;
		CMem<RSU_Obj*,256,256>Select_Obj;
		explicit ObjListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void Refresh();
};

struct ObjListTab : public QTableView
{
public:
    Q_OBJECT
public:
    ObjListModel Model;
    ObjListTab(QWidget * Parent ) : QTableView ( Parent )
        {
        QHeaderView * headerV =  verticalHeader();
        headerV->setVisible(false);
				QHeaderView * headerH =  horizontalHeader();//->setSectionResizeMode(QHeaderView::Interactive );
				connect( headerH, &QHeaderView::sectionResized, this, &ObjListTab::sectionResized);
				headerH->setStretchLastSection(true);
//				resizeColumnToContents( 0 );
        setModel( &Model );
        }
    void Refresh()
        {
				Model.Refresh();
        }
  void mousePressEvent(QMouseEvent* event) override;
 //   void show(CParams * pParams, int kParams);
  signals:
  void ShowObject ( LPCSTR ObjName, LPCSTR Model, void * pBase );
	private slots:
	void sectionResized( int logicalIndex, int oldSize, int newSize);
};

class List_W_Model : public QAbstractTableModel
{
public:
    Q_OBJECT
public:
    bool IsData;
    static bool NoRefresh;
    QTimer * timer;
    CMem<CParams, 16, 16>Params;
    explicit List_W_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void timerHit();
    void Refresh();
};

struct List_W_Tab : public QTableView
{
public:
    Q_OBJECT
public:
    CParams * pParams;
    int kParams;
    List_W_Model Model;
    List_W_Tab(QWidget * Parent ) : QTableView ( Parent )
    {
        pParams = NULL;
        kParams = 0;
        QHeaderView * headerV =  verticalHeader();
        headerV->setVisible(false);
				QHeaderView * headerH =  horizontalHeader();//->setSectionResizeMode(QHeaderView::Interactive );
				connect( headerH, &QHeaderView::sectionResized, this, &List_W_Tab::sectionResized);
				headerH->setStretchLastSection(true);
				setModel( &Model );
        setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    void Refresh()
    {
        Model.Refresh();
    }
private slots:
		void sectionResized( int logicalIndex, int oldSize, int newSize)
		{
		resizeRowsToContents();
		}
//    void contextMenuEvent(QContextMenuEvent *e);
//   void show(CParams * pParams, int kParams);
};

class List_P_Model : public QAbstractTableModel
{
public:
    Q_OBJECT
public:
    bool IsData;
    static bool NoRefresh;
    QTimer * timer;
    CMem<CParams, 16, 16>Params;
    explicit List_P_Model(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    void timerHit();
    void Refresh();
};

struct List_P_Tab : public QTableView
{
public:
    Q_OBJECT
public:
    CParams * pParams;
    int kParams;
    List_P_Model Model;
    List_P_Tab(QWidget * Parent ) : QTableView ( Parent )
    {
        pParams = NULL;
        kParams = 0;
        QHeaderView * headerV =  verticalHeader();
        headerV->setVisible(false);
				QHeaderView * headerH =  horizontalHeader();
				headerH->setStretchLastSection(true);
				connect( headerH, &QHeaderView::sectionResized, this, &List_P_Tab::sectionResized);
				setModel( &Model );
        setEditTriggers(QAbstractItemView::DoubleClicked);
    }
    void Refresh()
    {
        Model.Refresh();
    }
//    void contextMenuEvent(QContextMenuEvent *e);
 //   void show(CParams * pParams, int kParams);
private slots:
		void sectionResized( int logicalIndex, int oldSize, int newSize)
		{
			resizeRowsToContents();
		}
};

#include <QTableWidgetItem>

struct ConnInfo
{
    char Type; //'I' 'O' -jn RSU
    const char * RSU;
    const char * Model;
    const char * Descr;
};

class ShowRSU : public QDialog
{
    Q_OBJECT

public:
    CMem<ConnInfo,256,256> * pConn_Info;
    explicit ShowRSU(QWidget *parent = nullptr);
    ~ShowRSU();
    bool WasInit;
    void Init();
    void Go();
    void Refresh();
private slots:
    void on_Find_clicked();
		void ShowObject ( LPCSTR ObjName, LPCSTR Model );
    void SetRSURef( );
private:
    Ui::ShowRSU *ui;
};

#endif // SHOWRSU_H
