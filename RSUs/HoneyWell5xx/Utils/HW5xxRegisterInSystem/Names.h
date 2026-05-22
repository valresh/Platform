#ifndef NAMES_H
#define NAMES_H


#undef IN_DLL
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif

//
IN_DLL bool InitNamesAndVariables( bool bClearN, bool bClearV );
//
IN_DLL int GetID( const char* szClass, const char* szEntry, int number = 0 );
IN_DLL int GetID( DWORD TypeID, const char* szEntry, int number = 0 );
IN_DLL int GetId( const char* szClass, const char* szEntry, int number = 0 );
IN_DLL int GetId( DWORD TypeID, const char* szEntry, int number = 0 );
IN_DLL BYTE *GetVarMemory ();

template<size_t _SC, size_t _SE>
void GetName ( int ID, char (&szClass)[_SC], char (&szEntry)[_SE] )
{
  GetName( ID, szClass, _SC, szEntry, _SE );
}
void IN_DLL GetName( int ID, char *szClass, size_t _SC, char *szEntry, size_t _SE );

bool IN_DLL GetNameEx( UINT ID, const char** pszClass, const char** pszEntry, int *pFcsNumber = NULL );
IN_DLL struct CBase* GetNameData( UINT ID );
DWORD IN_DLL GetStructOffset( DWORD AddrStruct );
//IN_DLL int GetData( LPCSTR ObjName, LPCSTR InOutName, DWORD ID, void** ppData);
//
int IN_DLL AddPntRef( char * FullName, int ID_PNT_NAME );
int IN_DLL AddObjRef( char * DllName, char * ObjName, struct IBaseModel * pModel );
int IN_DLL GiveCommData( LPCSTR ObjName, DWORD TypeID, void** ppData, int number = 0 );
int IN_DLL GetCommData ( LPCSTR ObjName, DWORD TypeID, void** ppData, int number = 0 );
int IN_DLL CreateRefToData( LPCSTR ObjName, CBase *ppBase );

DWORD IN_DLL GiveCommDataSized( DWORD idClass, DWORD sizeObj, CBase **ppBase );
IN_DLL struct CBase* GetBase( DWORD AddrBase );
void IN_DLL SetNData( UINT ID, DWORD AddrBase );
DWORD IN_DLL GetNData( UINT ID );
IN_DLL struct CBase* GetBaseID( DWORD ID );
//
IN_DLL struct CBase* CreateStruct ( LPCSTR ObjName, DWORD TypeID, int number = 0 );
IN_DLL struct CBase* FindConnPoint( LPCSTR ObjName, DWORD TypeID, int number = 0 );
IN_DLL struct CBase* FindConnPoint( int ID_PNT_NAME, DWORD TypeID  );
IN_DLL struct CSystem* GetSystemData();
//
// Для последовательного перебора структур в общей памяти
IN_DLL bool WhileBase( UINT& ID, DWORD TypeID, CBase** base, const char** pszName = NULL, int *pFcsNumber = NULL );
IN_DLL char * GetClass ( const int ID_CLASS );

IN_DLL int RegisterClassInNames( int idClass, LPCSTR pszClassName );

typedef int (*tNamesStringComparator)( LPCSTR lpString1, LPCSTR lpString2 );
tNamesStringComparator IN_DLL SetNamesStringComparator( tNamesStringComparator pNSC );

#endif // NAMES_H
