#pragma once
//
#include <afxtempl.h>
#include "HoneyMnemoExtend.h"
#include "UniBuffer.h"
#include "ZadvigkiFromKlapan.h"
#include "FileXML.h"
#include <SocketClient.h>
//
enum EColorScheme
{
	eColor_0_1=0,
	eColor_0_100
};
struct STagsObj
  {
    int m_nNumber;      // Номер в трубке
    UINT m_eType; // Тип объекта
    HCHAR m_nName,m_nComm;// Ссылки на имена
    RECT rc;
    COLORREF color;
    int nFont;// Высота шрифта
    int nDots;// Формат вывода переменной
    int m_nSocketNumber;
	UINT nModelDialog;
	EColorScheme m_colorModelScheme;
  };
//
struct SUniTags : public SUniBuffer
  {
  SUniTags(SUniChar& n)
  : SUniBuffer(sizeof(STagsObj)), name(n){};
  //
  SUniChar& name;
  STagsObj& Tags(int n){STagsObj* lst = (STagsObj*)m_szBuffer;return lst[n];}
  char* Name(int n){STagsObj& obj = Tags(n);return name.Name(obj.m_nName);}
  char* Comm(int n){STagsObj& obj = Tags(n);return name.Name(obj.m_nComm);}
  };
//
class CArrayObj : public CFileXML
  {
  int m_nMonitor;
  public:
    CArrayObj(int nMonitor);
    virtual ~CArrayObj(void);
	  HMyEMF LoadCrd( LPCTSTR pszFileName, int& nError, bool bBuildList = false);
	  int    ReadCrd( LPCTSTR pszFileName,double  scaleX,double  scaleY);
	  int  FindShapeFromPoint( POINT point );
	  int  NextShapeFromPoint( POINT point, int find );
    INT_PTR FindShapeFromRect ( RECTL& rcl, int type );
    void DrawAllShapes(HDC hDC, CSocketClient& sockClient);
    void BuildClient( CSocketClient& sockClient);
    void ClearsAll();
	bool AlwaysLeaveSensorName();
    SUniTags mTags;
    SUniChar mName;
    char* m_szBuff;
	HANDLE hEvent;
    const char* Menu()
      {
      return m_strMenu;
      }
  public:
    bool ChangeMenu( const char* pszFile );
  public: // Получение имени ключа
    char* KeyName( const char* name );
  public: // Функции для работы с меню
    bool Menu(HMENU hMenu,UINT id,const char* pszCurr,bool bOwnerDraw,HXML nItem = INVALID_HXML);
    const char* FileToMenu(const char* pszFile,HXML nItem = INVALID_HXML);
    HXML NextMenu(HXML nItem = INVALID_HXML);
    int  CountMenu();
  private:
    bool IsCheck(HXML nItem,const char* pszCurr);
    CString m_strMenu;
  };
//
inline RECT GetRect(RECTL& rcl)
  {
  RECT rc;
  ::SetRect( &rc, rcl.left, rcl.top, rcl.right, rcl.bottom );
  return rc;
  }
