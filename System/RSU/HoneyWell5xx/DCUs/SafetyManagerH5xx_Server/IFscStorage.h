#pragma once
#include <UniBufferT.h>
#include <FSC_Info.h>
#include "FscTypes.h"

#include <InlineGDI.h>

struct IFscStorage
{
  struct SVarInfo
  {
    SDotValue *val;
    char szName[12*4];
  };
  virtual UINT AddFscBuff( void* Void, int nSize ) = 0;
  virtual UINT AddFscDots( SDotValue* add ) = 0;
  virtual SDotValue* DotV( UINT n ) = 0;
  virtual int FindDbfTag( LPCSTR ptr, CFSC_Info::ePointType ept, CFSC_Info::eType *pPntType = NULL ) = 0;
  virtual LPCSTR DbfName( int n, int m ) = 0;
  virtual void* Data( UINT n ) = 0;
  virtual UINT ItemLink( UINT nLink ) = 0;
  virtual UINT NextLink( UINT nLink ) = 0;
  virtual UINT PlusLink( UINT nLink, UINT nData ) = 0;
  virtual void* VoidInOut( int nDbf, EValueType eType, bool bIn ) = 0;
  virtual void* CreateSharedIO( int nDbf, EValueType eType, unsigned char mT, LFscBase::typeB mB ) = 0;
  virtual SPointIn* PointIn( UINT n ) = 0;
  virtual SPointOut* PointOut( UINT n ) = 0;
  virtual int GetVars( LFscBase *obj, SVarInfo *pvi, int cVI, LPCSTR *ppPntName ) = 0;
  //
  virtual LFscFile* FscFileList( UINT& nCount ) = 0;
  virtual LFscFile& FscFileItem( UINT nFile ) = 0;
#ifdef _WIN32
  virtual LFscBase* FscInRect( UINT nFile, POINT point ) = 0;
#endif
  virtual void* FscGiveData( UINT n ) = 0;
  virtual LFscBase* GetItem( UINT nItem ) = 0;
  //
  //virtual bool FindPoint( LPCSTR pszName, LPSTR pszIOtype, SDotValue** ppV, void ***ppIO ) = 0;
  //
#ifdef _WIN32
  virtual void FscFileDraw( CMyFont& font, UINT nFile, UINT nSelect, bool bNumber ) = 0;
#endif
  /*virtual bool RegisterACSCaller (HWND hCaller, bool bRegister) = 0;
  virtual void PopulateACSTree (HWND hTreeCtrl) = 0;
  virtual RECT InitDrawACS (HWND hCaller, HWND hTreeCtrl, HTREEITEM hItem, HDC hDC, DWORD& dwProp) = 0;
  virtual void DrawACS (HWND hCaller, HDC hDC) = 0;
  virtual bool OnACSMouseEvent (HWND hCaller, ACSMouseAction& sAction, UINT nFlags, POINT pt) = 0;
  virtual bool SerializeACSColor (int nColor, ACSColor& sColor, bool bSave) = 0;*/
  //
  virtual double GetTimeAccum() = 0;
  virtual __int64 GetNSteps() = 0;
};
