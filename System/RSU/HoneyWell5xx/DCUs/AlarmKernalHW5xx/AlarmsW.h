#pragma once
#include <FileXML.h>
#include "AlarmsFile.h"
#include <TinyXmlReader.h>
#include <map>

typedef std::map<std::string,std::string> tKeyAlmGr_Desc;

struct SAlarmsW : public CFileXML
{
  SUniHoney mHoney;// Список тэгов     (THoneyWell)
  SUniMnemo mMnemo;// Список мнемосхем (TMnemoSxem)
  SUniIndex mIndex;// Индексы тэгов и мнемосхем (WORD)
  SUniCharT mChars;// Буфер, хранящий все имена
  SUniAlmGr mAlmGr;// Список аларменных групп (TAlmGr)
  //
  SAlarmsW()
    : mHoney(mIndex,mChars)
    , mMnemo(mIndex,mChars)
	, mAlmGr(mIndex,mChars)
  {
  }
  bool BuildAll();
  bool Parser();
  bool ReadAndParseAssoc_disp( LPCSTR pszCsv );
  void Parser(THoneyWell* item,char* ptr);
  void Parser(TMnemoSxem& sxem,UINT    N);
  void ParserAlmGr(THoneyWell* item,char* ptr);
  void ParserAlmGr(TAlmGr& agr,UINT N, tKeyAlmGr_Desc& AlmGrDesc);  
  bool BuildHoneywellList();
  void HIALM( CAlarmBase* pObj, BYTE nEHLN, BYTE nType );
  inline bool IsAlarm( UINT nType );
  void DATAACQ( BYTE& tp, BYTE nType );
  void PID    ( BYTE& tp, BYTE nType );
  void DEVCTL ( BYTE& tp, BYTE nType );
  void FLAG   ( BYTE& tp, BYTE nType );
  void DIGACQ ( BYTE& tp, BYTE nType );
};
