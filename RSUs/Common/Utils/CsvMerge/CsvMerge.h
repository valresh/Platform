// CsvMerge.h : main header file for the CsvMerge application
//
// #pragma once

// #ifndef __AFXWIN_H__
// 	#error "include 'stdafx.h' before including this file for PCH"
// #endif

#include "resource.h"       // main symbols
#include <string>
#include <map>
#include <list>
#include <vector>
#include <fstream>


// CCsvMergeApp:
// See CsvMerge.cpp for the implementation of this class
//

class CCsvMergeApp : public CWinApp
{
  typedef std::vector<std::string> tRowCont_;
  typedef std::map<std::string, tRowCont_> tNewPointsCont_;
  typedef std::list< std::pair<std::string, tRowCont_> > tOldPointCont_;

  tNewPointsCont_ m_yNew;
  tOldPointCont_ m_yOld;

  void ReadCsvs( LPCSTR pszY, tNewPointsCont_ &cont, int nCatCols2Key = 1 );
  void ReadCsvs( LPCSTR pszY, tOldPointCont_ &cont, int nCatCols2Key = 1 );
  void EmbeddedMerge();
  void SkolkoJ();
  void Uralkaliy139();
  void OutAsIs( std::ostream &out, tRowCont_ &cont );
  void OutFromNew( std::ostream &out, tRowCont_ &contOld, tRowCont_ &contNew, int firstFromNew );
  void MergeFromNew( std::ostream &out, tRowCont_ &contOld, tRowCont_ &contNew, int firstFromNew );
  void MergeFromNewColTest( std::ostream &out, tRowCont_ &contOld, tRowCont_ &contNew, int firstFromNew, int coltest = 2 );
  template<size_t _S>
  void OutFromNew( std::ostream &out, tRowCont_ &contOld, tRowCont_ &contNew, int (&indexFromNew)[_S] );

  void AppendDataFromOld( LPCSTR szM, int indexFrom );
  void MergeAndSeparate( LPCSTR szM, int indexTo );//формирует раздельные списки: совпавшие, новые, ненайденные
  tOldPointCont_::iterator OutCurAndTryNextOld( tOldPointCont_::iterator itOld, tOldPointCont_::iterator end, tOldPointCont_ &deletedPoints, std::ofstream &out, int indexTo );
  void UPG118to119( LPCSTR szM );
  void PAZ6169( LPCSTR szM );
  void UPS70( LPCSTR szM );

  void TriDvConnect();
  void TriDvReadDV( LPCSTR psz, tNewPointsCont_ &cont );
  void TriDvReadOldTri( LPCSTR psz, tOldPointCont_ &cont );
  
  void DvExtructSOE();
  void ReadSOE( LPCSTR pszY, tNewPointsCont_ &cont );
public:
	CCsvMergeApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCsvMergeApp theApp;
