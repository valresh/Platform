#include "showsheme.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QKeyEvent>
#include <QPainter>
#include <string.h>
#include <QMenu>
#include "listemf.h"
#include "showparams.h"
#include "WinDraw.h"
#include "SysDataTypes.h"
#include <QFileInfo>

int K = 0;

double dx_0 = 0;
double dy_0 = 372;
double M_0 = 3.903;
double Shft_0 = 1.;
QPointF PosMouse;

void P_to_P( QPointF Pos, double & X, double & Y );

void ExtPaint ( void * _pSheme, QPainter * pP )
  {
  ShowSheme * pS = (ShowSheme*)_pSheme;
  if ( pS )
    {
    pS->Draw( pP );
    QPen pen(Qt::yellow, 2, Qt::SolidLine);
    pP->setPen( pen );
    SS * pSS = &pS->ss;
    double X, Y;
    P_to_P( pSS, PosMouse, X, Y );
    QPoint Pnt( X, Y );
    pP->drawEllipse( Pnt, 3, 3 );
    //pP->drawPixmapFragments()
    }
// //    pP->drawLine(200, 200, 300, 300);
//     double L, T, R, B;
//     pS->Model_To_Emf ( L, T, R, B, pS->Etalon );
//     pP->drawLine( L, T, R, T);
//     pP->drawLine( R, T, R, B);
//     pP->drawLine( R, B, L, B);
//     pP->drawLine( L, B, L, T );
  // pP->fillRect( 200, 200, 200, 200, Qt::GlobalColor::white );
  // QString S = QString("%1").number ( K++ );
  // pP->setFont(QFont("Arial", 50));
  // pP->drawText( QPointF(250,300), S );
  }


int kObCrdj;
const char * crd = NULL;

ShowSheme::ShowSheme( QWidget *parent, const char * File  )
  : EMF( parent, File )
  {
  Type = Shema;
  ss.pSheme = this;
//  QFileInfo file ( Path );
  FileName = File;//STR(file.baseName());
  setMinimumSize( 1500, 1000 );
  Drug = false;
  pCrd = NULL;
  kCrd = 0;
  pMainHydro->SetData ( 1019, (void*)&pDataPtr);
  //pDataPtr
  setMouseTracking(true);
  }
#if 0
BOOL CSchemeView::IsOnScheme (LPCTSTR szObjScheme)
{
    int nLen = szObjScheme ? lstrlen (szObjScheme) : 0;
    if ( nLen <= 1 )
        return FALSE;
    if ( szObjScheme[nLen-1] == _T(';') )
        nLen --;
    if ( nLen != m_cName.GetLength () )
        return FALSE;
    return (_tcsnicmp (m_cName, szObjScheme, nLen) == 0);
}
    for(DWORD n = 0; n < m_pDataPtr->kPipeRef; n++)
    {
        CPipeRef * pP = m_pDataPtr->pPipeRef[n];
        if ( !IsOnScheme ((LPCTSTR)pP->Shema) )
            continue;
        if ( lstrcmp (pP->PipeName, szPipe ) == 0 )
        {
            pP->Selected = true;

            CSegment * pSeg = pP->pFirstSeg;
            if ( pSeg->XY == NULL )
                return;

            CRect rcObj = GetPipeRect (pP);
            EnsureObjVisible (rcObj);
/*
            double xS = pSeg->XY[0];
            double yS = pSeg->XY[1];
            int dx, dy;
            GetPagePos (dx, dy);
            int x = (int)xS + dx;
            int y = (int)yS + dy;
            int Dx = (m_rcClient.right >> 1) - x;
            int Dy = (m_rcClient.bottom >> 1) - y;
            m_sPosScm.nShiftH += Dx;
            m_sPosScm.nShiftV += Dy;
*/
  }
  else
    pP->Selected = false;
  }
  Invalidate();
    for(DWORD n = 0; n < m_pDataPtr->kObjRef; n++)
    {
        CObjectRef * pO = m_pDataPtr->pObjRef[n];
        if ( !IsOnScheme ((LPCTSTR)pO->Shema) )
            continue;
        if ( pO->pObject == NULL )
            continue;
        if ( pO->pObject->ModelFlags & 0x80 )
            continue;
        if ( pO->pObject->NeedDrawBefore() != bBefore )
            continue;
        DrawData.ObjName = pO->ObjName;
        DrawData.rc.left = (int)(pO->L * m_sPosScm.dZoom) + dx;
        DrawData.rc.right = (int)(pO->R * m_sPosScm.dZoom) + dx;
        DrawData.rc.top = (int)(pO->T * m_sPosScm.dZoom) + dy;
        DrawData.rc.bottom = (int)(pO->B * m_sPosScm.dZoom) + dy;
        DrawObject (pO->pObject, &DrawData);
        if ( pO->Selected )
        {
            CRect rc = DrawData.rc;
            int s = 10;
            int x = rc.left-s;
            int y = rc.top-s;
            int w = abs ( rc.right - rc.left ) + 2*s;
            int h = ( rc.bottom - rc.top ) + 2*s;
            CDC MemSelect;
            CBitmap bmSelect;
            MemSelect.CreateCompatibleDC( pDC );
            bmSelect.CreateCompatibleBitmap( pDC, w, h );
            CBitmap* pBm = MemSelect.SelectObject(&bmSelect);
            CRect rect ( 0, 0, w, h );
            MemSelect.FillRect(rect, &CBrush(RGB(255,255,0)));
            BLENDFUNCTION blend = {AC_SRC_OVER, 0, 120, 0};
            AlphaBlend( pDC->m_hDC, x, y, w, h,
            MemSelect.m_hDC,0,0,w,h,blend);
            MemSelect.SelectObject(pBm);
        }
    }
}
*/
#endif
//bool ShowSheme::event (QEvent * event )
//  {
//
//  }

void ShowSheme::mousePressEvent(QMouseEvent *event)
  {
  PosMouse = event->scenePosition();
  int N = Find_Object( event->scenePosition() );
  if ( N >= 0 )
    {
    CObjectRef * pO = pDataPtr->pObjRef[N];
    IBaseModel * pObj = IBaseModel::Find((char*)pO->ObjName);
    if ( pObj == NULL )
      return;
    ShowParams * pParams = new ShowParams( this, pObj );
    pParams->setWindowTitle( (char*)pO->ObjName );
    pParams->show();
    return;
    }
    Drug = true;
    PosDrug = event->scenePosition();
    ss.pSheme = this;
    // int k = 0;
    // char Txt[128];
    // sprintf ( Txt, "%3.0lf,%3.0lf", PosMouse.x(), PosMouse.y());
    // QMessageBox::question(this, "Clik", Txt, QMessageBox::Ok );
  }

 // void ShowContextMenu(const QPoint &pos)
 //  {
 //  QMenu *menu = new QMenu;
 //  menu->addAction("Remove Data Point", this,
 //                  SLOT(test_slot()));

 //  menu->exec(w->mapToGlobal(pos));
 //  }

  void ShowSheme::mouseReleaseEvent(QMouseEvent *event)
  {
  if (event->button()==Qt::RightButton)
    {
    pMainWnd->listshem.show();
    return;
    }
    QPointF Pos = event->scenePosition();
    Drug = false;
    ss.pSheme = this;
    // int k = 0;
    // char Txt[128];
    // sprintf ( Txt, "%3.0lf,%3.0lf", Pos.x(), Pos.y());
    // QMessageBox::question(this, "Clik", Txt, QMessageBox::Ok );
  }
  void ShowSheme::mouseMoveEvent(QMouseEvent *event)
  {
   if ( Drug )
    {
    QPointF Pos = event->pos();
    ss.pSheme = this;
    ss.ShiftX += Pos.x() - PosDrug.x();
    ss.ShiftY += Pos.y() - PosDrug.y();
    PosDrug = Pos;
    renderEmf(&ss,QString(""));
    }
   int N = Find_Object( event->scenePosition() );
    if ( N >= 0 )
      setCursor(Qt::PointingHandCursor);
    else
      setCursor(Qt::ArrowCursor);
    // int k = 0;
    // char Txt[128];
    // sprintf ( Txt, "%3.0lf,%3.0lf", Pos.x(), Pos.y());
    // QMessageBox::question(this, "Clik", Txt, QMessageBox::Ok );
  }

void ShowSheme::wheelEvent(QWheelEvent *event)
  {
  QPoint Pnt = event->angleDelta();
  const double k = 0.0001;
  ss.pSheme = this;
  ss.ScaleX *= 1. + k * Pnt.y();
  ss.ScaleY *= 1. + k * Pnt.y();
  renderEmf(&ss,QString(""));
  }

void ShowSheme::ShowSheme::updateTime()
  {
  ss.pSheme = this;
  ss.pExtPaint = ExtPaint;
//  m_imageLabel->update();
//  m_imageLabel->repaint();
  renderEmf(&ss,QString(""));
  }

void SpecKey ( int key );
void ShowSheme::keyPressEvent ( QKeyEvent * event )
  {
  int key = event->key() & 0xFFFF;
  LastPressedKey = key;
    switch ( key )
  {
    case 81: //Q
      QCoreApplication::exit(0);
    break;
    case 43: //+
      //m_painter->scale( 0.9, 0.9 );
        ss.pExtPaint = ExtPaint;
        ss.ScaleX *= 1.1;
        ss.ScaleY *= 1.1;
        renderEmf(&ss,QString(""));
    break;
    case 45: //-
      ss.pExtPaint = ExtPaint;
      ss.ScaleX *= 0.9;
      ss.ScaleY *= 0.9;
      renderEmf(&ss,QString(""));
    break;
    case 16: //home
      ss.ScaleX = 1.;
      ss.ScaleY = 1.;
      ss.ShiftX = 0.;
      ss.ShiftY = 0.;
      renderEmf(&ss,QString(""));
    break;
    case 49: // 1
      SpecKey ( key );
    break;
    case 50: // 2
      SpecKey ( key );
      break;
    case 51: // 3
      SpecKey ( key );
      break;
    case 52: // 4
      SpecKey ( key );
      break;
    case 53: // 5
      SpecKey ( key );
      break;
    case 54: // 6
      SpecKey ( key );
      break;
    case 55: // 7
      SpecKey ( key );
      break;
    case 56: // 8
      SpecKey ( key );
      break;
    case 57: // 9
      SpecKey ( key );
      break;
  }

//  char Txt[128];
//  sprintf ( Txt, "%d", key);
//  QMessageBox::question(this, "Clik", Txt, QMessageBox::Ok );
  }

void ShowSheme::SetCrd( const char * _crd )
  {
  if ( crd )
      return;
  crd = _crd;
  QFile ReadCrd ( crd );
  if (!ReadCrd.open(QIODevice::ReadOnly| QIODevice::Unbuffered))
    return;
  int nObj = 0;
  int S = ReadCrd.size();
  char * txt = new char[S];
  ReadCrd.read( txt, S );
  ReadCrd.close();
  KKK();
  char * P = txt;
  ObjCrd Crd[MAX_OBJ];
  while ( P - txt < S )
    {
    char * E = P;
    while ( *E != 0x0a ) E++;
    *E = 0;
    int L = ChatToUTF8( Crd[nObj].ObjName, 64, P );
    if ( strstr ( Crd[nObj].ObjName, "Факельный"))
      KKK();
    P = E + 1; E = P;
    while ( *E != 0x0a ) E++;
    P = E + 1; E = P;
    while ( *E != 0x0a ) E++;
    P = E + 1; E = P;
    while ( *P == ' ' ) P++;
    Crd[nObj].L = atof ( P );
    while ( *P != ' ' ) P++;
    while ( *P == ' ' ) P++;
    Crd[nObj].B = atof ( P );
    while ( *P != ' ' ) P++;
    while ( *P == ' ' ) P++;
    Crd[nObj].R = atof ( P );
    while ( *P != ' ' ) P++;
    while ( *P == ' ' ) P++;
    Crd[nObj].T = atof ( P );
    while ( *E != 0x0a ) E++;
    P = E + 1;
//
    if ( fabs ( Crd[nObj].B - -62.95001) < 0.001 )
      {
      Etalon.setLeft( Crd[nObj].L );
      Etalon.setTop( Crd[nObj].T );
      Etalon.setRight( Crd[nObj].R );
      Etalon.setBottom( Crd[nObj].B );
      }
//    std::sscanf( Buf, " %lf %lf %lf %lf", Crd[nObj].L, Crd[nObj].B, Crd[nObj].R, Crd[nObj].T );
    nObj++;
    }
//
  nObj = 427;
  kCrd = nObj;
  pCrd = new ObjCrd[nObj];
  memmove ( pCrd, Crd, nObj * sizeof ( ObjCrd ));
  }
void ShowSheme::paintEvent(QPaintEvent *event)
  {
  }

bool ShowSheme::OnScheme (LPCTSTR ObjScheme )
  {
  int L = ObjScheme ? lstrlen (ObjScheme) : 0;
  if ( L <= 1 )
    return false;
  if ( ObjScheme[L-1] == ';' )
      L--;
//  if ( L != m_cName.GetLength () )
//    return FALSE;
  return (strncmp ( FileName, ObjScheme, L ) == 0);
  }

void ShowSheme::Draw( QPainter * pP )
  {
//  QPen pen(Qt::yellow, 1, Qt::SolidLine);
//  pP->setPen( pen );
  pMainWnd->DrawData.pCol = NULL;
  pMainWnd->DrawData.hWnd = NULL;
  pMainWnd->DrawData.ID_Dial = 0;
  pMainWnd->DrawData.nModelID = 0;
  pMainWnd->DrawData.hDC.pixmap = ss.pixmap;

  pCollector = &pMainWnd->DrawData.hDC;
//

  bool D = true;
  if ( pDataPtr )
  for( int n = 0; n < pDataPtr->kObjRef; n++)
    {
    CObjectRef * pO = pDataPtr->pObjRef[n];
    if ( !OnScheme ((LPCTSTR)pO->Shema) )
      continue;
    if ( pO->pObject == NULL )
      continue;
    if ( pO->pObject->ModelFlags & 0x80 )
      continue;
//    if ( pO->pObject->NeedDrawBefore() != bBefore )
//      continue;
    double L, T, R, B;
    Model_To_Emf ( L, T, R, B, pO );
//    L += 3;
    R += 10;
//    T += 0;
    B += 5;
    pMainWnd->DrawData.ObjName = pO->pObject->ObjName;
    pMainWnd->DrawData.ShemaName = pO->Shema;
    pMainWnd->DrawData.rc.left = L;
    pMainWnd->DrawData.rc.right = R;
    pMainWnd->DrawData.rc.top = T;
    pMainWnd->DrawData.rc.bottom = B;
    pMainWnd->DrawData.hDC.Init ( pP );
    pMainWnd->DrawData.hDC.Sx = ss.ShiftX;
    pMainWnd->DrawData.hDC.Sy = ss.ShiftY;
    pMainWnd->DrawData.hDC.M = ss.ScaleX;
     // pP->drawLine( L, T, R, T);
     // pP->drawLine( R, T, R, B);
     // pP->drawLine( L, B, L, T );
     // pP->drawLine( R, B, L, B);
     pO->pObject->DrawObj ( &pMainWnd->DrawData );
 //   if ( strstr ( pO->pObject->ObjName, "A-3001A"))
 //     KKK();
     // if ( D )
     // {
     //   D = false;
     // pMainWnd->DrawData.rc.setLeft( 0 );
     // pMainWnd->DrawData.rc.setRight( 100 );
     // pMainWnd->DrawData.rc.setTop( 0 );
     // pMainWnd->DrawData.rc.setBottom( 100 );
     // TransparentStd( pMainWnd->DrawData.hDC, pMainWnd->DrawData.rc, RGB ( 0, 255, 0 ));
     //}

#if 0
    DrawData.ObjName = pO->ObjName;
    DrawData.rc.left = (int)(pO->L * m_sPosScm.dZoom) + dx;
    DrawData.rc.right = (int)(pO->R * m_sPosScm.dZoom) + dx;
    DrawData.rc.top = (int)(pO->T * m_sPosScm.dZoom) + dy;
    DrawData.rc.bottom = (int)(pO->B * m_sPosScm.dZoom) + dy;
    DrawObject (pO->pObject, &DrawData);
    if ( pO->Selected )
    {
      CRect rc = DrawData.rc;
      int s = 10;
      int x = rc.left-s;
      int y = rc.top-s;
      int w = abs ( rc.right - rc.left ) + 2*s;
      int h = ( rc.bottom - rc.top ) + 2*s;
      CDC MemSelect;
      CBitmap bmSelect;
      MemSelect.CreateCompatibleDC( pDC );
      bmSelect.CreateCompatibleBitmap( pDC, w, h );
      CBitmap* pBm = MemSelect.SelectObject(&bmSelect);
      CRect rect ( 0, 0, w, h );
      MemSelect.FillRect(rect, &CBrush(RGB(255,255,0)));
      BLENDFUNCTION blend = {AC_SRC_OVER, 0, 120, 0};
      AlphaBlend( pDC->m_hDC, x, y, w, h,
                 MemSelect.m_hDC,0,0,w,h,blend);
      MemSelect.SelectObject(pBm);
#endif
    }
  }
void ShowSheme::closeEvent(QCloseEvent * event)
  {
  Close();
  }

void ShowSheme::resizeEvent(QResizeEvent *event)
  {
  WinRect = geometry();
  change = true;
  }

void ShowSheme::moveEvent(QMoveEvent *event)
  {
  WinRect = geometry();
  change = true;
  }

// void ListEMF::mousePressEvent(QMouseEvent *event)
//   {
//   // QListView :: mousePressEvent(event);
//   // QModelIndex index = currentIndex();
//   // QString itemText = index.data(Qt::DisplayRole).toString();
//   // emit ShowSheme ( itemText.toStdString().c_str());
//   KKK();
//   // QModelIndexList Sel = selectedIndexes();
//   // if ( !Sel.isEmpty())
//   //   {
//   //   QModelIndex Ind = Sel[0];
//   //   KKK();
//   //   }
//   }
