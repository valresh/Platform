#include "wtrends.h"
#include "ui_wtrends.h"
#include "CommProc.h"
#include "showtrends.h"
#include "Err.h"

#define NEXT \
P = E + 1;\
  E = strchr ( P, ';' );\
  if ( E == NULL )\
  continue;

WTrends::WTrends(const char * FileTrend, QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::WTrends)
{
  Ready = false;
  ui->setupUi(this);
//  QWidget * central = new QWidget(this);
  imageLabel = new QLabel( this );
  // imageLabel->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  // imageLabel->setBackgroundRole(QPalette::Base);
  // imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  // imageLabel->setText(tr("Тренд"));
  setWindowIcon( QIcon(":/windows.png"));
  FileName = FileTrend;
  setWindowTitle( FileTrend );
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
  Char<1024>Path;
  Path.Prt ("%sINI/Trends/%s.csv", PROJECT_ROOT, FileTrend );
  FILE * F = fopen ( Path, "rt" );
    if ( F == NULL )
      return;
  // pShow = new ShowTrends( this );
  // QRect rc = geometry();
  // pShow->setGeometry( rc );
  char Str[1024];
  fgets ( Str, 1024, F );
  kVar = 0;
    while ( fgets ( Str, 1024, F ))
    {
      int n = kVar;
        if ( n >= MAX_VAR )
          break;
    TrendVar & V = Vars[n];
      kVar = n + 1;
      char * P = Str;
      //
      char * E = strchr ( P, ';' );
        if ( *E )
          *E = 0;
    V.Name = P;
      V.Addr = ConnectPoint( V.Name.Str, V.Type );
      V.Trend_ID =  Trends.Add( V.Name.Str, V.Type, V.Addr );
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
  // pShow->show();
  // pShowKey = (int64_t)pShow;
  Ready = true;
}

WTrends::~WTrends()
{
  delete ui;
}

typedef  void *test();
void WTrends::updateTime()
{
  // if ( pShowKey != (int64_t)pShow )
  //    {
  //    KKK();
  //    return;
  //    }
  repaint();
}

void WTrends::paintEvent( QPaintEvent *event )
{
  if ( !Ready )
    return;
  QSize targetSize = size();//  QRectF rc = rect();
  QPixmap pixmap(targetSize);
  pixmap.fill(Qt::white);
  imageLabel->setPixmap(pixmap);
  static QPainter P;//(&pixmap);
  P.begin( this );
  const int dY_bott = 20;
  const int dX_left = 40;
  QRectF rc = rect();
  rc.setLeft( rc.left() + dX_left );
  rc.setBottom( rc.bottom() - dY_bott );
  P.fillRect( rc, Qt::black );
  imageLabel->setPixmap(pixmap);
  imageLabel->resize(pixmap.size());
  return;
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
  return;
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
  Pause = false;
  SetShift = 0;
    if ( Shift < 0 )
      Shift = 0;
  if ( !Pause )
      StartPos = Trends.pHead->PosRecords % MAX_STEP;
  double m = 1.;//mT->sliderPosition() * 0.001;
  const double Tmax = log( 100. );
  const double Tmin = log( 1. );
  double K = 1.;//exp ( Tmax + m * ( Tmin - Tmax ));
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
  //@@  step->setText( Txt );
  int nStart = StartPos - Shift;
  Record & R = Trends.R(nStart);
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
    ShowSteps = true;
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
          Record & R = Trends.R(nStart--);
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
      if ( xo >= 0 && abs ( xo - x ) >= 5 )
        {
//        P.drawLine( xo, yo, x, y );
        xo = x;
        yo = y;
        }
      if ( xo < 0 )
          {
            xo = x;
            yo = y;
          }
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
          NextLab -= dX_Lab;
          P.restore();
      }
  }
}
  P.end();
//  imageLabel->setPixmap(pixmap);
//  imageLabel->resize(pixmap.size());
KKK();
}
// if ( nSelected >= 0 )
//   {
//       int nStart = StartPos;
//       Record & R = Trends.pRecords[nStart];
//       int nV = Vars[nSelected].Trend_ID;
//       double V = R.Vars[nV];
//       char Txt[256];
//       OutDI( &V, 'D', Txt );
//       SelValue->setText(Txt);
//       SelVar->setText(Vars[nSelected].Name.Str);
//   }
// else
//     SelVar->setText("");
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
P.restore();
}

