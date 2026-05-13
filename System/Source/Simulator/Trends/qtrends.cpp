#include "qtrends.h"
#include "showtrends.h"
#include "ui_qtrends.h"
#include "CommProc.h"
#include "mainwindow.h"

#define NEXT \
  P = E + 1;\
  E = strchr ( P, ';' );\
  if ( E == NULL )\
    continue;

#define To_DB \
  DB::Set ( DB::_("Тренды", FileName), sizeof ( QTrends_W ), static_cast<QTrends_W*>(this));

#define From_DB \
  {\
  int L = 0;\
  if ( DB::Get ( DB::_("Тренды", FileName), sizeof ( QTrends_W ), L, static_cast<QTrends_W*>(this)))\
    {\
    ASS(L == sizeof ( QTrends_W ))\
    }\
  }

QTrends::QTrends(QWidget *parent, const char * FileTrend )
  : QMainWindow(parent)
  , ListVar (this )
  , ui(new Ui::QTrends)
  {
  ui->setupUi(this);
  setWindowIcon( QIcon(":/windows.png"));
  Type = Trend;
  ui->TimeStep->setText( "Шаги" );
  Name = FileTrend;
  FileName = FileTrend;
  setWindowTitle( FileTrend );
  ListVar.Model.pTrends = ui->Wnd;
  ShowTrends * pDraw = ui->Wnd;
  ListVar.Recv( pDraw );
  pDraw->mT = ui->ScaleT;
  pDraw->step = ui->step;
  pDraw->SelVar = ui->SelVar;
  pDraw->SelValue = ui->SelValue;
//  QLineEdit *step;
  ScaleT = 1000;
  ShowSteps = true;
  // QTrends_W::WinRect = geometry();
  // From_DB
  // setGeometry( QTrends_W::WinRect );
  ui->Wnd->ShowSteps = true;
  ui->ScaleT->setMinimum( 0 );
  ui->ScaleT->setMaximum( 1000 );
  ui->ScaleT->setValue( ScaleT );
// Чтение файла
  Char<1024>Path;
  Path.Prt ("%sINI/Trends/%s.csv", PROJECT_ROOT, FileTrend );
  FILE * F = fopen ( Path, "rt" );
  if ( F == NULL )
    return;
  ShowTrends * pShow = ui->Wnd;
  char Str[1024];
  fgets ( Str, 1024, F );
  while ( fgets ( Str, 1024, F ))
    {
    int n = pShow->kVar;
    if ( n >= MAX_VAR )
      break;
    TrendVar & V = pShow->Vars[n];
    pShow->kVar = n + 1;
    char * P = Str;
//
    char * E = strchr ( P, ';' );
    if ( *E )
      *E = 0;
    V.Name = P;
    V.Addr = ConnectPoint( V.Name.Str, V.Type );
    V.Trend_ID =  pMainWnd->Trends.Add( V.Name.Str, V.Type, V.Addr );
    NEXT
    //M ???
    NEXT
    V.Min = atof ( P );
    NEXT
    V.Max = atof ( P );
    NEXT
    // R,G,B
    int r = 0, g = 0, b = 0;
    char * Q = P;
    r = atoi ( Q );
    char * D = strchr ( P, ',' );
    Q = D + 1;
    g = atoi ( Q );
    D = strchr ( Q, ',' );
    Q = D + 1;
    b = atoi ( Q );
    V.color = RGB(r,g,b);
    NEXT
    if ( atoi ( P ) )
      V.UseNom = true;
    else
      V.UseNom = true;
    NEXT
    V.Nom = atof ( P );
    NEXT
    //??
    NEXT
    V.Pres = atoi ( P );
    }
  // QAction * newAct = new QAction(tr("&Новый"), this);
  // newAct->setShortcuts(QKeySequence::New);
  // newAct->setStatusTip(tr("Новый файл трендов"));
  // co11nnect(newAct, SIGNAL(triggered()), this, SLO22T(ne22wFile()));

  // QAction * openAct = new QAction(tr("&Open..."), this);
  // openAct->setShortcuts(QKeySequence::Open);
  // openAct->setStatusTip(tr("Open an existing file"));
  // conn11ect(openAct, SIGNAL(triggered()), this, SLO22T(op22en()));
  // QWidget *centralWidget = new QWidget(this);

  // QMenu * fileMenu = menuBar()->addMenu(tr("&Файл"));
  // fileMenu->addAction(newAct);
  // fileMenu->addSeparator();
  // fileMenu->addAction(openAct);
//
  }

bool QTrends::AddTrend( const char * Name, char Type, void * pVar )
  {
  ShowTrends * pShow = ui->Wnd;
  int n = pShow->kVar;
  if ( n >= MAX_VAR )
    return false;
  TrendVar & V = pShow->Vars[n];
  pShow->kVar = n + 1;
  V.Name = Name;
  V.Type = Type;
  V.Addr = pVar;
  V.Trend_ID =  pMainWnd->Trends.Add( V.Name.Str, V.Type, V.Addr );
  V.Min = 0.;//atof ( P );
  V.Max = 0.;//atof ( P );
  V.color = RGB(255,255,255);
  V.UseNom = false;
  V.Nom = 0.;
  V.Pres = 1;
  }

QTrends::~QTrends()
{
  delete ui;
}
// void QTrends::open()
// {

// }

void QTrends::updateTime()
{
  ui->Wnd->repaint();
  ListVar.Model.timerHit();
  if ( ui->Wnd->Pause )
    ui->Pause->setStyleSheet("QPushButton { background-color: #FF0000; } " );
  else
    ui->Pause->setStyleSheet("QPushButton { background-color: #F0F0F0; } " );
}
void QTrends::resizeEvent(QResizeEvent *event)
{
  QTrends_W::WinRect = geometry();
  change = true;
  To_DB
}
void QTrends::moveEvent(QMoveEvent *event)
{
  QTrends_W::WinRect = geometry();
  change = true;
  To_DB
}
void QTrends::closeEvent(QCloseEvent * event)
{
  Close();
}

void QTrends::on_ShowVars_clicked()
{
  ListVar.setWindowTitle( Name.Str );
  ListVar.show( ui->Wnd->Vars, ui->Wnd->kVar );
}

void QTrends::on_Back_clicked()
{
  ui->Wnd->SetShift = -1;
}


void QTrends::on_Forvard_clicked()
{
  ui->Wnd->SetShift = 1;
}


void QTrends::on_Pause_clicked()
{
  ui->Wnd->SetShift = -2;
}


void QTrends::on_Continue_clicked()
{
  ui->Wnd->SetShift = 2;

}


void QTrends::on_Up_clicked()
{
}


void QTrends::on_Down_clicked()
{

}


void QTrends::on_Align_clicked()
{

}


void QTrends::on_ScaleT_sliderMoved(int position)
{
  ScaleT = ui->ScaleT->sliderPosition();
To_DB
}


void QTrends::on_TimeStep_pressed()
  {
  ui->Wnd->ShowSteps =!ui->Wnd->ShowSteps;
  if ( ui->Wnd->ShowSteps )
    ui->TimeStep->setText( "Шаги" );
  else
    ui->TimeStep->setText( "Время" );
  }


void QTrends::on_ScaleT_sliderPressed()
  {
  ScaleT = ui->ScaleT->sliderPosition();
  To_DB
  }


 void QTrends::on_TimeStep_clicked()
  {
  ShowSteps = ui->Wnd->ShowSteps;
  To_DB
  }

