#include "msglist.h"
#include "ui_msglist.h"

MsgList::MsgList(QWidget *parent)
  : QDialog(parent)
  , ui(new Ui::MsgList)
{
  ui->setupUi(this);
  ui->List->setStyleSheet("QListView { background-color: #f0f0f0; }");
  ui->List->setModel( &Model );
}

MsgList::~MsgList()
{
  delete ui;
}

void MsgList::Add ( const char * Txt )
{
	if ( Model.Msg.L >= 1000 )
		{
		if ( Model.Msg.L == 1000 )
			{
			CStr & S = Model.Msg.push_back();
			S = "Слишком много сообщений";
			}
		return;
		}
  CStr & S = Model.Msg.push_back();
  S = Txt;
  int row = Model.columnCount();
  Model.insertRows(row, 1);
  QModelIndex index = Model.index(row, 0 );
  Model.setData(index, Txt, Qt::DisplayRole );
  }

MsgListModel::MsgListModel( QObject *parent )
{
}


int MsgListModel::rowCount(const QModelIndex &parent) const
{
  return 100;//Msg.L;
}

int MsgListModel::columnCount(const QModelIndex &parent) const
{
  return 1;
}


// Qt::ItemFlags MsgListModel::flags(const QModelIndex &index) const
// {
//   int r = index.row();
//   int c = index.column();
//   if ( c == 1 )
//     return QAbstractTableModel::flags(index)|Qt::ItemIsUserCheckable;
//   return QAbstractTableModel::flags(index);
// };

bool MsgListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  // int r = index.row();
  // if ( r >= kVars )
  //   return false;
//  if (role == Qt::EditRole)
//    {
//    return true;
//    }
  return false;
}

QVariant MsgListModel::data(const QModelIndex &index, int role) const
  {
  int r = index.row();
  if ( r >= Msg.L )
    return QVariant();
  switch (role)
    {
    case Qt::DisplayRole:
      {
      CStr * Txt = Msg.Get(r);
      if ( Txt->Str[0] == '#' )
          return QString( Txt->Str + 1 );
      return QString( Txt->Str );
      }
    case Qt::EditRole:
    break;
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
        CStr * Txt = Msg.Get( r );
        if ( Txt->Str && Txt->Str[0] != '#' )
            return QVariant(QBrush(Qt::red));
        return QVariant(QBrush(Qt::black));
     }
    break;
    case Qt::BackgroundRole:
      {
      CStr * Txt = Msg.Get( r );
      if ( Txt->Str && Txt->Str[0] != '#' )
        return QVariant(RGB ( 255, 0, 0 ));
      return QVariant(RGB ( 192, 220, 192 ));
      }
    break;
    case Qt::TextAlignmentRole:
    break;
    case Qt::UserRole:
    break;
    }
  return QVariant();
}

