#include "showtrends.h"
#include <QPainter>
#include "Err.h"
#include <QMenu>
#include "trendsvar.h"
#include "mainwindow.h"

ShowTrends::ShowTrends(QWidget *parent)
  : QWidget{parent}, VarProp(this)
  {
//  CLEAR(Vars)
  setWindowIcon( QIcon(":/windows.png"));
  kVar = 0;
  SetShift = 0;
  Shift = 0;
  StartPos = 0;
  Pause = false;
  Steps = 1;
  mTime = 1. / 0.36;
  pntMouse.setX( -100 );
  pntMouse.setY( -100 );
  nSelected = -1;
  PressMouse = false;
  Drug = false;
  nWidth = 100;
  nHeight = 100;
  }

void ShowTrends::SetLimits( int nVar )
  {
  int N = Vars[nVar].Trend_ID;
  int nStart = StartPos - Shift;
  double Min = 1e10;
  double Max = -1e10;
  for ( int n = 10; n <StartPos; n++ )
    {
    Record & R = Trends.pRecords[nStart--];
    double V = R.Vars[N];
    if ( V < Min )
      Min = V;
    if ( V > Max )
      Max = V;
    }
  double M = 0.5 * ( Min + Max );
  double D = ( Max - Min );
  if ( D < 1e-6 )
    D = 1e-6;
  Vars[nVar].Min = M - 0.6 * D;
  Vars[nVar].Max = M + 0.6 * D;
  }


void ShowTrends::paintEvent( QPaintEvent *event )
{
  const int dY_bott = 20;
  const int dX_left = 40;
  QPainter P(this );
  QRectF rc = rect();
  rc.setLeft( rc.left() + dX_left );
  rc.setBottom( rc.bottom() - dY_bott );
  P.fillRect( rc, Qt::black );
  nWidth = rc.width();
  nHeight = rc.height();
  QPen PenRamk ( Qt::DotLine );//SolidLine );
  PenRamk.setWidth( 1 );
  PenRamk.setColor(RGB ( 0, 92, 0 ));
  P.setPen( PenRamk );
  double Alfa = 0.1;
  for ( int n = 0; n < 9; n++ )
  {
    int y = (int)( Alfa * nHeight );
    Alfa += 0.1;
    P.drawLine( dX_left, y,  nWidth+dX_left, y );
  }
  Alfa = 0.1;
  int dX_Lab = Alfa * nWidth;
  for ( int n = 0; n < 9; n++ )
  {
    int x = (int)( Alfa * nWidth + dX_left );
    Alfa += 0.1;
    P.drawLine( x, 0,  x, nHeight );
  }
////////////////////////////////////////////
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
  double m = mT->sliderPosition() * 0.001;
  const double Tmax = log( 100. );
  const double Tmin = log( 1. );
  double K = exp ( Tmax + m * ( Tmin - Tmax ));
  Steps = K;
  if ( Steps < 1 )
    Steps = 1;
  char Txt[64];
  Mtime = mTime / K;
  if ( ShowSteps )
    {
    Sprintf ( Txt, "%dшаг", Steps * nWidth );
    }
  else
    {
    Sprintf ( Txt, "%3.1lfмин", nWidth / ( 60. * Mtime ));
    }
  step->setText( Txt );
  int nStart = StartPos - Shift;
  Record & R = Trends.pRecords[nStart];
  double TimeMax = R.Time;
  bool ValidPnt = false;
  int NextLab = 0;
  bool WasSelected = false;
  for ( int nV = 0; nV < kVar;  nV++ )
    {
    int N = Vars[nV].Trend_ID;
    Pen.setColor( Vars[nV].color );
    if ( nV == nSelected )
      Pen.setWidth( 3 );
    else
      Pen.setWidth( 1 );
    int nStart = StartPos - Shift;
    int xo = -1, yo = -1;
    int w = nStart / Steps;
    if ( w > nWidth || !ShowSteps )
      w = nWidth;
    int po = -1;
    for ( int p = w - 1; p >= 0; p-- )
      {
      po = p;
      if ( nStart <= 1 )
        break;
      double V0 = 0;
      int K = 0;
      double LastTime = 0.;
      int LastStep = 0;
      for ( int j = 0; j < Steps; j++ )
        {
        Record & R = Trends.pRecords[nStart--];
        LastTime = R.Time;
        LastStep = R.Step;
        double V = R.Vars[N];
        V0 += V;
        K++;
        if ( nStart <= 1 )
          break;
        }
      if ( K > 0 )
        V0 /= K;
      if ( xo < 0 )
        {
        if ( Vars[nV].UseNom )
          {
          QPen PenNom ( Qt::DashLine);
          PenNom.setColor( Vars[nV].color );
          PenNom.setWidth( 1 );
          P.setPen( PenNom );
          double AlfaY = ( Vars[nV].Max - Vars[nV].Nom ) / ( Vars[nV].Max- Vars[nV].Min);
          if ( AlfaY > 0. && AlfaY <= 1. )
            {
            int y = nHeight * AlfaY;
            P.drawLine( dX_left, y, nWidth + dX_left, y );
            }
          P.setPen( Pen );
          }
        }
      double AlfaY = ( Vars[nV].Max - V0 ) / ( Vars[nV].Max- Vars[nV].Min);
      int x;
      if ( ShowSteps )
        x = p + dX_left;
      else
        x = w - ( TimeMax - LastTime ) * Mtime + dX_left;
      nVar[x] = nStart + 1;
      if ( x < dX_left )
        break;
      if ( AlfaY > 0. && AlfaY <= 1. )
        {
        int y = nHeight * AlfaY;
        if ( xo >= 0 )
          P.drawLine( xo, yo, x, y );
        xo = x;
        yo = y;
        if ( abs ( x - pntMouse.x()) < 8 && abs ( y - pntMouse.y()) < 8)
          {
          WasSelected = true;
          nSelected = nV;
          }
        }
      else
        {
        xo = -1;
        yo = -1;
        }
      if ( nV == 0 )
        {
        if ( p == w - 1 )
          {
          int Grp = ( x - dX_left ) / dX_Lab;
          NextLab = Grp * dX_Lab;
          }
        if ( x - dX_left < NextLab )
          {
          char Txt[64];
          if ( ShowSteps )
            {
            Sprintf ( Txt, "%d", LastStep );
            }
          else
            {
            Sprintf ( Txt, "%3.1l", nWidth / ( 60. * Mtime ));
            }
//            painter.setFont(font);
            QRect rc( x + dX_left - 20, nHeight + 5, 40, 15 );
            P.save();
            P.setPen(Qt::black);
            P.drawText( rc, Qt::AlignCenter, Txt );
            P.restore();
            NextLab -= dX_Lab;
          }
        }
      }
    KKK();
    }
  if ( nSelected >= 0 )
    {
        int nStart = StartPos;
        Record & R = Trends.pRecords[nStart];
        int nV = Vars[nSelected].Trend_ID;
        double V = R.Vars[nV];
        char Txt[256];
        OutDI( &V, 'D', Txt );
        SelValue->setText(Txt);
        SelVar->setText(Vars[nSelected].Name.Str);
    }
  else
      SelVar->setText("");
  pntMouse.setX( -100 );
  pntMouse.setY( -100 );
  if ( PressMouse )
    {
    if ( WasSelected )
      {
      PressMouse = false;
      repaint();
      }
    else
      nSelected = -1;
    }
  PressMouse = false;
//  New_Pres = false;
//  FirstDraw = false;
  }

#include <QMouseEvent>
void ShowTrends::OnLbuttonDown(QMouseEvent *event)
  {

  }

void ShowTrends::ListVarRbuttonDown(QTableView * pTable, QModelIndex & I)
  {
  QMenu menu;
  QAction * actions[5];
  actions[0] = menu.addAction("Показать параметры");
  actions[1] = menu.addAction("Найти на схемах");
  QAction * selectedAction = menu.exec(QCursor::pos());
  int r = I.row();
  if ( selectedAction == actions[0])
    {
      VarProp.Dial( &Vars[r] );
      return;
    }
  if ( selectedAction == actions[1])
    {
      pMainWnd->Find.Find  ( Vars[r].Name );
      return;
    }
  }

void ShowTrends::ListVarLbuttonDown(QTableView * pTable, QModelIndex & I )
  {
  nSelected = I.row();
  }

void ShowTrends::OnRbuttonDown(QMouseEvent *event)
  {
  QMenu menu;
  QAction * actions[5];
  int K = 0;
  actions[0] = menu.addAction("Пауза");
  K++;
  actions[1] = menu.addAction("Продолжить");
  K++;
  if ( nSelected >= 0 )
  {
    actions[2] = menu.addAction( Vars[nSelected].Name.Str );
    K++;
  }
  // Получаем индекс ячейки под курсором
  QAction* selectedAction = menu.exec(this->mapToGlobal(event->pos()));
  int N = -1;
  for ( int n = 0; n < 3; n++ )
  {
    if ( selectedAction == actions[n])
    {
      N = n;
      break;
    }
  }

  switch ( N )
  {
    case 0:
      Pause = true;
      break;
    case 1:
      Pause = false;
      break;
    case 2:
      {
        VarProp.Dial( &Vars[nSelected] );
        break;
      }
  }
}

void ShowTrends::mousePressEvent(QMouseEvent *event)
  {
  if (event->button() == Qt::LeftButton)
    {
    PressMouse = true;
    pntMouse = event->pos();
    pntDrug = pntMouse;
    CLEAR(nVar)
    repaint();
    Drug = true;
    KKK();
    if ( nSelected >= 0 )
        {
        int x = event->pos().x();
        int N = nVar[x];
        if ( N >= 0 )
        {
        Record & R = Trends.pRecords[N];
        int ID = Vars[nSelected].Trend_ID;
         double V = R.Vars[ID];
        char Txt[256];
        OutDI( &V, 'D', Txt);
        QToolTip::showText(mapToGlobal(event->pos()),Txt);
        }
        }
    }
  if (event->button() == Qt::RightButton)
    {
      OnRbuttonDown( event );
    }
  }

void ShowTrends::mouseReleaseEvent(QMouseEvent *event)
  {
  Drug = false;
  }

  void ShowTrends::mouseDoubleClickEvent(QMouseEvent *event)
  {

  }

void ShowTrends::mouseMoveEvent(QMouseEvent *event)
  {
  QPoint pnt = event->pos();
  if ( nSelected >= 0 )
    {
    double Delta = fabs ( Vars[nSelected].Max - Vars[nSelected].Min ) / nHeight;
    double  dy = ( pnt.y() - pntDrug.y()) * Delta;
    Vars[nSelected].Max += dy;
    Vars[nSelected].Min += dy;
    }
  else
    {
    for ( int nV = 0; nV < kVar;  nV++ )
      {
      double Delta = fabs ( Vars[nV].Max - Vars[nV].Min ) / nHeight;
      double dy = ( pnt.y() - pntDrug.y()) * Delta;
      Vars[nV].Max += dy;
      Vars[nV].Min += dy;
      }
    }
  if ( Pause )
    {
    double dx = ( pnt.x() - pntDrug.x());
    if ( ShowSteps )
      {
      dx *= Steps;
      }
    else
      {
      dx /= 60. * Mtime;
      }
    Shift -= dx;
    if ( Shift < 1 )
      Shift = 1;
    }
  pntDrug = pnt;
  repaint();
  }

void ShowTrends::wheelEvent(QWheelEvent *event)
  {
  const double k = 0.0001;
  QPoint Pnt = event->angleDelta();
  if ( nSelected >= 0 )
    {
    double M = 0.5 * ( Vars[nSelected].Max + Vars[nSelected].Min );
    double Delta = Vars[nSelected].Max - Vars[nSelected].Min;
    Delta *= 1. + k * Pnt.y();
    Vars[nSelected].Max = M + 0.5 * Delta;
    Vars[nSelected].Min = M - 0.5 * Delta;
    }
  else
    {
    for ( int nV = 0; nV < kVar;  nV++ )
      {
      double M = 0.5 * ( Vars[nV].Max + Vars[nV].Min );
      double Delta = Vars[nV].Max - Vars[nV].Min;
      Delta *= 1. + k * Pnt.y();
      Vars[nV].Max = M + 0.5 * Delta;
      Vars[nV].Min = M - 0.5 * Delta;
      }
    }
  repaint();
  }
