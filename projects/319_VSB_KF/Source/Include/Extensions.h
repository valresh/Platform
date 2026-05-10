#pragma once
#include "BaseModel.h"
#define _USE_MATH_DEFINES
#include "Param.h"

#ifdef LINUX
#include <QtCore/qglobal.h>
#ifdef UTILS_EXPORTS
#define IN_DLL Q_DECL_EXPORT
#else
#define IN_DLL Q_DECL_IMPORT
#endif
#else
#ifdef EXTENSIONS_EXPORTS
#define EXTENSIONS_API __declspec(dllexport)
#else
#define EXTENSIONS_API __declspec(dllimport)
#endif
#endif


struct IExtension
{
	CStr Name;
	virtual bool ReadData( struct IBaseModel * pModel ) { return true;};
};


struct ISplit_2F : public IExtension
{
	void * pObject;
	virtual void Split ( double P, double T, double M_Liq, bool * IsComp, double * Cmol_Liq, double * Cmol_Liq_1F, double * Cmol_Liq_2F ) = 0;
};


struct IAzeotrope : public IExtension
  {
  void * pObject;
  virtual IAzeotrope * New ( const char * Model ) = 0;
  virtual int Init( IBaseModel * pMain ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  virtual void Gamma ( double P, double T, bool * IsComp, double * Cmol_Liq, 
    double * Gamma, double * Pz_Gamma, double * Y_Gamma ) = 0;
  };


// Обратная связь с объектом
struct ILevel_Object
  {
  double ExtReg_LevZ; // Задание для внешнего регулятора
  bool UseExtReg; // Использовать для внешний регулятор
  // Эти функции вызываются при обращении к StepT
  virtual void ChangeVol ( double K_change ) = 0; // Вызывается при изменении пользователем объема
  virtual void ChangeLevel ( int nFaza, double K_change ) = 0; // Вызывается при изменении пользователем объема
  };

enum ePhases{PHASE_GAS = 0, PHASE_PRODUCT, PHASE_WATER, PHASE_POLYMER, NUMBER_OF_PHASES};

// struct ILevel_123 : public IExtension
//   {
//   ILevel_Object * pObject;
// //
// //
//   double Vol, VolZ;
//   bool ExternalRegLev;
// //
//   double dL_2_dt, dL_Summ_dt;
// //
//   ILevel_123::ILevel_123()
//     {
//     pObject = NULL;
//     Vol = 10.;
//     VolZ = 0.;
//     ExternalRegLev = false;
//     }
//   virtual void CalcEps (struct CNode_123 * pNode) = 0;
//   virtual void CalcLevels( double dt, double V_1, double V_2, double V_3, double M_1 = 0., double M_2 = 0., double M_3 = 0. ) = 0;
//   virtual int OnDefect( CDef * pDefect ) = 0;
//   virtual void StepT( double dt )
//     {
// 	  if ( VolZ > 0. )
// 	    {
// 		  pObject->ChangeVol( VolZ / Vol );
// 		  Vol = VolZ;
// 		  VolZ = 0.;
// 	    }
//     };
//   virtual double GetVol() = 0; // Суммарный объем
//   virtual double GetGasVol() = 0; // Газовый объем
//   virtual void GetRegim( IBaseModel * pMain ) = 0; // Сведения о текущих уровнях в виде PARM(L_1, "...") ..
//   virtual void GetTask( IBaseModel * pMain ){}; // Сведения о текущих задании в виде PARM(L_1, "...") ..
//   virtual bool IsLevelZ( ) = 0; // Есть ли заданные уровни
//   virtual double GetRegLevel( ) = 0; // Значение уровня для внешней ругулировки
//   virtual bool OverFlow( double & kL_1, double & kL_2, double & kL_3 ) = 0; //true при переполнении. kL_1, kL_2, kL_3 - к-ты уменьшения массы фаз
//   virtual double GetLevel(ePhases Phase) = 0;
//   virtual double GetLevel() = 0;
//   virtual int SetData( int ID, void * pData ){ return 0; };
//   static void SetEps(double H_b, double H_t, double L_1, double L_2, double & Eps_1, double & Eps_2);
//   };

struct IGammaParam
  {
  DWORD * Param;
  double * P;
  double * T;
  bool * IsComp;
  double * X;
  double * P_sat;
  double * Gamma;
  double * dH_liq;
  //IGammaParam()
  //  {
  //  memset ( this, 0, sizeof ( *this ));
  //  }
  };

struct IGamma : public IExtension
{
	IGamma * pCommon;
	virtual int Init( IBaseModel * pMain, IGamma * pCommon ) = 0;
	virtual void GetParams( IBaseModel * pMain, bool ShowTab ) = 0;
	virtual void UpdateParams( struct CParams & Param ) = 0;
	virtual void Gamma ( IGammaParam * pParam ) = 0;
	virtual int SaveState( char * SaveName ) = 0;
	virtual int RestoreState ( char * SaveName, char * StrName ) = 0;
};

struct IReactionsParam
  {
  DWORD  * Param;
  double dt;
  double * P;
  double * T;
  bool   * IsComp;
  double * X;
  double * Y;
  double * Q;
  double * M_Liq;
  double * R;
  };

struct IReactions : public IExtension
{
	virtual int Init( IBaseModel * pMain ) = 0;
	virtual void GetParams( IBaseModel * pMain ) = 0;
	virtual int GetParamList ( int NumbCall, int Numb, IBaseModel * pMain ) { return -1; };
	virtual void UpdateParams( struct CParams & Param ) = 0;
	virtual void Reactions( double dt, IReactionsParam * pParam ) = 0;
	virtual int SaveState( char * SaveName ) = 0;
	virtual int RestoreState ( char * SaveName, char * StrName ) = 0;
};


struct IPyrolysis
  {
  double * T_in;
  double * Q_Fire;
//
  double * T_out;
  double * kFlow;
//
  struct CFlow * pFlow_in;
  struct CFlow * pFlow_out;
//
  virtual void Reactions( double dt ) = 0;
  };

struct ICreatePyrolysis : public IExtension
{
	virtual bool ReadData( char * PathData ) { return true; };
	virtual IBaseModel * CreateObject( char * ObjName, IBaseModel * pMain ) = 0;
	virtual IPyrolysis * GetInterface ( IBaseModel * pObject ) = 0;
};

/*
struct ISensor : public IExtension
  {
  enum { ID = 0x42AF3987 };
  enum { Mask = 0x00000010 };
  char * Name;
  char * CompName;
  double Value;
//
  ISensor( );
  virtual int Init( IBaseModel * pMain ) { return 0;};
  virtual void GetParams( IBaseModel * pMain );
  virtual void UpdateParams( struct CParams & Param ){ ; };
  virtual double Get( struct CFlow * pFlow ) = 0;
  virtual double GetComp ( struct CFlow * pFlow, int & Id );
  };


struct IFlowReactions : public IExtension
  {
  enum { ID = 0x719BA024 };
  enum { Mask = 0x00000040 };
  //
  virtual int Init( IBaseModel * pMain ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  virtual void FlowReactions( double dt, struct CFlow * pFlow ) = 0;
  };

struct IParaffineРrecipitation : public IExtension
  {
  enum { ID = 0x2E1A7B33 };
  enum { Mask = 0x00000040 };
  //
  virtual int Init( IBaseModel * pMain ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual int SaveState( ) = 0;
  virtual int RestoreState( char * StrName ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  virtual void Рrecipitation( double dt, struct CFlow * pFlow ) = 0;
  virtual void Рrecipitation( double dt, double P, double T, bool * IsComp, double * Cmol, double & Q ) = 0;
  virtual void SetData( DWORD ID, void * pData ) = 0;
  };


struct IHeat_Change
  {
  enum eType{ Жидкость, Газ };
  eType Type;
  double T_in_heat, T_out_heat, T_out_cond;
  enum eDefect{ Исправен, Теплоотдача, Разгерметизация };
  eDefect Defect;
  double Defect_Param;
//
  IHeat_Change * pNext;
  IHeat_Change()
    {
    pNext = NULL;
    Type = Жидкость;
    T_in_heat = T_out_heat = T_out_cond = 15.;
    Defect = Исправен;
    Defect_Param = 0.;
    }
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  virtual void GetHeat( double T_ext, double Lev_Liq, double & Q ) = 0;
  };

struct IHeat_Change_List
  {
  IHeat_Change * pFirst;
  IHeat_Change * pLast;
  double T_in_heat, T_out_heat, T_out_cond;
  IHeat_Change_List()
    {
    pFirst = NULL;
    pLast = NULL;
    T_in_heat = T_out_heat = T_out_cond = 15.;
    }
  void Add( IHeat_Change * pHeat )
    {
    if ( pFirst == NULL )
      pFirst = pHeat;
    if ( pLast )
      pLast->pNext = pHeat;
    pLast = pHeat;
    }
  void GetHeat( int Type, double T_ext, double Lev_Liq, double & Q )
    {
    IHeat_Change * pHeat = pFirst;
    double Qs = 0.;
    double sT_in_heat = 0., sT_out_heat = 0., sT_out_cond = 0., q = 0.;
    int k = 0;
    while ( pHeat )
      {
      if ( Type >= 0 && Type != pHeat->Type )
        {
        pHeat = pHeat ->pNext;
        continue;
        }
      pHeat->GetHeat( T_ext, Lev_Liq, q );
      Qs += q;
      sT_in_heat += pHeat->T_in_heat;
      sT_out_heat += pHeat->T_out_heat;
      sT_out_cond += pHeat->T_out_cond;
      k++;
      pHeat = pHeat ->pNext;
      }
    T_in_heat = sT_in_heat / k;
    T_out_heat = sT_out_heat / k;
    T_out_cond = sT_out_cond / k;
    Q = Qs;
    }
  
  void GetParams( IBaseModel * pMain )
    {
    IHeat_Change * pHeat = pFirst;
    while ( pHeat )
      {
      pHeat->GetParams( pMain );
      pHeat = pHeat ->pNext;
      }
    }
  
  void UpdateParams( struct CParams & Param )
    {
    IHeat_Change * pHeat = pFirst;
    while ( pHeat )
      {
      pHeat->UpdateParams( Param );
      pHeat = pHeat ->pNext;
      }
    }
  };
*/
/*
struct IExtChem : public IExtension
  {
  enum { ID = 0x59AB4321 };
  enum { Mask = 0x00000020 };
  //
  virtual int SaveState( ) = 0;
  virtual int RestoreState ( char * StrName ) = 0;
  virtual int Init( IBaseModel * pMain ) = 0;
  virtual void GetParams( IBaseModel * pMain ) = 0;
  virtual void UpdateParams( struct CParams & Param ) = 0;
  virtual void NewExt( IBaseModel * pMain, int * ID, char * Name, ... ) = 0;
  virtual void * GetExt( int ID ) = 0;
  virtual void GetParams( int ID, IBaseModel * pMain ) = 0;
  virtual void UpdateParams( int ID, struct CParams & Param ) = 0;
  };
*/



struct IIntegral_Ctrl : public IExtension
{
	enum { ID = 0x52AF7321 };
	virtual void Control( double dt, double CtrlValue ) = 0;
	virtual int SaveState( ) = 0;
	virtual int RestoreState ( char * StrName ) = 0;
	virtual int Init( IBaseModel * pMain ) = 0;
	virtual void GetParams( IBaseModel * pMain ) = 0;
	virtual void UpdateParams( struct CParams & Param ) = 0;
	NEW
};


struct CProp_L
{
    int NumbSelect;
    int NumbFirstInExtInfo;
};

 bool IN_DLL GetPropList( CProp_L & Prop, char * Type, char PropList[256] );
// int IN_DLL GetIntList( char * Group, int MaxNames, char * Names[] );
 void IN_DLL GetDefault( CProp_L & Prop, char * Type );
 IN_DLL void * GetProp( CProp_L & Prop );
 IN_DLL void * GetProp(char * Group, char * Interface);

IN_DLL IExtension * Get_Ext(char * Dll, char * Point);
IN_DLL struct IExtension * ReadExtension(struct IBaseModel * pModel);

template <class T> T GetPropInt( CProp_L & Prop )
{
	return (T)GetProp( Prop );
}

template <class T> T GetPropInt( char * Group, char * Interface )
{
	return (T)GetProp( Group, Interface );
}

struct CShowParmsOmStage
{
	enum eWhat { X, Y, Gamma };
	eWhat What;
	int ID;
	char * Format;
};

#if 0
struct IN_DLL CParamExtension : public IFreeParam
{
	char * Group;
	int NumbExt;
	int kNames;
	char * Names[32];
	bool WasRead;
	//
	CParamExtension( char * Group );
	bool SetValue ( const char * Value );
	void ChangeVal( double Delta );
	void Out ( char Txt[256] );
	bool IsWrite();
	int Len();
	void * Data();
	bool SetData( int Len, const void * pData );
	bool SetOld ( int OldNumbExt );
	IExtension * GetExt( );
};
#endif
struct IEtalonComp : public IExtension
{
	double P_et, T_et, F_et_kg, F_et_mol, Mw_et;
	bool IsComp[128];
	double Cmol[128];
	///////////
	virtual void Init( ) = 0;
	virtual void Refresh( struct CFlow * pRealFlow ) = 0;
	virtual void SaveState( char * NameState ) = 0;
	virtual int RestState( char * StrName, char * NameState ) = 0;
	virtual void GetParams( IBaseModel * pMain ) = 0;
	virtual void UpdateParams( struct CParams & Param ) = 0;
	NEW
};
