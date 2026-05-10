#ifndef BASEHTML_H
#define BASEHTML_H

//custom includes - not imported
#include <BaseType.h>



#include <HoneyMnemoExtend.h>
#include <FormatScn.h>


#include <crosswindows.h>
#include <linuxTypes/CString.h>
//

class CHTML;
class CTag;
//
//
struct O_VAL
{
 EValueType eType;
 union
  {
   char ostr[512];
   bool obool;
   int  oint;
   double odouble;
  };
  O_VAL() {eType = enumValueUnk; odouble = 0;}
};
//
struct SBaseHTML
  {
  SBaseHTML()
  : m_nKolvo(0)
  , m_nPlace(0)
    {
    };
  int m_nKolvo;// Количество параметров
  int m_nPlace;// Начало в буфере данных
  //
  const char* GetName (CHTML& html,int n);
  const char* GetValue(CHTML& html,int n);
  const char* GetName (CHTML& html);
  const char* GetValue(CHTML& html);
  int   AddTexts(CHTML& html, const char* text, int dl = 0 );
  int   FindItem(CHTML& html, const char* name );
  void  AddItems(CHTML& html, int nCount, STegScn* teg );
  void  SetLastName ( CHTML& html, const char* text );
  void  SetLastValue( CHTML& html, const char* text );
  //
  bool IsYou( CHTML& html, const char* szID )
    {
    return lstrcmpi( GetName(html), szID ) == 0;
    }
  };
//
struct SBaseDSD : public SBaseHTML
  {
  SBaseDSD()
  : m_nID(-1)
  , m_nCountDSD(0)
  , m_nPlaceDSD(0)
  , m_nCountPip(0)
  , m_nPlacePip(0)
    {
    };
  int m_nID;
  //
  int m_nCountDSD;// Количество дочерних продуктов
  int m_nPlaceDSD;// Начало продуктов в буфере данных
  //
  int m_nCountPip;// Количество используемых объектов
  int m_nPlacePip;// Началообъектов в буфере данных
  //
  const char* PropNameValue( CHTML& html, const char* text );
  void ActionDSD( HWND hWnd, CHTML& html );
  const char* PointName(CHTML& html);
  const char* CommaPointName(CHTML& html);
  //
  void String( CHTML& html, cross::CString& str, int N );
  const char* GetStr(CHTML& html,const char* name);
  int         GetInt(CHTML& html,const char* name);
  UINT Pipe(CHTML& html, char* pparam = NULL, CTag* pTag = NULL);
  bool IsOK( CHTML& html );
  cross::SParamValueH* Value( CHTML& html, int n = 0 );
  bool IsPushButtonAction0(CHTML& html);
  bool PushAction0( HWND hWnd, CHTML& html, LPCTSTR szParam, std::string strValue="", cross::SParamValueH* pVal=NULL);
  private:
    bool PushButtons( HWND hWnd, CHTML& html );
    bool PushAction1( HWND hWnd, CHTML& html, LPCTSTR szParam );
    bool PushAction3( HWND hWnd, CHTML& html, LPCTSTR szParam );
    bool PushAction5( HWND hWnd, CHTML& html, LPCTSTR szParam );
    void ResetDEVCTL(CHTML& html, void* val ,cross::SParamValueH* pVal,std::string& strValue);
    void Set1_InFlag(CHTML& html, void* val ,cross::SParamValueH* pVal,std::string& strValue, std::string strName);
  };
//
#endif
