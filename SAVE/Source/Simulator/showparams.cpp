#include "showparams.h"
#include "ui_showparams.h"
#include "Param.h"
#include "BaseModel.h"
#include "Err.h"
#include <QStandardItemModel>

ShowParams::ShowParams(QWidget *parent, IBaseModel * _pObj )
  : QMainWindow(parent)
  , ui(new Ui::ShowParams)
{
  ui->setupUi(this);
  pObj = _pObj;
  CLockParams Lock;
  pObj->GetParams( "" );
  kParams = CParams::kParams;
  pParams = new CParams[kParams];
  ui->ParamList->Model.kParams_old = kParams;
  memmove (pParams, CParams::Params, kParams *sizeof ( CParams ));
  ui->ParamTree->Init ( kParams, pParams, (char*)pObj->ObjName );
  //ui->ParamList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch );
  //ui->ParamList->resizeColumnToContents( 1 );
  ui->ParamList->resizeRowsToContents();
  Sign = 0;
  KKK();
}

ShowParams::~ShowParams()
{
  delete ui;
  delete pParams;
}

void ParamsTree::Init ( int _kParams, CParams * _pParams, char * ObjName )
{
  QHeaderView * pH = header();
  pH->hide();
  kParams = _kParams;
  pParams = _pParams;
  QStandardItem * pItems[10];
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
      Level++;
      pItems[Level] = pTab;
      continue;
    }
    if ( P.Type == 'E' )
    {
      Level--;
    }
  }
  //  pRoot = new QStandardItemModel();
  //  pRoot->appendRow( pItems[0] );
  //  QModelIndex rootIndex = model()->index(0, 0); // Получить индекс первого узла
  //  setRootIndex(rootIndex);
  setModel(pRoot);
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

