#pragma once
#include "UniBuffer.h"
//
#undef  ATTR32
#define ATTR32 _WIN32_FILE_ATTRIBUTE_DATA
class CArrayObj;
//
struct SItemCrd
  {
  HCHAR m_nName;
  EDataTypes eType;
  int   m_nCount;
  HCHAR m_nFile[4];
  int   m_nNumb[4];
  };
//
class CTexData
  {
  protected:
  static const char m_chVer;
  struct SUniItem : public SUniBuffer
    {
    SUniItem(SUniChar& n)
    : SUniBuffer(sizeof(SItemCrd)), name(n){};
    //
    SUniChar& name;
    SItemCrd& Item(int n)  {SItemCrd* lst = (SItemCrd*)m_szBuffer;return lst[n];}
    };
  SUniItem mItem;
  SUniChar mChar;
  public:
    CTexData();
	 ~CTexData();
    //
    char* Name(int n)
      {
      SItemCrd& obj = mItem.Item(n);
      return mChar.Name(obj.m_nName);
      }
    char* File(int n,int m)
      {
      SItemCrd& obj = mItem.Item(n);
      return mChar.Name(obj.m_nFile[m]);
      }
    //
    CString LoadList( CArrayObj& xml );
    CString ReadList( CArrayObj& xml );
    //
  protected:
    void ClearAll();
    bool ReadBin( char* ptr, ATTR32 attr );
    void SaveBin( char* bin, ATTR32 attr, CArrayObj& xml );
    void AddItems( struct SUniTags& mTags, HCHAR nFile, SUniBuffer& mOrd );
  };
