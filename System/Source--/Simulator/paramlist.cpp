#include "showparams.h"
#include "BaseModel.h"
#include "Param.h"
#include "Err.h"
#include "paramslist.h"
#include <QTime>
#include <QStandardItemModel>
#include "CommProc.h"
#include "minitrend.h"
#include "list.h"
#include "mainwindow.h"
#include "qtrends.h"
#include <QComboBox>


bool ParamsListModel::NoRefresh = false;

ParamsListModel::ParamsListModel(QObject *parent)
{
  IsData = false;
  timer = new QTimer(this);
  timer->setInterval(1000);
  connect(timer, &QTimer::timeout , this, &ParamsListModel::timerHit);
  timer->start();
  pParams = NULL;
  kParams = 0;
  NoRefresh = false;
}

void ParamsListModel::timerHit()
{
  if ( !IsData )
    return;
  if ( NoRefresh )
    return;
  // we identify the top left cell
  QModelIndex topLeft = createIndex(0,0);
  QModelIndex bottomRight = createIndex(kParams_old-1,1);
  kParams_old = kParams;
  emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
  //  beginResetModel();
  // // input_address = data[0]
  // // input_numbers = data[1]
  // // column_count = 2
  // // row_count = len(self.input_numbers)
  //  endResetModel();    // emit a signal to make the view reread identified data  }
}

int ParamsListModel::rowCount(const QModelIndex &parent) const
{
  return 100;//kParams;
}

int ParamsListModel::columnCount(const QModelIndex &parent) const
{
  return 2;
}

QVariant ParamsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
    switch (section) {
      case 0:
        return QString("Параметр");
      case 1:
        return QString("Значение");
    }
  }
  return QVariant();
}

Qt::ItemFlags ParamsListModel::flags(const QModelIndex &index) const
{
  int r = index.row();
  int c = index.column();
  if ( r < kParams && c == 1 )
  {
    if ( pParams[r].Type == 'B' )
      return QAbstractTableModel::flags(index) | Qt::ItemIsUserCheckable;
    if ( pParams[r].ParamName[0] != '#' )
      return QAbstractTableModel::flags(index) | Qt::ItemIsEditable; // Разрешить редактирование
  }
  return QAbstractTableModel::flags(index);
};

bool ParamsListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  int r = index.row();
  if ( r >= kParams )
    return false;
  int c = index.column();
  if ( c != 1 )
    return false;
  if (role == Qt::EditRole)
  {
    QString Txt = value.toString();
    pParams[r].SetValue ( (char*)Txt.toStdString().c_str());
    NoRefresh = false;
    return true;
  }
  if (role == Qt::CheckStateRole)
  {
    if ( pParams[r].Type == 'B' )
    {
      if ( value.toInt() == Qt::Checked )
        *(bool*)pParams[r].Addr = true;
      else
        *(bool*)pParams[r].Addr = false;
      return true;
    }
    // Update your internal data structure with the new state
  }
  return false;
}

QVariant ParamsListModel::data(const QModelIndex &index, int role) const
{
  int r = index.row();
  if ( r >= kParams )
    return QVariant();
  int c = index.column();
  switch (role)
    {
    case Qt::DisplayRole:
      if ( c == 0 )
      {
        char * N = pParams[r].ParamName;
        if ( N[0] == '#' )
          return QString( N + 1 );
        else
          return QString( N );
      }
      char Txt[256];
      if ( pParams[r].pShowParam )
        pParams[r].pShowParam->Show( &pParams[r] );
      pParams[r].Out(Txt);
      return QString ( Txt );
    case Qt::EditRole:
      NoRefresh = true;
      if ( c == 1 )
      {
        KKK();
        //     char Txt[256];
        //     pParams[r].Out(Txt);
        //     return QString ( Txt );
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
        if ( c == 0 && pParams[r].Type == 'L' )
          {
            return QColor( 0, 0, 255 );
          }
        if ( c == 0 && pParams[r].ParamName[0] == '#' )
        {
          return QColor( 255, 0, 0 );
        }
      }
      break;
    case Qt::BackgroundRole:
      // if (r == 1 && c == 1)  // change background only for cell(1,2)
      //   return QBrush(Qt::red);
      break;
    case Qt::TextAlignmentRole:
      //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
      //        return int(Qt::AlignRight | Qt::AlignVCenter);
      break;
    case Qt::CheckStateRole:
      if ( c == 1 && pParams[r].Type == 'B' )
      {
        bool V = *(bool*)pParams[r].Addr;
        if ( V )
          return Qt::Checked;
        else
          return Qt::Unchecked;
      }
      break;
    case Qt::UserRole:
      {
      qlonglong Var = (qlonglong)&pParams[r];
      return QVariant( Var );
      }
      break;
  }
  return QVariant();
}

void ParamsList::show(CParams * _pParams, int _kParams)
{
  Model.IsData = false;
  pParams = _pParams;
  kParams = _kParams;
  Model.pParams = pParams;
  // int k = Model.rowCount();
  // if ( kParams > k )
  //   {
  //   Model.insertRows( k, kParams - k );
  //   }
  // if ( kParams < k )
  // {
  //   Model.removeRows( 0, k - kParams );
  // }
  Model.kParams = kParams;
  Model.IsData = true;
  Model.timerHit();
}

#include <QMenu>

void ParamsList::mousePressEvent( QMouseEvent * e )
{
  QTableView::mousePressEvent( e );
  Qt::MouseEventFlags flags = e->flags();
  if ( flags & Qt::MouseEventCreatedDoubleClick )
    {
    return;
    }
  QModelIndex index = indexAt(e->pos());
  if ( !index.isValid())
    return;
  qlonglong pp = index.data( Qt::UserRole).toLongLong();
  CParams * pParam = (CParams *)pp;
  if ( pParam == NULL )
    return;
  if ( pParam->Type != 'L' )
    return;
  QMenu menu;
  QAction * actions[64];
  char Txt[1024];
  strcpy ( Txt, (char*)pParam->pDefValue );
  char* P = Txt;
  char* Pb = NULL;
  char* Pe = NULL;
  int n = 0;
  int nSel = *(int*)pParam->Addr;
  while ( 1 )
    {
    Pb = P;
    char* Pn = strchr(P, ';');
    Pe = Pn;
    if ( Pn == NULL)
      break;
    *Pe = 0;
    actions[n] = menu.addAction( P );
    if ( n == nSel )
      menu.setActiveAction( actions[n] );
    n++;
    P = Pn + 1;
    }
  QAction * sel = menu.exec( QCursor::pos());
  if ( sel == NULL )
    return;
  for ( int i = 0; i < n; i++ )
    {
      if ( sel == actions[i] )
      {
        *(int*)pParam->Addr = i;
        break;
      }
    }
}

#include <QContextMenuEvent>

void ParamsList::contextMenuEvent(QContextMenuEvent *e)
  {
  resizeRowsToContents();
  QModelIndex index = indexAt(e->pos());
  if ( !index.isValid())
    return;
  qlonglong pp = index.data( Qt::UserRole).toLongLong();
  CParams * pParam = (CParams *)pp;
  QMenu menu;
  QAction * actions[5];
  actions[0] = menu.addAction("Выдать ссылку");
  actions[1] = menu.addAction("Вставить ссылку");
  actions[2] = menu.addAction("В тренд");
  actions[3] = menu.addAction("В минитренд");
  // Получаем индекс ячейки под курсором
  QAction* selectedAction = menu.exec(viewport()->mapToGlobal(e->pos()));
  int N = -1;
  for ( int n = 0; n < 3; n++ )
    {
    if (selectedAction == actions[n])
      {
      N = n;
      break;
      }
    }

  switch ( N )
    {
      case 0:
        {
          Char<256>Txt;
          Txt.Prt( "%s.%s", pParam->pModel->ObjName.Str, pParam->ParamName );
          TxtToClp( Txt.Str );
        }
      break;
      case 1:
        {
          if ( pParam->Type == 'S')
            {
            TxtFromClp( pParam->Len, (char*)pParam->Addr );
            }
        }
        break;
      case 2:
        {
//
        // QString Dir = PROJECT_ROOT;
        // Dir += "INI/Trends/";
        // QDir DirList( Dir );
        // QStringList Filtr ( "*.csv");
        // QFileInfoList L = DirList.entryInfoList( Filtr,
        //                   QDir::NoFilter, QDir::Name);
        // if ( L.isEmpty())
        //   {
        //   SysMsgErr ( "Не найдены файлы трендов" );
        //   break;
        //   }
// загруженные
        WinList * pW = WinList::pFirst;
        int K = 0;
        char * pTrend = NULL;
        while ( pW )
          {
          if ( pW->Type == WinList::Trend )
            {
            K++;
            pTrend = pW->FileName;
            }
          pW = pW->pNext;
          }
        if ( K == 0 )
          {
          SysMsgErr ( "Тренды не загружены" );
          break;
          }
        if ( K > 0 )
          {
          List & S = pMainWnd->Select;
          S.Init("Выбор группы трендов");
          pW = WinList::pFirst;
          while ( pW )
            {
            if ( pW->Type == WinList::Trend )
              S.Add( pW->FileName );
            pW = pW->pNext;
            }
          S.Show();
          int Res = S.result();
          if ( Res == 0 )
            break;
          pTrend = S.Model.Vars[S.Selected];
          pW = WinList::pFirst;
          while ( pW )
            {
            if ( pW->Type == WinList::Trend &&
              strcmp ( pW->FileName, pTrend ) == 0 )
            {
              QTrends * pT = (QTrends*)pW;
              break;
            }
            pW = pW->pNext;
            }
///
          }
        }
      break;
      case 3:
        {
          char Name[256];
          if ( pParam->pModel )
            {
            Sprintf( Name, "%s.%s", pParam->pModel->ObjName.Str, pParam->ParamName );
            int ID =  Trends.Add( Name, pParam->Type, pParam->Addr );
            minitrend * pTrend = new minitrend( NULL );
            pTrend->AddVar( ID );
            pTrend->show();
            }
          KKK();
        }
      break;
    }
  }
