#pragma once
#include <_magicKeysCentum.h>
#include <ConnectY.h>

template<class T>
void tYOKO_INITSTRUCT_STRUCT( T *pObj )
{
  memset( ((BYTE*)pObj) + 12, 0, sizeof( T ) - 12 );
  pObj->size = sizeof( T );
  pObj->ID_CLASS = T::TypeID;
}

#undef  YOKO_INITSTRUCT
#define YOKO_INITSTRUCT \
  tYOKO_INITSTRUCT_STRUCT( this );

template< class T >
int YokoReadStuctImpl( LPCSTR pName, T* pObj )
{
  if( !pYokoStateSer )
    return -1;
  DWORD LStruct = pYokoStateSer->BytesCanRead();
  BYTE * Addr = (BYTE*)pObj;
  DWORD Size = sizeof( T );
  if( !pYokoStateSer->SimpleRead( Addr, __min(LStruct, Size)) ) return -4;
  if( LStruct > Size )
  {
    LStruct -= Size;
    pYokoStateSer->Shift( LStruct );
  }
  return 0;
}

#define YOKO_READSTRUCT(Name,Class) \
{\
  if( strcmp( StrName, Name ) == 0 )\
    return YokoReadStuctImpl(Name, (Class*)this );\
  return 2;\
}

template< class T >
int YokoReadImpl( T* pObj )
{
  if( !pYokoStateSer )
    return -1;
  DWORD Size = sizeof( T );
  BYTE * Addr = (BYTE*)pObj;
  DWORD LStruct = pYokoStateSer->BytesCanRead();
  if( !pYokoStateSer->SimpleRead( Addr, __min(LStruct, Size)) ) return -4;
  if( LStruct > Size )
  {
    LStruct -= Size;
    pYokoStateSer->Shift( LStruct );
  }
  return 0;
}

#define YOKO_READ_NAMECOMPARE(Name, TestName,Class) \
  if( strcmp( TestName, Name ) == 0 )\
  {\
    return YokoReadImpl( (Class*)this );\
  }

#define REST_CLASS(Name,Class) YOKO_READ_NAMECOMPARE( Name, StrName, Class)

#define REST_FIELD(Field) \
  if( strcmp( StrName, #Field ) == 0 )\
  {\
    return YokoReadImpl( &W->Field );\
  }

template< class T >
int YokoWriteStateImpl( LPCSTR pName, T* pObj )
{
  if( !pYokoStateSer )
    return 1;
  DWORD Key = emkcStruct;
  if( !pYokoStateSer->SimpleWrite( &Key, sizeof( Key )) ) return -1;
  if( !pYokoStateSer->WriteStr( pName ) ) return -2;
  if( !pYokoStateSer->WriteSized( pObj, sizeof(*pObj)) ) return -3;

  return 0;
}

#define YOKO_SAVESTRUCT(Name,Class) if( int r=YokoWriteStateImpl(Name, (Class*)this ) ) return r;

#define SAVE_CLASS(Name,Class) if( int r=YokoWriteStateImpl(Name, (Class*)this ) ) return r;

#define SAVE_FIELD( Field ) if( int r=YokoWriteStateImpl( #Field, &W->Field ) ) return r;

#define YOKO_WRITE_NAMED(Name,Class) YOKO_SAVESTRUCT(Name,Class)
