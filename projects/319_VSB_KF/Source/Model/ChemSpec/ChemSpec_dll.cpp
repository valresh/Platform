#include "stdafx.h"
#include "Info.h"
#include "ChemBase.h"
#include "ChemSpec.h"


typedef
double (*tRo_Water) ( double Tc, double P );
double Ro_Water ( double Tc, double P );

__declspec (dllimport) tRo_Water pRo_Water;

BOOL APIENTRY DllMain( _HANDLE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved	 )
{
  pRo_Water = Ro_Water;
  return TRUE;
}





MODEL_INFO(CChemSpec,0,"","")

#undef IF
#define IF(Comp) if ( lstrcmp ( CompName, Comp ) == 0 )

CGas * Create_Water_IAPWS();
CGas * Create_Water_Appr();
CGas * Create_S();
CGas * Create_C();
CGas * Create_Salt();
CGas * Create_CaCl2();
//CGas * Create_MAN();
//CGas * Create_DBPH();
CReal * Create_C2H4();
CReal * Create_C5H10();
//CReal * Create_C4H4();
//CReal * Create_C3H4();
//CReal * Create_H2();
CReal * Create_DEA_H2S();
CReal * Create_DEA_CO2();

extern "C"
  {
#ifdef LINUX
Q_DECL_EXPORT CGas * GetGasComp( const char * CompName )
#else
__declspec(dllexport) CGas * GetGasComp( const char * CompName )
#endif

    {
    IF("Вода по IAPWS")
      {
      return Create_Water_IAPWS();
      }
    IF("Вода аппр.")
      {
      return Create_Water_Appr();
      }
    IF("Примесная сера")
      {
      return Create_S();
      }
    IF("CARBON")
      {
      return Create_C();
      }
    IF("Соль")
      {
      return Create_Salt();
      }
	IF("CaCl2")
	{
		return Create_CaCl2();
	}
    //IF("MALEIC ANHYDRIDE")
    //  {
    //  return Create_MAN();
    //  }
    //IF("DIBUTYL PHTHALATE")
    //  {
    //  return Create_DBPH();
    //  }
    return NULL;
    }
#ifdef LINUX
    Q_DECL_EXPORT CReal * GetRealComp( const char * CompName )
#else
    __declspec(dllexport) CReal * GetRealComp( const char * CompName )
#endif
  {
  IF( "ETHYLENE")
    {
    return Create_C2H4();
    }
	IF( "CYCLOPENTANE")
	  {
		return Create_C5H10();
	  }
	IF("DEA+H2S")
	  {
		return Create_DEA_H2S();
	  }
  IF("DEA+CO2")
	  {
		return Create_DEA_CO2();
	  }
  //IF("HYDROGEN")
	 // {
		//return Create_H2();
	 // }
  return NULL;
  }
  };
