#pragma once
#include "QbBase.h"
#include <QbDataTypes_.h>
#include <MemoryServiceQB5xx.h>
#include <rsuStr.h>

template<class C, class S>
KQbBase * CreateImpl( LPCSTR Name )
{
  C * pClass = new C();
  pClass->QB_Type = S::TypeID;
  pClass->PointName = Name;
  _Ktoupper(pClass->PointName.text);
  S *W = (S*)KMemoryServiceQB5xx::Instance().CreateIOsStruct( pClass->PointName, S::TypeID, 0 );
  pClass->W = W;

  return pClass;
}


#define SQB_CREATE0(Class,Struct) \
  static KQbBase* Create( LPCSTR Name )\
{\
  return CreateImpl<Class,Struct>( Name );\
}

class SQB_STATUS : public KQbBase
{
  double m_PVperiod, m_OPperiod, m_MDperiod;
  double m_OPoutputWait;
  SPntConnectInfo m_Pv2SourceConnectInfo;
  SPntConnectInfo m_Op2SourceConnectInfo, m_Op2DestinctConnectInfo;
  KQbRsuX m_PV2Source, m_OP2Source;
  KQbRsuX m_OP2Destinc;
  double m_pulseUpAt, m_dtSum;
public:
  W_QBSTATUS *W;
  SQB_CREATE0(SQB_STATUS, W_QBSTATUS)
  static void InitParm();
  virtual void GetAsParams( KQbBridge2SysParam &paramCont );
  virtual LPCSTR GetTypeName();
  NEWQB
protected:
  virtual void SetParmList();
  virtual void Link();
  virtual void StepT( double dt );
  virtual SUserDef* GetUserDefs( DWORD &nCount );
  void SetOP();
  void AfterRestoreState();
};

#include <QB_Declare.h>
class SQB_ANALOG : public KQbBase
{
  struct SAlgo4
  {
    double ALG01;
    char ALG03[32*4];
    double ALG04;
    char ALG06[32*4];
    double ALG07;
    double ALG10;
    double ALG13;
    double ALG16;
    char ALG19[32*4];
  };
  struct SAlgo7
  {
    char ALG01[32*4];
    char ALG02[32*4];
    short ALG04;
    double ALG12;
    char ALG21[32*4];
  };
  struct SAlgo15
  {
    short blockNo;
    char ALG01[32*4];
    char ALG03[32*4];
    double ALG11;//scale factory
  };
  struct SAlgo20
  {
    char ALG01[128*4];
    //double ALG20_01_RESULT;
  };

  enum
  {
    A_COUNT = 4,
  };
  union
  {
    SAlgo4 algo4;
    SAlgo7 algo7;
    SAlgo15 algo15;
    SAlgo20 algo20;
  };
  SPntConnectInfo m_ASourceConnectInfo[A_COUNT], m_ADestinctConnectInfo[A_COUNT];
  KQbRsuX m_ASources[A_COUNT], m_ADestinct[A_COUNT];

  double m_period;

  KQbRsuX alg4_obj3;
  KQbRsuX alg7_obj2;
  KQbRsuX alg15_obj3;
  KScriptHandle algo20script;
public:
  W_QBANALOG *W;
  SQB_ANALOG();
  SQB_CREATE0(SQB_ANALOG, W_QBANALOG)
  static void InitParm();
  virtual void GetAsParams( KQbBridge2SysParam &paramCont );
  virtual LPCSTR GetTypeName();
  NEWQB
protected:
  virtual void SetParmList();
  virtual bool SetValue( LPCSTR pField, LPCSTR pszVal, LPCSTR pszVal2 );
  virtual void Link();
  virtual void StepT( double dt );
  virtual void SetSourceAddressSM( LPCSTR pszController, LPCSTR pszWord, LPCSTR pszParmName );
  virtual void SetDestinctAddressSM( LPCSTR pszController, LPCSTR pszWord, LPCSTR pszParmName );
  virtual void SetSourceAddressMOD( LPCSTR pszController, LPCSTR pszWord1, LPCSTR pszWord2, int ID, LPCSTR pszParmName );
  virtual void SetDestinctAddressMOD( LPCSTR pszController, LPCSTR pszWord1, LPCSTR pszWord2, int ID, LPCSTR pszParmName );
  virtual void SetSourceAddressHC900_UMB( LPCSTR pszIPxAddr, LPCSTR pszParmName );
  virtual void SetDestinctAddressHC900_UMB( LPCSTR pszIPxAddr, LPCSTR pszParmName );
  virtual void SetSourceAddressRsLogixMod( LPCSTR pszNameB1, LPCSTR pszNameB2, LPCSTR pszDataType, LPCSTR pszParmName );
  virtual void SetDestinctAddressRsLogixMod( LPCSTR pszNameB1, LPCSTR pszNameB2, LPCSTR pszDataType, LPCSTR pszParmName );
  virtual SUserDef* GetUserDefs( DWORD &nCount );
  virtual void OnScriptAssgned( LPCSTR pszFld );
  void AfterRestoreState();
};

class SQB_PSA : public KQbBase
{
public:
  W_QBPSA *W;
  SQB_CREATE0(SQB_PSA, W_QBPSA)
  static void InitParm();
  virtual void GetAsParams( KQbBridge2SysParam &paramCont );
  virtual LPCSTR GetTypeName();
  NEWQB
protected:
  virtual void SetParmList();
  virtual void Link();
  virtual void StepT( double dt );
  virtual SUserDef* GetUserDefs( DWORD &nCount );
};