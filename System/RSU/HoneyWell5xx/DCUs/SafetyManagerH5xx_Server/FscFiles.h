#pragma once
#include "FscFile.h"

// Содержит список сохраняемых значений
struct SFscDots : public SUniBufferT<SDotValue>
{
  SFscDots()
    : SUniBufferT()
  {};
  //
  SDotValue* Item(UINT n)
  {
    if ( n >= Count() )
      return NULL;
    return m_szBuffer + n;
  }
};

// Содержит список входов-выходов с указанием на следующий
// для объектов у которых нефиксированное количество входов-выходов
namespace ns_FN
{
  struct SLocal
  {
    UINT nData;
    UINT nNext;
  };
}
struct SFscNext : public SUniBufferT<ns_FN::SLocal>
{
  SFscNext()
    : SUniBufferT(){};
  //
  UINT Item(UINT n)
  {
    if ( n == INVALID_UNI ) 
      return INVALID_UNI;
    if ( n >= Count() ) 
      return INVALID_UNI;
    //SLocal* local = (SLocal*)m_szBuffer;
    //return local[n].nData;
    return m_szBuffer[n].nData;
  }
  //
  UINT Next(UINT n)
  {
    if ( n == INVALID_UNI ) 
      return INVALID_UNI;
    if ( n >= Count() ) 
      return INVALID_UNI;
    //SLocal* local = (SLocal*)m_szBuffer;
    //return local[n].nNext;
    return m_szBuffer[n].nNext;
  }
  //
  UINT Add( UINT nData )
  {
    _Type local = { nData, (UINT) - 1};
    return AddObj( &local );
  }
  //
  UINT AddInEnd( UINT n, UINT nData )
  {
    if ( n == -1 )
      return Add( nData );
    while ( m_szBuffer[n].nNext != INVALID_UNI )
      n = m_szBuffer[n].nNext;
    UINT nAdd = Add( nData );
    // После добавления m_szBuffer может измениться
    m_szBuffer[n].nNext = nAdd;
    return nAdd;
  }
};

// Данные по всем файлам
class CFscFiles : public IFscStorage
{
protected:
  SFscFile mFile;// Список всех файлов
  SFscList mList;// Список всех объектов в файлах
  SFscBuff mBuff;// Список разных структур
  SFscDots mDots;// Список сохраняемых значений
  SFscNext mNext;
public:
  struct SFldsInfo
  {
    enum eFldBlockType
    {
      HZ,
      Program,
      Comment,
      Function,
      Tag_number_index,
      FLD_index,
      Equation_block,
    }type;
    int nFld;
    char szTagName[32*4];
    char szTitle[128*4];
    SFldsInfo() : nFld(0), type(HZ)
    {
      szTagName[0] = 0;
      szTitle[0] = 0;
    }
  };
public:
  CFscFiles(void);
  ~CFscFiles(void);
  bool BuildListFsc( IFscStorage* fsc, LPCSTR pszFileMask, SUniTemp &temp, SFldsInfo *pFldsInfo = NULL, int nFI = 0 );
  LFscFile* FscFileList( UINT& nCount )
  {
    nCount = mFile.Count();
    return mFile.m_szBuffer;
  }
  LFscFile& FscFileItem( UINT nFile )
  {
    return mFile.Item( nFile );
  }
#ifdef _WIN32
  LFscBase* FscInRect( UINT nFile, POINT point );
#endif
  void* FscGiveData( UINT n )
  {
    return mBuff.Data( n );
  }
  LFscBase* GetItem( UINT nItem )
  {
    return mList.Item( nItem );
  }
  virtual int GetVars( LFscBase *obj, SVarInfo *pvi, int cVI, LPCSTR *ppPntName );
protected:
  //IFscStorage
  virtual UINT AddFscBuff( void* Void, int nSize );
  virtual UINT AddFscDots( SDotValue* add );
  virtual SDotValue* DotV( UINT n );
  virtual void* Data( UINT n );
  virtual UINT ItemLink( UINT nLink );
  virtual UINT NextLink( UINT nLink );
  virtual UINT PlusLink( UINT nLink, UINT nData );
  virtual SPointIn* PointIn( UINT n );
  virtual SPointOut* PointOut(UINT n);
protected:
  bool Load( IFscStorage* fsc, LPCSTR pszFileMask, SUniTemp& temp, SFldsInfo *pFldsInfo, int nFI );
  void Link( IFscStorage* fsc, SUniTemp& temp, UINT nParent );
  UINT SetInput( SUniTemp& temp, UINT nProc );
  void SetOutput( SUniTemp& temp, UINT nProc, LFscFile& file );
};
