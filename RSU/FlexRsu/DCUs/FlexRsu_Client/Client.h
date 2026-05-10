#pragma once
#include <basemodel.h>
#include "ICBsPool.h"
#include <rsu_client.h>

class KClient
  : //public IBaseModel, 
	protected ICBsPool
{
  struct SExchange
  {
    SExchange() : modelType(0), pModelAddr(NULL), rsuType(0), pRsuAddr(NULL), b2Model(false)
    {}
    char modelType;
    void *pModelAddr;
    char rsuType; // I - integer, D - double, B - bool
    void *pRsuAddr;
    bool b2Model;//1-to model, 0 to rsu
  };

  typedef std::vector<SExchange> tExchangeCont_;
  tExchangeCont_ m_RsuModelExchanges;
  DWORD m_nAcsFindStep;

  SExchange* AddLink( LPSTR pszModel, LPCSTR pszPath );

  //set params for SExchange on Step0
  template <typename  SimObj>
  void SetExchangeParams(SExchange* pRec, SimObj* pObj, char rsuType, bool b2Model);
  
  // transfer any type data between two void* pointers
  template <typename  SourceType, typename  DestinationType>
  void AssignPointer(void* pSource, void* pDestination);
  
  // transfer all data between simulator and model
  void TransferData();
    
  std::string m_ObjName = "";
  rsu_cp::RsuClientProxy m_proxy;  
public:
  KClient( LPSTR ObjName, rsu_cp::RsuClient* a_cl);
  // Интерфейс IBaseModel
  virtual int Init( int );
  virtual int Step0();
  virtual int Step1();
  virtual int StepT(double dt);
  virtual int SetData( int TypeData, const char* pData );
  virtual void Finalize();
  LPCSTR ObjName();
  NEW
protected:
  char m_szObjName[64] = { 0 };
  //ICBsPool
  virtual LPCSTR GetName();
#ifdef _WIN32
  virtual bool GetFirstACSObject();
  virtual bool GetNextACSObject( void ** pData, char *Name, size_t sn, char *Data, size_t sd );
  virtual bool ShowACSObject( 
	  HWND hMainWnd, 
		char * Name, void * pData );
#endif
};
