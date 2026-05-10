#pragma once
#include "./rsuErr.h"
/////////////////////////////////////////////////////////
#undef WRT_VAR
#define WRT_VAR(Var) hFile.write( (char*)&Var, sizeof ( Var ) ); 
#undef WRT_DATA
#define WRT_DATA(L,Data) hFile.write( (char*)Data, L );
#undef WRT_ARR
#define WRT_ARR(Data) hFile.write( (char*)Data, sizeof(Data) );
#undef WRT_CLASS
#define WRT_CLASS(Class,K,Data) \
  {\
  DWORD L = lstrlen ( #Class );\
  hFile.write(&L, 1);\
  hFile.write(#Class, L);\
  L = sizeof(Class);\
  hFile.write(&L, 4);\
  hFile.write(&K, 4);\
  hFile.write(Data, K * L);\
  }
#undef WRT_STR
#define WRT_STR(Str) \
  {\
  DWORD L = lstrlen ( Str );\
  ASS(L<0x10000)\
  hFile.write(reinterpret_cast<char*>(&L), 2);\
  hFile.write(reinterpret_cast<char*>(Str), L);\
  }
#undef WRT_KEY
#define WRT_KEY(Key) { DWORD K = Key; hFile.write( (char*)&K, sizeof ( K ) ); }
/////////////////////////////////////////////////////////
static BOOL ReadImpl( std::fstream& hFile, BYTE* pData, DWORD sr, DWORD Lr)
{
  hFile.read(reinterpret_cast<char*>(pData), sr);
  Lr = (DWORD)hFile.gcount();
  BOOL b = Lr > 0 || sr == 0; 
  ASS( b );
  return b;
}

template<class T>
DWORD Read(std::fstream& hFile, T* var, DWORD sr)
{
    DWORD Lr = 0;
    ReadImpl(hFile, (BYTE*)var, sr, Lr);
    return Lr;
}
template<class T>
void Read( std::fstream& hFile, T* var, DWORD sr, DWORD& Lr)
{
  ReadImpl( hFile, (BYTE*)var, sr, Lr );
}

#undef READ_VAR
#define READ_VAR(Var) Read( hFile, &Var, sizeof( Var ), Lr ); 
#undef READ_DATA
#define READ_DATA(L,Data) Read( hFile, Data, L );
#undef READ_ARR
#define READ_ARR(Data) Read( hFile, Data, sizeof(Data) );
#undef READ_CLASS
template<typename Class, typename tK>
bool ReadClassImpl( std::fstream& hFile, Class **Data, tK K, LPCSTR pszClass )
{
  DWORD L = 0;
  Read(hFile, &L, 1);
  Read( hFile, &L, 1 );
  char ClassName[32];
  Read( hFile, ClassName, L );
  ClassName[L] = 0;
  int Res = strcmp ( ClassName, pszClass );
  ASSR( Res == 0, false )
  L = 0;
  Read( hFile, &L, 4 );
  ASSR(L == sizeof(Class), false);
  int Kr = 0;
  Read( hFile, &Kr, 4 );
  ASSR( Kr == K, false )
  if ( *Data == NULL ) 
    *Data = new Class[K];
  Read( hFile, *Data, K * L );
  return true;
}
#define READ_CLASS(Class,K,Data) \
  {\
  if( !ReadClassImpl( hFile, &Data, K, #Class ) ) return false;\
  }

#undef READ_STR
template<size_t _S>
void ReadStrImpl(std::fstream& hFile, char (&Str)[_S])
{
    //!!! Сюда можно вставить конвертер 1251 -> UTF8
  Str[0] = 0;
  DWORD L = 0;
  Read( hFile, &L, 2 );
  ASS( L+1 <= sizeof ( Str ));
  Read( hFile, Str, L );
  Str[L] = 0;
}
#define READ_STR(Str) \
  ReadStrImpl( hFile, Str );

#undef READ_KEY
template<typename T>
bool READ_KEY_impl( T Key, std::fstream& hFile )
{
  DWORD K = 0;
  Read( hFile, &K, sizeof ( K ) );
  if ( K != Key )
  {
    ASS(FALSE)
    return false;
  }
  return true;
}

#define READ_KEY(Key, retErr ) \
  {\
  if( !READ_KEY_impl( Key, hFile ) )\
    {\
    ASS(FALSE)\
    return retErr;\
    }\
  }
