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
#include <QStatusBar>


double Ms = 3.771;
double Sxs = 4;
double Sys = 4;

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
#include <QToolBar>

ShowSheme::ShowSheme( QWidget *parent, const char * File, const char * _Select  )
    : EMF( parent, File )
{
    Type = Shema;
    Select = _Select;
    ss.pSheme = this;
    //  QFileInfo file ( Path );
    FileName = File;//STR(file.baseName());
    setWindowTitle( FileName.Str );
    Char<1024> Crd;
    Crd.Prt( "%sDATA/SCHEME/%s.crd", PROJECT_ROOT, FileName );
    SetCrd( Crd );
    setMinimumSize( 1500, 1000 );
    Drug = false;
    pMainHydro->SetData ( 1019, (void*)&pDataPtr);
    toolbar = addToolBar(tr("Control"));
 //
    QAction * Pause = new QAction(tr("&Пауза"), this);
    Pause->setIcon( QIcon(":/pause.png"));
    Pause->setStatusTip(tr("Пуск"));
    connect(Pause, &QAction::triggered, this, &ShowSheme::tbPause);
 //
    QAction * Cont = new QAction(tr("&Продолжить"), this);
    Cont->setIcon( QIcon(":/next_step.png"));
    Cont->setStatusTip(tr("Продолжить"));
    connect(Cont, &QAction::triggered, this, &ShowSheme::tbCont);
//
    QAction * SaveParams = new QAction(tr("&Сохранить параметры"), this);
    SaveParams->setIcon( QIcon(":/diskette_down.png"));
    SaveParams->setStatusTip(tr("Пуск"));
    connect(SaveParams, &QAction::triggered, this, &ShowSheme::tbSaveParams);
//
    QAction * SaveState = new QAction(tr("&Сохранить состояние"), this);
    SaveState->setIcon( QIcon(":/diskette_reload.png"));
    SaveState->setStatusTip(tr("Пуск"));
    connect(SaveState, &QAction::triggered, this, &ShowSheme::tbSaveState);
    //
    QAction * SaveAll = new QAction(tr("&Сохранить все"), this);
    SaveAll->setIcon( QIcon(":/diskette.png"));
    SaveAll->setStatusTip(tr("Пуск"));
    connect(SaveAll, &QAction::triggered, this, &ShowSheme::tbSaveAll);
    //
    toolbar->addAction ( Pause );
    toolbar->addAction ( Cont );
    toolbar->addSeparator();
    toolbar->addAction ( SaveParams );
    toolbar->addAction ( SaveState );
    toolbar->addAction ( SaveAll );
    setWindowIcon(QIcon(":/List.png"));
//    connect(this, &ShowSheme::on_Start, pMainWnd, &MainWindow::on_Start_clicked);
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
extern char * GoTo;
void ShowSheme::mousePressEvent(QMouseEvent *event)
{
    PosMouse = event->scenePosition();
    // if ( m_pDataPtr )
    // {
    //     for ( int n = 0; n < m_cDrawObj.L; n++ )
    //     {
    //         CDrawObject & Obj = m_cDrawObj[n];
    //         if ( Obj.Selected )
    //             m_nWaitSec = DEF_WAIT_REFRESH;
    //         Obj.Selected = false;
    //     }
    //     for(DWORD n = 0; n < m_pDataPtr->kPipeRef; n++ )
    //     {
    //         CPipeRef * pP = m_pDataPtr->pPipeRef[n];
    //         if ( !IsOnScheme ((LPCTSTR)pP->Shema) )
    //             continue;
    //         pP->Selected = false;
    //     }
    // }
    IBaseModel * pShemaObj[16];
    int k = Find_Object( event->scenePosition(), pShemaObj );
    if ( k > 0 )
      {
      if ( k == 1 && GoTo == NULL )
        {
        IBaseModel * pO = pShemaObj[0];
        ShowParams * pParams = new ShowParams( this, pO );
        char Title[256];
        Sprintf( Title, "%s(%s)",  pO->ObjName.Str, pO->Model.Str);
        pParams->setWindowTitle( Title );
        pParams->move(PosMouse.x(), PosMouse.y());
        pParams->show();
        return;
        }
        //   CObjectRef * pO = pDataPtr->pObjRef[N];
        //   IBaseModel * pObj = IBaseModel::Find((char*)pO->ObjName);
        //   if ( pObj == NULL )
        //     return;
      if ( k > 1 || GoTo )
        {
        QMenu menu;
        QAction * actions[100];
        int p = 0;
        for ( int n = 0; n < k; n++ )
          {
          actions[n] = menu.addAction(pShemaObj[n]->ObjName.Str);
          }
        if ( GoTo )
          {
          menu.addSeparator();
          menu.addAction( GoTo );
          }
        QAction * selectedItem = menu.exec(event->globalPosition().toPoint());
        if ( selectedItem )
          {
          for ( int n = 0; n < k; n++ )
            {
            if ( selectedItem == actions[n] )
              {
              IBaseModel * pO = pShemaObj[n];
              ShowParams * pParams = new ShowParams( this, pO );
              char Title[256];
              Sprintf( Title, "%s(%s)",  pO->ObjName.Str, pO->Model.Str);
              pParams->setWindowTitle( Title );
              pParams->move(PosMouse.x(), PosMouse.y());
              pParams->show();
              return;
              }
            }
// GoTo
          FileName = GoTo;
          Char<1024> Crd;
          Crd.Prt( "%sDATA/SCHEME/%s.crd", PROJECT_ROOT, FileName );
          crd = NULL;
          SetCrd( Crd );
          Select = GoTo;
          setWindowTitle( GoTo );
//          pMainHydro->SetData ( 1019, (void*)&pDataPtr);
          static Char<1024>Go;
          Go.Prt( "%sDATA/SCHEME/%s.emf", PROJECT_ROOT, GoTo );
          ss.NewPath = Go;
          KKK();
          }
        else
          KKK();
        }
      }
    int N = Find_Pipe( event->scenePosition() );
    if ( N >= 0 )
    {
        CPipeRef *pP = pDataPtr->pPipeRef[N];
        IBaseModel * pObj = IBaseModel::Find((char*)pP->PipeName);
        if ( pObj == NULL )
            return;
        ShowParams * pParams = new ShowParams( this, pObj );
        char Title[256];
        Sprintf( Title, "%s(%s)",  pObj->ObjName.Str, pObj->Model.Str);
        pParams->setWindowTitle( Title );
        pParams->move(PosMouse.x(), PosMouse.y());
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
    IBaseModel * pShemaObj[16];
    int k = Find_Object( event->scenePosition(), pShemaObj );
    if ( k > 0 )
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
    Char<128>Txt;
    Txt.Prt ( "%d", pSys->m_nStep );
    statusBar()->showMessage( Txt.Str );
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
    case 'S': //Q
        Pause = true;
        break;
    case 'C': //Q
        Pause = false;
        break;
    case 43: //+
        if ( pMainWnd->TS())
            Ms *= 1.01;
        else
        {
            ss.pExtPaint = ExtPaint;
            ss.ScaleX *= 1.1;
            ss.ScaleY *= 1.1;
            renderEmf(&ss,QString(""));
        }
        break;
    case 45: //-
        if ( pMainWnd->TS())
            Ms /= 1.01;
        else
        {
            ss.pExtPaint = ExtPaint;
            ss.ScaleX *= 0.9;
            ss.ScaleY *= 0.9;
            renderEmf(&ss,QString(""));
        }
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
        if ( pMainWnd->TS())
            Sys++;
        else
            SpecKey ( key );
        break;
    case 51: // 3
        SpecKey ( key );
        break;
    case 52: // 4
        if ( pMainWnd->TS())
            Sxs--;
        else
            SpecKey ( key );
        break;
    case 53: // 5
        SpecKey ( key );
        break;
    case 54: // 6
        //      SpecKey ( key );
        Sxs++;
        break;
    case 55: // 7
        SpecKey ( key );
        break;
    case 56: // 8
        if ( pMainWnd->TS())
            Sys--;
        else
            SpecKey ( key );
        break;
    case 57: // 9
        SpecKey ( key );
        break;
    }
    if ( pMainWnd->TS())
        renderEmf(&ss,QString(""));

    //  char Txt[128];
    //  sprintf ( Txt, "%d", key);
    //  QMessageBox::question(this, "Clik", Txt, QMessageBox::Ok );
}

#define END \
{\
        int L = strlen ( Str ) - 1;\
        while ( Str[L] == 0x0a || Str[L] == 0x0d ) Str[L--] = 0;\
}

void ShowSheme::SetCrd( const char * _crd )
{
    crd = _crd;
    FILE * F = fopen ( crd, "rt" );
    if ( F == NULL )
        return;
    ObjCrd Crd[MAX_OBJ];
    char Str[1024];
    int nObj = 0;
    while ( fgets ( Str, 1024, F ) )
      {
      END
      Crd[nObj].ObjName = Str;
      Crd[nObj].pObj = IBaseModel::Find( Str );
      fgets ( Str, 1024, F );
      END
      Crd[nObj].GoTo = Str;
      fgets ( Str, 1024, F );
      END
      fgets ( Str, 1024, F );
      END
      char * P = Str;
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
      nObj++;
      }
    fclose ( F );
    //
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
  char OS[1024];
  strcpy_s( OS, 1024, ObjScheme );
  char * P = OS;
  while ( 1 )
    {
      char * E = strchr( P, ';' );
        if ( E )
          *E = 0;
        if ( strcmp ( FileName, P ) == 0 )
          return true;
        if ( E == NULL )
          break;
        P = E + 1;
    }
  return false;
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
  if ( pMainWnd->TS())
    {
    pP->save();
    QPen Pen(Qt::SolidLine);
    Pen.setWidth( 1 );
    Pen.setColor( RGB ( 255, 255, 0 ));
    pP->setPen( Pen );
    for( int n = 0; n < kCrd; n++)
      {
      ObjCrd * pO = &pCrd[n];
      double L = pO->L * Ms + Sxs, T = -pO->T * Ms + Sys, R = pO->R * Ms + Sxs, B = -pO->B * Ms + Sys;
      pP->drawLine( L, T, R, T);
      pP->drawLine( R, T, R, B);
      pP->drawLine( L, B, L, T );
      pP->drawLine( R, B, L, B);
      }
    pP->restore();
    }
    pCollector = &pMainWnd->DrawData.hDC;
    //
  bool D = true;
  for( int n = 0; n < kCrd; n++)
    {
    ObjCrd * pO = &pCrd[n];
    if ( pO == NULL || pO->pObj == NULL )
        continue;
    double L = pO->L * Ms + Sxs, T = -pO->T * Ms + Sys, R = pO->R * Ms + Sxs, B = -pO->B * Ms + Sys;
    pMainWnd->DrawData.ObjName = pO->pObj->ObjName;
    pMainWnd->DrawData.ShemaName = FileName;
    pMainWnd->DrawData.rc.left = L;
    pMainWnd->DrawData.rc.right = R;
    pMainWnd->DrawData.rc.top = T;
    pMainWnd->DrawData.rc.bottom = B;
    pMainWnd->DrawData.hDC.Init ( pP );
    pMainWnd->DrawData.hDC.Sx = ss.ShiftX;
    pMainWnd->DrawData.hDC.Sy = ss.ShiftY;
    pMainWnd->DrawData.hDC.M = ss.ScaleX;
    pO->pObj->DrawObj ( &pMainWnd->DrawData );
    if ( Select &&  strcmp ( Select, pO->pObj->ObjName.Str) == 0 )
      {
      CRect rc = pMainWnd->DrawData.rc;
      int s = 10;
      int x = rc.left-s;
      int y = rc.top-s;
      int w = abs ( rc.right - rc.left ) + 2*s;
      int h = ( rc.bottom - rc.top ) + 2*s;
      pP->fillRect( x, y, w, h,QBrush(QColor(255,255,0,80)));
      }
    }
/*
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

            if ( Select &&  strcmp ( Select, pO->pObject->ObjName.Str) == 0 )
            {
                CRect rc = pMainWnd->DrawData.rc;
                int s = 10;
                int x = rc.left-s;
                int y = rc.top-s;
                int w = abs ( rc.right - rc.left ) + 2*s;
                int h = ( rc.bottom - rc.top ) + 2*s;
                pP->fillRect( x, y, w, h,QBrush(QColor(255,255,0,80)));
            }
        }
*/
}

void ShowSheme::tbPause()
{
  Pause = true;
}
void ShowSheme::tbCont()
{
  Pause = false;
}

extern bool SaveParams;
extern bool SaveState;

void ShowSheme::tbSaveParams()
{
  SaveParams = true;
}
void ShowSheme::tbSaveState()
{
  SaveState = true;

}
void ShowSheme::tbSaveAll()
{
  SaveParams = true;
  SaveState = true;

}

void ShowSheme::on_New_Shema_triggered()
  {
  pMainWnd->listshem.show();
  return;
  }


void ShowSheme::on_Exit_triggered()
  {
  QCoreApplication::exit(0);
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
