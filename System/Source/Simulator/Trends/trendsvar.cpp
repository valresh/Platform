#include "trendsvar.h"
#include "ui_trendsvar.h"
#include "showtrends.h"
#include "Err.h"
#include <QTime>
#include <QStandardItemModel>
#include "CommProc.h"
#include <QHeaderView>
#include "showtrends.h"
#include "mainwindow.h"

TrendsVar::TrendsVar(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::TrendsVar)
{
  setWindowIcon( QIcon(":/webcam.png"));
  ui->setupUi(this);
  ui->Wnd->pDraw = pDraw;
  Model.IsData = false;
  QHeaderView * headerV = ui->Wnd->verticalHeader();
  headerV->setVisible(false);
  QHeaderView * headerH = ui->Wnd->horizontalHeader();
  headerH->setStretchLastSection(true);
  ui->Wnd->setModel( &Model );
  ui->Wnd->setEditTriggers(QAbstractItemView::DoubleClicked);
  }

void TrendsVar::Recv ( ShowTrends * _pDraw )
  {
  pDraw = _pDraw;
  ui->Wnd->Recv(pDraw );
  }

void TrendsVar::show(TrendVar * _pVars, int _kVars)
{
  Model.IsData = false;
  Model.kVars = _kVars;
  Model.pVars = _pVars;
  // int k = Model.rowCount();
  // if ( Model.kVars > k )
  //   {
  //   Model.insertRows( k, Model.kVars - k );
  //   }
  // if ( Model.kVars < k )
  // {
  //   Model.removeRows( 0, k - Model.kVars );
  // }
  Model.IsData = true;
//  Model.timerHit();
  QMainWindow::show();
}

TrendsVar::~TrendsVar()
{
  delete ui;
}

bool TrendsVarModel::NoRefresh = false;

TrendsVarModel::TrendsVarModel(QObject *parent)
{
  pTrends = NULL;
  IsData = false;
  //timer = new QTimer(this);
  //timer->setInterval(1000);
  //connect(timer, &QTimer::timeout , this, &TrendsVarModel::timerHit);
  //timer->start();
  pVars = NULL;
  kVars = 0;
  NoRefresh = false;
}

void TrendsVarModel::timerHit()
  {
  if ( !IsData )
    return;
  if ( NoRefresh )
    return;
  QModelIndex topLeft = createIndex(0,0);
  QModelIndex bottomRight = createIndex(kVars_old-1,1);
  kVars_old = kVars;
  emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
  }

int TrendsVarModel::rowCount(const QModelIndex &parent) const
  {
  return 100;//kVars;
  }

int TrendsVarModel::columnCount(const QModelIndex &parent) const
  {
  return 2;
  }

QVariant TrendsVarModel::headerData(int section, Qt::Orientation orientation, int role) const
  {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString("Ссылка");
      case 1:
        return QString("Значение");
    }
  }
  return QVariant();
}

Qt::ItemFlags TrendsVarModel::flags(const QModelIndex &index) const
{
  int r = index.row();
  int c = index.column();
  if ( c == 1 )
    return QAbstractTableModel::flags(index)|Qt::ItemIsUserCheckable;
  return QAbstractTableModel::flags(index);
};

bool TrendsVarModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  int r = index.row();
  if ( r >= kVars )
    return false;
  int c = index.column();
  if ( c != 1 )
    return false;
  // if (role == Qt::EditRole)
  //   {
  //   QString Txt = value.toString();
  //   pParams[r].SetValue ( (char*)Txt.toStdString().c_str());
  //   NoRefresh = false;
  //   return true;
  //   }
  if (role == Qt::CheckStateRole)
    {
    //if ( value.toInt() == Qt::Checked )
    pTrends->SetLimits( r );
    return true;
    }
    // Update your internal data structure with the new state
  return false;
  }

QVariant TrendsVarModel::data(const QModelIndex &index, int role) const
  {
  int r = index.row();
  if ( r >= kVars )
    return QVariant();
  int c = index.column();
  switch (role)
    {
    case Qt::DisplayRole:
      if ( c == 0 )
        {
        return QString( pVars[r].Name );
        }
      char Txt[256];
      pVars[r].Out(Txt);
      return QString ( Txt );
    case Qt::EditRole:
      NoRefresh = true;
      if ( c == 1 )
        {
        KKK();
        }
      break;
      //   return QTime::currentTime().toString();
    case Qt::FontRole:
      // if (r == 0 && c == 0)
      //   { // change font only for cell(0,0)
      //   QFont boldFont;
      //   boldFont.setBold(true);
      //   return boldFont;
      // }
      break;
    case Qt::ForegroundRole:
      {
//       return pVars[r].color;
      }
      break;
    case Qt::BackgroundRole:
      return pVars[r].color;
      break;
    case Qt::TextAlignmentRole:
      //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
      //        return int(Qt::AlignRight | Qt::AlignVCenter);
      break;
    case Qt::CheckStateRole:
      if ( c == 1 && pVars[r].Type == 'D' )
        {
        double V = *(double*)pVars[r].Addr;
        if ( V > pVars[r].Max || V < pVars[r].Min)
          return Qt::Checked;
        else
          return Qt::Unchecked;
        }
      break;
    case Qt::UserRole:
      {
        qlonglong Var = (qlonglong)&pVars[r];
        return QVariant( Var );
      }
      break;
  }
  return QVariant();
}


// #include <QMenu>


// #include <QContextMenuEvent>

// void ParamsList::contextMenuEvent(QContextMenuEvent *e)
// {
//   QModelIndex index = indexAt(e->pos());
//   if ( !index.isValid())
//     return;
//   qlonglong pp = index.data( Qt::UserRole).toLongLong();
//   CParams * pParam = (CParams *)pp;
//   QMenu menu;
//   QAction * actions[5];
//   actions[0] = menu.addAction("Ссылка");
//   actions[1] = menu.addAction("В тренд");
//   actions[2] = menu.addAction("В минитренд");
//   // Получаем индекс ячейки под курсором
//   QAction* selectedAction = menu.exec(viewport()->mapToGlobal(e->pos()));
//   int N = -1;
//   for ( int n = 0; n < 3; n++ )
//   {
//     if (selectedAction == actions[n])
//     {
//       N = n;
//       break;
//     }
//   }

//   switch ( N )
//   {
//     case 0:
//       {

//       }
//       break;
//     case 1:
//       {

//       }
//       break;
//     case 2:
//       {
//         char Name[256];
//         if ( pParam->pModel )
//         {
//           Sprintf( Name, "%s.%s", pParam->pModel->ObjName.Str, pParam->ParamName );
//           int ID =  Trends.Add( Name, pParam->Type, pParam->Addr );
//           minitrend * pTrend = new minitrend( NULL );
//           pTrend->AddVar( ID );
//           pTrend->show();
//         }
//         KKK();
//       }
//       break;
//   }
// }

bool TrendVar::Out(char Txt[256])
  {
  return OutDI( Addr, Type, Txt );
  }

void TrendsVar::on_Wnd_clicked(const QModelIndex &index)
  {
    QMenu menu;
    QAction * actions[5];
    actions[0] = menu.addAction("Показать параметры");
    actions[1] = menu.addAction("Найти на схкмах");
    QAction * selectedAction = menu.exec( QCursor::pos());
    QModelIndex I = ui->Wnd->indexAt(QCursor::pos());
    int r = I.row();
    if ( selectedAction == actions[0])
      {
      pDraw->VarProp.Dial( &pDraw->Vars[r] );
      return;
      }
    if ( selectedAction == actions[1])
      {
       pMainWnd->Find.Find  ( pDraw->Vars[r].Name );
      return;
      }
  }


void TrendsVar::on_Wnd_doubleClicked(const QModelIndex &index)
  {

  }


void TrendsVar::on_Wnd_viewportEntered()
  {

  }

