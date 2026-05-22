#pragma once
#include <QtCore/qglobal.h>
#ifdef SYSTEMREPLACE_EXPORTS
#define SYSTEMREPLACE_API Q_DECL_EXPORT
#else
#define SYSTEMREPLACE_API Q_DECL_IMPORT
#pragma comment( lib, "SystemReplace.lib")
#endif

typedef int (*tRsuHandleInitData)( struct CBase* pObj );

class SYSTEMREPLACE_API KNoName
{
  typedef int (*tNamesStringComparator)( LPCSTR lpString1, LPCSTR lpString2 );
  tNamesStringComparator pfStringComparator;

  struct SNoNameHeader
  {
    char szName[32];//Идентификатор файла
    DWORD nCount ;//Количество им¸н
    DWORD nCurr  ;//Текущая позиция в szBuff
    DWORD nMaxPos;//Текущая позиция в szVars

    DWORD shiftOrds;
    DWORD shiftBuff4Ords;
    DWORD shiftObjects;

    DWORD countOrds;//MAX_NAMES
    DWORD nBuff4Ords;//MAX_BUFFER
    DWORD nBuffObjects;//MEM_SIZE

    DWORD clientCounter;
  };

  struct SBuffOrd
  {
    DWORD nOrd;
    DWORD nClass;
    DWORD nEntry;
    int  nNumber;
    size_t  nData;
  };

#pragma pack(push)
#pragma pack( 1 )
  struct SBuffRecSeparator
  {
    DWORD strLength;
    DWORD crc32;
    char str[1];
  };
#pragma pack(pop)

  SNoNameHeader *pHeader;
  SBuffOrd *pOrd;
  BYTE* pszString4Ords;//szBuff
  BYTE* pszObjects;//szVars

  HANDLE m_hVarMapping;
  HANDLE m_hSemNames;

  bool m_bCrc2Upper;
  bool m_bCompareByCrc;

  enum
  {
    eClassNamesCount = 0x10000,
    eRsuSupportCount = 15,
  };
  static LPCSTR s_ClassNames[eClassNamesCount];
  static tRsuHandleInitData s_InitDataCalls[eRsuSupportCount];
public:
  KNoName();
  KNoName( KNoName &src );
  bool IsOpened();
  ~KNoName();
  KNoName& operator = ( KNoName &src );
public:
  static KNoName & Create( LPCSTR pszName, bool bMaster, DWORD nNames = 800000, DWORD nBtsStrings = 0xd00000, DWORD nBtsObjs = 0x0dd00000 );
public:
  static int RegisterClassInNames( DWORD idClass, LPCSTR pszClassName );
  static void RegisterInitCBaseCall( tRsuHandleInitData pInitData );
public:
  struct CBase* CreateStruct( LPCTSTR ObjName, DWORD TypeID, int number );
  struct CBase* CreateStruct( LPCTSTR ObjName, DWORD TypeID, DWORD sizeObj, int number );
  struct CBase* FindStruct( LPCTSTR ObjName, DWORD TypeID, int number );
  int CreateRefToData( LPCSTR ObjName, struct CBase *pBase, int newNumber = -1 );//если newNumber >= 0, то сделать в другом домене
  bool WhileBase( DWORD& ID, const DWORD ClassID, struct CBase** base, LPCSTR* pszName, int *pFcsNumber );
  bool GetNameEx( DWORD ID, LPCSTR* pszClass, LPCSTR* pszEntry, int *pFcsNumber = NULL );
  bool GetNameEx( DWORD ID, DWORD ClassID, LPCSTR* pszClass, LPCSTR* pszEntry, int *pFcsNumber = NULL );
  void GetName( DWORD ID, char *szClass, size_t _SC, char *szEntry, size_t _SE );
  void Reset();//использовать только bMaster-ом
  void PrintStatistic();
  size_t GetNAddr( DWORD ID );
  CBase* GetBase( size_t AddrBase );
  void SetCompareMethod( bool bUseCrc );//устанавливать везде
  tNamesStringComparator SetStringComparator( tNamesStringComparator pfnOther );
protected:
  int GetID( LPCSTR szClass, LPCSTR szEntry, bool bAdd, int number );
  int GetID( DWORD TypeID, LPCSTR szEntry, bool bAdd, int number );
  CBase* GetNameData( int ID );
  CBase* AppendData( int ID, DWORD TYPE, DWORD size = 0 );//int GiveCommData( int ID, DWORD TYPE, void** ppData )
private:
  LPCSTR Class( SBuffOrd &_ord, DWORD &crc );
  LPCSTR Entry( SBuffOrd &_ord, DWORD &crc );
  int Number( SBuffOrd &_ord );
  LPCSTR Class( DWORD ID, DWORD &crc );
  LPCSTR Entry( DWORD ID, DWORD &crc );
  int Number( DWORD ID );
  void SetBuffer( LPCSTR text );
  int InitData( struct CBase* pObj );
  LPCSTR GetEmpty();
};

template<size_t _SC, size_t _SE>
void GetName( KNoName &cont, DWORD ID, char (&szClass)[_SC], char (&szEntry)[_SE] )
{
  cont.GetName( ID, szClass, _SC, szEntry, _SE );
}
