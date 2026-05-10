#include "CommProc.h"
#include "minitrend.h"
#include "ui_minitrend.h"

minitrend::minitrend(QWidget *parent)
  : QWidget(parent)
  , ui(new Ui::minitrend)
{
  ui->setupUi(this);
  Type = MiniTrend;
  ui->Wnd->show();
  minidraw * pDraw = ui->Wnd;
  pDraw->pMain = this;
  pDraw->mT = ui->ScaleT;
  pDraw->mD = ui->ScaleV;
  ui->ScaleT->setMinimum( 0 );
  ui->ScaleT->setMaximum( 1000 );
  ui->ScaleT->setValue( 1000 );
  ui->ScaleV->setMinimum( 0 );
  ui->ScaleV->setMaximum( 1000 );
  ui->ScaleV->setValue( 500 );
  // QPainter P(this );
  // QRect rc = rect();
  // P.fillRect( rc, Qt::black );

}

minitrend::~minitrend()
  {
  delete ui;
  }

bool minitrend::AddVar( int N )
  {
  minidraw * pDraw = ui->Wnd;
  if ( pDraw->kVars >= MAX_VAR )
    return false;
  pDraw->Vars[pDraw->kVars++] = N;
  ui->VarList->addItem( Trends.Items[N].Name.Str );
  return true;
  }

void minitrend::PreSave()
  {
  int P = 0;
  minidraw * pDraw = ui->Wnd;
  memmove ( Params + P, pDraw->Vars, sizeof (pDraw->Vars) );
  P += sizeof (pDraw->Vars);
  memmove ( Params + P, &pDraw->kVars, sizeof (pDraw->kVars) );
  }
void minitrend::AfterRestore( class MainWindow * pMainWnd )
  {
  // int P = 0;
  // minidraw * pDraw = ui->Wnd;
  // memmove ( pDraw->Vars, Params + P, sizeof (pDraw->Vars) );
  // P += sizeof (pDraw->Vars);
  // memmove ( &pDraw->kVars, Params + P, sizeof (pDraw->kVars) );
  }
/////////////////////////////////////////////////////////////////////////////
minidraw::minidraw( QWidget * pMain ) : QWidget ( pMain )
  {
  Shift = 0;
  Pen.setWidth( 1 );
  Pen.setStyle( Qt::SolidLine );
  CLEAR(Vars)
    kVars = 0;
    colVars[0] = RGB(255,0,0);
    colVars[1] = RGB(0,255,0);
    colVars[2] = RGB(0,0,255);
    colVars[3] = RGB(255,255,255);
    colVars[4] = RGB(255,255,0);
    colVars[5] = RGB(255,0,255);
    colVars[6] = RGB(0,255,255);
    colVars[7] = RGB(192,192,192);
    New_Pres = true;
    FirstDraw = true;
    StartPos = 0;
    Pause = false;
    CLEAR(Min)
    CLEAR(Max);
    Steps = 1;
  }

void minidraw::MM( int N, double V0 )
  {
  if ( fabs ( V0 ) < 1e-6 )
    {
      if ( V0 > 0. )
        V0 = 1e-6;
      else
        V0 = -1e-6;
    }
  double D = fabs ( V0 ) * d;
  if ( New_Pres )
  {
    Min[N] = V0 - D;
    Max[N] = Min[N] + 2. * D;
  }
  else
  {
    if ( Min[N] > V0 )
    {
      Min[N] = V0 - D;
      Max[N] = Min[N] + 2. * D;
    }
    if ( Max[N] < V0 )
    {
      Max[N] = V0 + D;
      Min[N] = Max[N] - 2. * D;
    }
  }
  }

void minidraw::paintEvent( QPaintEvent *event )
  {
  QPainter P(this );
  QRectF rc = rect();
  P.fillRect( rc, Qt::black );
  int nWidth = rc.width();
  int nHeight = rc.height();
  QPen PenRamk ( Qt::DotLine );//SolidLine );
  PenRamk.setWidth( 1 );
  PenRamk.setColor(RGB ( 0, 92, 0 ));
  P.setPen( PenRamk );
  const double Amax = log( 0.5 );
  const double Amin = log( 1e-4 );
  double m = mD->sliderPosition() * 0.001;
  d = exp ( Amin + m * ( Amax - Amin ));
  double L = log10 ( d ) + 2;
  char Txt[32];
  sprintf ( Txt, "%2.1lf", L );
  pMain->ui->pow->setText( Txt );
  const double Tmax = log( 100 );
  const double Tmin = 0.;
  m = mT->sliderPosition() * 0.001;
  double K = exp ( Tmax + m * ( Tmin - Tmax ));
  Steps = K;
  if ( Steps < 1 )
    Steps = 1;
  sprintf ( Txt, "%d", Steps );
  pMain->ui->step->setText( Txt );
  double Alfa = 0.1;
  for ( int n = 0; n < 9; n++ )
  {
    int y = (int)( Alfa * nHeight );
    Alfa += 0.1;
    P.drawLine( 0, y,  nWidth, y );
  }
  Alfa = 0.1;
  for ( int n = 0; n < 9; n++ )
  {
    int x = (int)( Alfa * nWidth );
    Alfa += 0.1;
    P.drawLine( x, 0,  x, nHeight );
  }
  if ( SetShift == -2 )
    {
    Pause = true;
    }
  if ( SetShift == -1 )
    {
    Shift += nWidth / 4;
    Pause = true;
    }
  if ( SetShift == 1 )
    {
    Shift -= nWidth / 4;
    Pause = true;
    }
  if ( SetShift == 2 )
    {
    Shift = 0;
    Pause = false;
    }
  SetShift = 0;
  if ( Shift < 0 )
    Shift = 0;
  if ( !Pause )
    StartPos = Trends.PosRecords;
  int xo = -1, yo = -1;
  for ( int nV = 0; nV < kVars;  nV++ )
    {
    int N = Vars[nV];
    Pen.setColor( colVars[nV] );
    P.setPen( Pen );
    int nStart = StartPos - Shift;
    for ( int x = nWidth - 1; x >= 0; x-- )
      {
      if ( nStart < 0 )
        break;
      double V0 = 0;
      int K = 0;
      for ( int j = 0; j < Steps; j++ )
        {
        Record & R = Trends.pRecords[nStart--];
        double V = R.Vars[N];
        V0 += V;
        K++;
        if ( nStart < 0 )
          break;
        }
      if ( K > 0 )
        V0 /= K;
      if ( xo < 0 )
        MM( nV, V0 );
      if ( FirstDraw )
        continue;
      double AlfaY = ( Max[nV] - V0 ) / ( Max[nV]- Min[nV]);
      int y = nHeight * AlfaY;
      if ( xo >= 0 )
        P.drawLine( xo, yo, x, y );
      xo = x;
      yo = y;
      }
    }
  New_Pres = false;
  FirstDraw = false;
  }


void minitrend::updateTime()
  {
  ui->Wnd->repaint();
  }

void minitrend::resizeEvent(QResizeEvent *event)
  {
    WinRect = geometry();
    change = true;
  }

void minitrend::moveEvent(QMoveEvent *event)
  {
    WinRect = geometry();
    change = true;
  }

void minitrend::closeEvent(QCloseEvent * event)
  {
    Close();
  }


void minitrend::on_ScaleV_sliderMoved(int position)
  {
  ui->Wnd->New_Pres = true;
  ui->Wnd->repaint();
  }


void minitrend::on_Back_clicked()
  {
  ui->Wnd->SetShift = -1;
  ui->Wnd->repaint();
  }


void minitrend::on_Forvard_clicked()
  {
  ui->Wnd->SetShift = 1;
  ui->Wnd->repaint();
  }


  void minitrend::on_Continue_clicked()
  {
  ui->Wnd->SetShift = 2;
  ui->Wnd->repaint();
  }


  void minitrend::on_Pause_clicked()
  {
  ui->Wnd->SetShift = -2;
  ui->Wnd->repaint();
  }


void minitrend::on_ScaleT_sliderMoved(int position)
  {
  ui->Wnd->repaint();
  }

