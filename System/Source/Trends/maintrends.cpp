#include "maintrends.h"
#include "ui_maintrends.h"
#include "CommProc.h"
#include "qtrends.h"

MainTrends::MainTrends(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainTrends)
{
  strcpy ( PROJECT, "319_VSB_KF" );
  ui->setupUi(this);
  setWindowTitle( "Вывод трендов" );
//  setObjectName("319_VSB_KF");
  ui->centralwidget->setWindowTitle( "319_VSB_KF" );
  SetPaths();
  ui->listTrends->SetList( PROJECT_ROOT );
  timer = new QTimer ( this );
  connect(timer, &QTimer::timeout, this, &MainTrends::updateTime);
  timer->start( 1000 );
}

MainTrends::~MainTrends()
{
  delete ui;
}

void MainTrends::updateTime()
  {
  WinList::UpdateAllTime();
  }

void MainTrends::on_Open_clicked()
{
  QModelIndexList Sel = ui->listTrends->selectedIndexes();
    if ( !Sel.isEmpty())
    {
      foreach (QModelIndex Ind, Sel)
      {
        QString itemText = Ind.data(Qt::DisplayRole).toString();
        static char Name[256];
       strcpy ( Name, STR(itemText));
       QTrends * pT = new QTrends( NULL, Name );
       pT->show();
      }
  }
  ui->listTrends->clearSelection();

}



void MainTrends::on_listTrends_doubleClicked(const QModelIndex &index)
{
  on_Open_clicked();
}

