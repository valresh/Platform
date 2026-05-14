#include "showsheme.h"
#include <QMessageBox>
#include <QChar>


static double dx_0 = 0;
static double dy_0 = 0;
static double M_0 = 1.001;

static double dx_p = 0.;
static double dy_p = -35.;
static double M_p = 1.;

void P_to_P( SS * pSS, QPointF Pos, double & X, double & Y )
  {
  double M = pSS->ScaleX;
  X = ( Pos.x() * M_p + dx_p ) / M - pSS->ShiftX;
  Y = ( Pos.y() * M_p + dy_p ) / M - pSS->ShiftY;
  }

#define EPS					(double)50.

bool IsNearPipe (CPipeRef *pP, double Xp, double Yp)
  {
      CSegment * pS = pP->pFirstSeg;
      double X = Xp;
      double Y = Yp;
      while ( pS )
      {
          int kPnt = pS->kPnt;
          double Xe = pS->XY[0];
          double Ye = pS->XY[1];
          double Xb, Yb;
          for ( int n = 1; n < kPnt; n++ )
          {
              Xb = Xe;
              Yb = Ye;
              Xe = pS->XY[2*n];
              Ye = pS->XY[2*n+1];
              double dXs = Xe - Xb;
              double dYs = Ye - Yb;
              double dX = X - Xb;
              double dY = Y - Yb;
              double C = dX * dXs + dY * dYs;
              double Z = dXs * dXs + dYs * dYs;
              if ( C < 0. )
                  continue;
              if ( C > Z )
                  continue;
              double t = C / Z;
              double dXt = dX - dXs * t;
              double dYt = dY - dYs * t;
              double D = dXt * dXt + dYt * dYt;
              if ( D < EPS )
                  return true;
          }
          pS = pS->pNext;
      }
      return false;
  }

int ShowSheme::Find_Pipe ( QPointF Pos )
  {
      // В координатах EMF
      double M = ss.ScaleX;
      double X = ( Pos.x() * M_p + dx_p ) / M - ss.ShiftX;
      double Y = ( Pos.y() * M_p + dy_p ) / M - ss.ShiftY;
      double Xm = ( X - dx_0 ) / M_0;
      double Ym = ( Y - dy_0 ) / M_0;
      CPipeRef *pP_Sel = NULL;
      for( int n = 0; n < pDataPtr->kPipeRef; n++ )
      {
          CPipeRef *pP = pDataPtr->pPipeRef[n];
          if ( !OnScheme ((LPCTSTR)pP->Shema) )
              continue;
          if ( IsNearPipe (pP, Xm, Ym) )
          {
              return n;
          }
      }
      return -1;
  }
extern double Ms;
extern double Sxs;
extern double Sys;


int ShowSheme::Find_Object ( QPointF Pos, IBaseModel * pObj[]  )
  {
// В координатах EMF
  double M = ss.ScaleX;
  double X = ( Pos.x() * M_p + dx_p ) / M - ss.ShiftX;
  double Y = ( Pos.y() * M_p + dy_p ) / M - ss.ShiftY;
  double Xm = ( X - dx_0 ) / M_0;
  double Ym = ( Y - dy_0 ) / M_0;
  KKK();
  int k = 0;
  for( int n = 0; n < kCrd; n++)
  {
      ObjCrd * pO = &pCrd[n];
      if ( strstr ( pO->ObjName, "Сырье печи к" ))
      {
          KKK();
      }
      double L = pO->L * Ms + Sxs, T = -pO->T * Ms + Sys, R = pO->R * Ms + Sxs, B = -pO->B * Ms + Sys;
      if ( Xm < L )
          continue;
      if ( Xm > R )
          continue;
      if ( Ym < T )
          continue;
      if ( Ym > B )
          continue;
      IBaseModel * O = IBaseModel::Find( pO->ObjName );
      if ( O )
          pObj[k++] = O;
  }
/*
  for( int n = 0; n < pDataPtr->kObjRef; n++)
  {
    CObjectRef * pO = pDataPtr->pObjRef[n];
    if ( strstr ( pO->pObject->ObjName, "Сырье печи к змеевикам"))
      KKK();
    if ( !OnScheme ((LPCTSTR)pO->Shema) )
      continue;
    if ( pO->pObject == NULL )
      continue;
    if ( pO->pObject->ModelFlags & 0x80 )
      continue;
    if ( Xm < pO->L )
      continue;
    if ( Xm > pO->R )
      continue;
    if ( Ym < pO->T )
      continue;
    if ( Ym > pO->B )
      continue;
    KKK();
  //  QMessageBox::question(this, "Объект", pO->ObjName.Str, QMessageBox::Ok );
    return n;
  }
*/
  return k;
  }

  void ShowSheme::Model_To_Emf ( double & L, double & T, double & R, double & B, QRectF & Model )
  {
  L = ( Model.left() * M_0 + dx_0 );
  T = ( Model.top() * M_0 + dy_0 );
  R = ( Model.right() * M_0 + dx_0 );
  B = ( Model.bottom() * M_0 + dy_0 );
  }


void ShowSheme::Model_To_Emf ( double & L, double & T, double & R, double & B, ObjCrd & Obj )
  {
  L = ( Obj.L * M_0 + dx_0 );
  T = ( Obj.T * M_0 + dy_0 );
  R = ( Obj.R * M_0 + dx_0 );
  B = ( Obj.B * M_0 + dy_0 );
  }
void ShowSheme::Model_To_Emf ( double & L, double & T, double & R, double & B, CObjectRef * pO )
  {
    L = ( pO->L * M_0 + dx_0 );
    T = ( pO->T * M_0 + dy_0 );
    R = ( pO->R * M_0 + dx_0 );
    B = ( pO->B * M_0 + dy_0 );
  }
#if 1
 void ShowSheme::SpecKey ( int key )
    {
    switch ( key )
    {
      case 49: // 1
        break;
      case 50: // 2
        dy_p += 1.;
        renderEmf(&ss,QString(""));
        break;
      case 51: // 3
        M_p *= 0.99;
        renderEmf(&ss,QString(""));
        break;
      case 52: // 4
        dx_p -= 1.;
        renderEmf(&ss,QString(""));
        break;
      case 53: // 5
        break;
      case 54: // 6
        dx_p += 1.;
        renderEmf(&ss,QString(""));
        break;
      case 55: // 7
        break;
      case 56: // 8
        dy_p -= 1.;
        renderEmf(&ss,QString(""));
        break;
      case 57: // 9
        M_p *= 1.01;
        renderEmf(&ss,QString(""));
        break;
      }
    }
#endif
#if 0
  void ShowSheme::SpecKey ( int key )
  {
    switch ( key )
    {
      case 49: // 1
        break;
      case 50: // 2
        dy_0 += 1.;
        renderEmf(&ss,QString(""));
        break;
      case 51: // 3
        M_0 *= 0.999;
        renderEmf(&ss,QString(""));
        break;
      case 52: // 4
        dx_0 -= 1.;
        renderEmf(&ss,QString(""));
        break;
      case 53: // 5
        break;
      case 54: // 6
        dx_0 += 1.;
        renderEmf(&ss,QString(""));
        break;
      case 55: // 7
        break;
      case 56: // 8
        dy_0 -= 1.;
        renderEmf(&ss,QString(""));
        break;
      case 57: // 9
        M_0 *= 1.001;
        renderEmf(&ss,QString(""));
        break;
    }
  }
#endif
      int ChatToUTF8( char * To, int MaxTo, char * From, bool Prefix )
        {
        unsigned char * F = (unsigned char *)From;
        int p = 0;
        if ( Prefix )
        {
        To[p++] = 0xEF;
        To[p++] = 0xBB;
        To[p++] = 0xBF;
        }
        while ( 1 )
          {
          if ( p + 3 > MaxTo )
            break;
          int s = *F++;
          if ( s == 0 )
            break;
          if ( s < 0x80 )
            {
            To[p++] = s;
            continue;
            }
          if ( s < 0x7FF )
            {
            if ( s >= 0xC0 )
              s += 0x410 - 0xC0;
            // требуется 2 символа
            int r2 = 0b10000000 | ( s & 0x3F );
            s = s >> 6;
            int r1 = 0b11000000 | ( s & 0x1F );
            To[p++] = r1;
            To[p++] = r2;
            continue;
            }
          assert( 0 ); // больше 2-х не будем
          }
        To[p] = 0;
        return p;
        }

      void Test()
      {
      //QString Src = "ййиллquyt";
      // char * pDest = NULL;
      // std::u16string Q = Src.toStdU16String();
      // size_t Res = UnicodeToUTF8( &pDest, Q, (size_t)Src.size());
      }
