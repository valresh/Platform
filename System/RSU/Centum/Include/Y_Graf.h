#pragma once

#ifdef YGRAF_EXPORTS
#define YGRAF_API _EXP
#else
#define YGRAF_API _IMP
#endif


struct YGRAF_API CGrafParm
{
  double PV_min, PV_max, MV_min, MV_max;
  int Step;
  int kPnt;
  double * pPVdata;
  double * pSVdata;
  double * pMVdata;
  double * pP;
  double * pI;
  double * pD;
  HWND hWnd;
  CGrafParm()
  {
    PV_min = 0.;
    PV_max = 0.;
    MV_min = 0.;
    MV_max = 0.;
    Step = 1;
    kPnt = 300;
    pPVdata = NULL;
    pSVdata = NULL;
    pMVdata = NULL;
    pP = NULL;
    pI = NULL;
    pD = NULL;
    hWnd = NULL;
  };
};


bool YGRAF_API CreateWnd( char * Name, HWND hWndMain, HINSTANCE hRes, CGrafParm * pGP );
