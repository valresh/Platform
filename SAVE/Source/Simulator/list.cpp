#include "list.h"
#include "ui_list.h"
#include "CommProc.h"

List::List(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::List)
  {
  ui->setupUi(this);
  setModal(true);
  Model.IsData = false;
//  ui->Tab->setBackgroundRole()
  ui->Tab->setAutoFillBackground( true );
  ui->Tab->setModel( &Model );
  }

List::~List()
  {
  delete ui;
  }

void List::Init( const char * Title )
  {
  setWindowTitle( Title );
  Model.kVars = 0;
  Model.IsData = false;
  }

bool List::Add ( const char * Item )
  {
  if ( Model.kVars >= MAX_VAR_IN_LIST )
      return false;
  StrCpy ( Model.Vars[Model.kVars++], Item );
  }

void List::Show()
  {
  Model.IsData = true;
  exec();
  }

// QModelIndexList List::selectedIndexes()
//   {
//     return QTableView::selectedIndexes();
//   }

/////////////////////////////////////////////////////////////////////

ListModel::ListModel(QObject *parent)
  {
  IsData = false;
  kVars = 0;
  }

  // QModelIndexList Sel = ui->List->selectedIndexes();
  // if ( !Sel.isEmpty())
  // {
  //   foreach (QModelIndex Ind, Sel)
  //   {
  //     QString itemText = Ind.data(Qt::DisplayRole).toString();
  //     emit ShowTrend ( itemText.toStdString().c_str());
  //   }
  // }
  // ui->List->clearSelection();

int ListModel::rowCount(const QModelIndex &parent) const
{
  return 5;//kVars;
}

int ListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}

QVariant ListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  return QVariant();
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
  int r = index.row();
  int c = index.column();
//  if ( c == 1 )
//    return QAbstractTableModel::flags(index)|Qt::ItemIsUserCheckable;
  return QAbstractTableModel::flags(index);
};

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
  {
  return false;
  int r = index.row();
  if ( r >= kVars )
    return false;
  int c = index.column();
  if ( c != 1 )
    return false;
  if (role == Qt::CheckStateRole)
    {
    //if ( value.toInt() == Qt::Checked )
    return true;
    }
  return false;
  }

QVariant ListModel::data(const QModelIndex &index, int role) const
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
        return QString( Vars[r] );
        }
      break;
    case Qt::EditRole:
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
      return QVariant ( RGB(192,220,192 ));
      break;
    case Qt::BackgroundRole:
      return QVariant ( RGB(192,220,192 ));
      break;
    case Qt::TextAlignmentRole:
      //      if (r == 1 && c == 1) // change text alignment only for cell(1,1)
      //        return int(Qt::AlignRight | Qt::AlignVCenter);
    break;
    case Qt::CheckStateRole:
    break;
    case Qt::UserRole:
      {
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


void List::on_OK_clicked()
{
  QModelIndexList Sel = ui->Tab->selectedIndexes();
  if ( !Sel.isEmpty())
  {
    foreach (QModelIndex Ind, Sel)
    {
//      QString itemText = Ind.data(Qt::DisplayRole).toString();
      Selected = Ind.row();
      break;
    }
  }
  ui->Tab->clearSelection();
  accept();//  hide();
}

void List::on_Cancel_clicked()
{
  reject();//hide();
}


void List::on_Tab_doubleClicked(const QModelIndex &index)
{
  on_OK_clicked();

}

