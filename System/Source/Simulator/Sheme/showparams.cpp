#include "showparams.h"
#include "ui_showparams.h"
#include "Param.h"
#include "BaseModel.h"
#include "Err.h"
#include <QStandardItemModel>
#include "DB.h"

#define To_DB \
DB::Set ( "Диалоги", "Параметры", sizeof ( ShowParams_W ), static_cast<ShowParams_W*>(this));

#define From_DB \
{\
    int L = 0;\
    if ( DB::Get ( "Диалоги", "Параметры", sizeof ( ShowParams_W ), L, static_cast<ShowParams_W*>(this)))\
      {ASS(L == sizeof ( ShowParams_W ))}\
}

ShowParams::ShowParams(QWidget *parent, IBaseModel * _pObj )
  : QMainWindow(parent)
  , ui(new Ui::ShowParams)
{
  setWindowIcon( QIcon(":/book.png"));
  ui->setupUi(this);
  ui->ParamList->pMain = this;
  pObj = _pObj;
  if ( pObj->bp )
    ui->bp->setCheckState(Qt::Checked);
  else
    ui->bp->setCheckState(Qt::Unchecked);
  WinRect = geometry();
  QByteArray w = ui->splitter->saveState();
  w_split[0] = w[0];
  w_split[1] = w[1];
  cx_0 = ui->ParamList->columnWidth( 0 );
  From_DB
  setGeometry( WinRect );
  QByteArray S;
  S.append(w_split[0]);
  S.append(w_split[1]);
  ui->splitter->restoreState( S );
  ui->ParamList->setColumnWidth( 0, cx_0 );
//
  CLockParams Lock;
  pObj->GetParams( "" );
  kParams = CParams::kParams;
  pParams = new CParams[kParams];
  ui->ParamList->Model.kParams_old = kParams;
  ui->ParamTree->pParamsList = this;
  memmove (pParams, CParams::Params, kParams *sizeof ( CParams ));
  ui->ParamTree->Init ( kParams, pParams, (char*)pObj->ObjName );
  //ui->ParamList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
  //ui->ParamList->resizeColumnToContents( 1 );
  ui->ParamList->resizeRowsToContents();
//  ui->ParamList->setSectionResizeMode(QHeaderView::ResizeToContents);
  Sign = 0;
  KKK();
}

ShowParams::~ShowParams()
{
  delete ui;
  delete pParams;
}
#include <QItemSelectionModel>

void ParamsTree::Init ( int _kParams, CParams * _pParams, char * ObjName )
{
  QHeaderView * pH = header();
  pH->hide();
  kParams = _kParams;
  pParams = _pParams;
  QStandardItem * pItems[16];
  int Level = 0;
  pRoot = new QStandardItemModel();
  QStandardItem * parentItem = pRoot->invisibleRootItem();
  //  pItems[0] = NULL;
  pItems[Level] = parentItem;
  for ( int n = 0; n < kParams; n++ )
    {
    CParams & P = pParams[n];
    if ( P.Type == 'T' )
      {
      QStandardItem * pTab = new QStandardItem( P.ParamName ) ;
      pTab->setData( n );
      pItems[Level]->appendRow( pTab );
      QModelIndex ind = pTab->index();
      if ( P.ParamName[0] )
        {
        if (*(int*)P.Addr )
          expand( ind );
        else
          collapse( ind );
        }
      else
        KKK();
      Level++;
      if ( Level > 15 )
          Level = 15;
      pItems[Level] = pTab;
      continue;
      }
    if ( P.Type == 'E' )
    {
      Level--;
      if ( Level < 0 )
          Level = 0;
    }
  }
  //  pRoot = new QStandardItemModel();
  //  pRoot->appendRow( pItems[0] );
  //  QModelIndex rootIndex = model()->index(0, 0); // Получить индекс первого узла
  //  setRootIndex(rootIndex);
  setModel(pRoot);
  QItemSelectionModel* selectModel = selectionModel();
  QModelIndex rootIndex = model()->index(0, 0);
  selectModel->select(QItemSelection(rootIndex, rootIndex), QItemSelectionModel::Select | QItemSelectionModel::Rows);
  pParamsList->on_ParamTree_clicked( rootIndex );
  }

void ShowParams::on_ParamTree_clicked(const QModelIndex &index)
{
  QVariant N = ui->ParamTree->pRoot->data( index, Qt::UserRole + 1 );
  int n = N.toInt() + 1;
  int kP = 0;
  for ( int i = n; i < kParams; i++ )
  {
    CParams & P = pParams[i];
    if ( P.Type == 'T' || P.Type == 'E')
      break;
    kP++;
  }
  ui->ParamList->show ( pParams + n, kP );
}


void ShowParams::on_P_1_clicked()
{
  on_Change( 1. );
}


void ShowParams::on_M_1_clicked()
{
  on_Change( -1. );
}


void ShowParams::on_P_01_clicked()
{
  on_Change( 0.1 );
}


void ShowParams::on_M_01_clicked()
{
  on_Change( -0.1 );
}


void ShowParams::on_P_001_clicked()
{
  if ( Sign )
    on_Change( 0.001 );
  else
    on_Change( 0.01 );
}


void ShowParams::on_M_001_clicked()
{
  if ( Sign )
    on_Change( -0.001 );
  else
    on_Change( -0.01 );
}


void ShowParams::on_Sign_clicked()
{
  Sign = 1 - Sign;
  if ( Sign )
    ui->Sign->setText( "0.001");
  else
    ui->Sign->setText( "0.01");
}

void ShowParams::on_Change( double Delta )
  {
  Delta *= 0.01;
  //  ui->List->setModel();
  // QStringList List;
  // foreach (QFileInfo f, L)
  // {
  //   QString Name = f.fileName();
  //   Name.truncate( Name.size() - 4 );
  //   List.append( Name );
  // }
  QModelIndexList indexList =  ui->ParamList->selectionModel()->selectedIndexes();
  int row;
  foreach (QModelIndex index, indexList)
    {
    QVariant P = ui->ParamList->Model.data( index, Qt::UserRole );
    qlonglong Var = P.toULongLong();
    CParams * pP = (CParams*)Var;
    if ( pP->ParamName[0] == '#')
      continue;
    if ( pP->Type == 'D')
      {
      if ( Delta > 0. )
        *((double*)pP->Addr) *= 1. + Delta;
      else
        *((double*)pP->Addr) /= 1. - Delta;
      if ( pP->pModel )
        pP->pModel->UpdateParam( *pP );
      continue;
      }
    if ( pP->Type == 'I')
      {
      if ( Delta >= 0.099 )
        *((int*)pP->Addr) += 10;
      if ( Delta <= -0.099 )
        *((int*)pP->Addr) -= 10;
      if ( Delta > 0.0 )
        *((int*)pP->Addr) += 1;
      if ( Delta <= 0. )
        *((int*)pP->Addr) -= 1;
      if ( pP->pModel )
        pP->pModel->UpdateParam( *pP );
      continue;
      }
    }
  ui->ParamList->repaint();
  }


  void ShowParams::on_bp_checkStateChanged(const Qt::CheckState &arg1)
  {
      pObj->bp = ui->bp->isChecked();
  }

#include <SetDataTypes.h>

void ShowParams::on_Step1_clicked()
  {
  pObj->SetData( sd_PipeState, NULL );
  }

  void ShowParams::resizeEvent(QResizeEvent *event)
  {
    WinRect = geometry();
    To_DB
  }
  void ShowParams::moveEvent(QMoveEvent *event)
  {
    WinRect = geometry();
    To_DB
  }


void ParamsList::sectionResized( int logicalIndex, int oldSize, int newSize )
  {
  resizeRowsToContents();
  if ( logicalIndex == 0 )
    {
      pMain->cx_0 = newSize;
      pMain->SaveToDB();
    }
  }

ParamsList::ParamsList(QWidget * Parent ) : QTableView ( Parent )
  {
    pParams = NULL;
    kParams = 0;
    QHeaderView * headerV =  verticalHeader();
    QHeaderView * headerH =  horizontalHeader();
    connect( headerH, &QHeaderView::sectionResized, this, &ParamsList::sectionResized);
    headerV->setVisible(false);
    Model.pTable = this;
    setModel( &Model );
    setEditTriggers(QAbstractItemView::DoubleClicked);
  }

void ShowParams::on_splitter_splitterMoved(int pos, int index)
  {
  QByteArray w = ui->splitter->saveState();
  w_split[0] = w[0];
  w_split[1] = w[1];
  To_DB
  }

void ShowParams::SaveToDB()
  {
  To_DB
  }

void ShowParams::on_ParamTree_collapsed(const QModelIndex &index)
  {
  QVariant N = ui->ParamTree->pRoot->data( index, Qt::UserRole + 1 );
  int n = N.toInt() + 1;
  CParams & P = pParams[n];
  *(int*)P.Addr = 0;
  }


void ShowParams::on_ParamTree_expanded(const QModelIndex &index)
  {
  QVariant N = ui->ParamTree->pRoot->data( index, Qt::UserRole + 1 );
  int n = N.toInt() + 1;
  CParams & P = pParams[n];
  *(int*)P.Addr = 1;
  }

