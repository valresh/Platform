#include "stdafx.h"
#include "Vol.h"
#include "Err.h"
#include "CommProc.h"
#include "HydroStruct.h"
#include "TestNodes.h"

#define SET_BPN(PntName) \
  {\
  static C_BP T;\
  if ( ClassID == 0 || T.ID == 0 )\
  T.SetID( ClassID );\
  if ( T.Name == NULL )\
    {\
    T.Name = PntName;\
    T.File = __FILE__;\
    T.Line = __LINE__;\
    }\
    if ( BreakPoint && T.Break ) BREAK;\
  }

enum 
{
	PNT_GROUP_HEAT_1 = 1,
	PNT_GROUP_HEAT_2,
	PNT_GROUP_HEAT_3,
	PNT_GROUP_WATER,
  PNT_GROUP_COMMIO,
	PNT_GROUP_GAS,
	PNT_GROUP_OTHER,
};

NODE_LIST
NODE_IN(0,0,"top_#","Вход/выход сверху")
NODE_IN(0,0,"bot_#","Вход/выход снизу")
NODE_IN(1,0,"i_heat_1","Вход 1 теплоносителя")
NODE_IN(1,1,"o_heat_1","Выход 1 теплоносителя")
NODE_IN(2,0,"i_heat_2","Вход 2 теплоносителя")
NODE_IN(2,1,"o_heat_2","Выход 2 теплоносителя")
NODE_IN(3,0,"i_heat_3","Вход 3 теплоносителя")
NODE_IN(3,1,"o_heat_3","Выход 3 теплоносителя")
END_LIST


STATIC int TestNodes( char * ObjName, int kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( strstr ( ObjName, "V-3002"))
    KKK();
  if ( kNodes <= 0 )
    {
    ModMsg ( "У емкости '%s' не заданы входы", (char*)ObjName );
    return 1;
    }
  int kErr = 0;
  int kHeat = 0;
  int kHeat_2 = 0;
  int kHeat_3 = 0;
  int kW = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
    char Type = ' ';
    char * Name = pOP->PntName;
    if ( lstrcmpi_m( "i_heat_1|вход_нагрев", Name ) == 0 )
      {
      kHeat++;
      continue;
      }
    if ( lstrcmpi_m( "o_heat_1|выход_нагрев", Name ) == 0 )
      {
      kHeat++;
      continue;
      }
    if ( strcmp ( Name, "i_heat_2" ) == 0 )
      {
      kHeat_2++;
      continue;
      }
    if ( strcmp ( Name, "o_heat_2" ) == 0 )
      {
      kHeat_2++;
      continue;
      }
	if ( strcmp ( Name, "i_heat_3" ) == 0 )
      {
      kHeat_3++;
      continue;
      }
    if ( strcmp ( Name, "o_heat_3" ) == 0 )
      {
      kHeat_3++;
      continue;
      }
    // if ( strcmp ( Name, "exch_g" ) == 0 )
    //   {
    //   Type = 'в';
    //   continue;
    //   }
    // if ( strcmp ( Name, "exch_l" ) == 0 )
    //   {
    //   Type = 'н';
    //   continue;
    //   }
    if ( Name[0] == 't' )
      {
      Type = 't';
      continue;
      }
    if ( Name[0] == 'b' )
      {
      Type = 'b';
      continue;
      }
    // if ( Name[0] == 'в' )
    //   {
    //   Type = 'в';
    //   continue;
    //   }
    // if ( Name[0] == 'н' )
    //   {
    //   Type = 'н';
    //   continue;
    //   }
    if ( Name[0] == 'w' )
      {
      Type = 'w';
      kW++;
      continue;
      }
    }
  if ( kHeat != 0 && kHeat != 2 )
    {
    ModMsg ( "У емкости '%s' неправильные входы нагревателя", (char*)ObjName );
    kErr++;
    }
  if ( kHeat_2 != 0 && kHeat_2 != 2 )
    {
    ModMsg ( "У емкости '%s' неправильные входы нагревателя 2", (char*)ObjName );
    kErr++;
    }
  if ( kHeat_3 != 0 && kHeat_3 != 2 )
    {
    ModMsg ( "У емкости '%s' неправильные входы нагревателя 3", (char*)ObjName );
    kErr++;
    }
  if ( kW != 0 && kW != 1 )
    {
    ModMsg ( "У емкости '%s' выход воды должен быть один", (char*)ObjName );
    kErr++;
    }
  return kErr;
  }

int CVol::TestNodes( int _kNodes, struct CObjectPoint ** ppNodes )
  {
  if ( strstr ( ObjName, "V-3002"))
    KKK();
    kNodes = _kNodes;
	if ( kNodes <= 0 )
	  {
		ModMsg ( "У емкости '%s' не заданы входы", (char*)ObjName );
		return 1;
	  }
	if( ::TestNodes( ObjName, kNodes, ppNodes ))
		return 2;
	pNodes = new CNode[kNodes];
	kIO = 0;//kNodes;
	bool Err = false;
	int nComm = 0;
  int L_NodeNames = 0;
  kNodeNames = 0;
	for ( int n = 0; n < kNodes; n++ )
	  {
		CObjectPoint * pOP = ppNodes[n];
    pOP->Type = eГидравлика;
		pOP->NumbInGroup = 0;
		pNodes[n].Numb = n;
		pNodes[n].nIO = -1;
		pNodes[n].pOP = pOP;
    pNodes[n].Slow = 0.;
    pNodes[n].UseForCtrlLev = false;
		pOP->PntGroup = kIO + PNT_GROUP_OTHER;
		pOP->IO = '+';
		char Type = ' ';
    char Spec = ' ';
		char * Name = pOP->PntName;
    if ( lstrcmpi_m( "i_heat_1|вход_нагрев", Name ) == 0 )
      {
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_1;
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      UseHeat[0] = true;
      }
    if ( lstrcmpi_m( "o_heat_1|выход_нагрев", Name ) == 0 )
      {
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_1;
      pOP->NumbInGroup = 1;
      pOP->IO = '-';
      }
    if ( strcmp( Name, "i_heat_2" ) == 0 )
      {
      UseHeat[1] = true;
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_2;
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      }
    if ( strcmp( Name, "o_heat_2" ) == 0 )
      {
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_2;
      pOP->NumbInGroup = 1;
      pOP->IO = '-';
      }
    if ( strcmp( Name, "i_heat_3" ) == 0 )
      {
      UseHeat[2] = true;
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_3;
      pOP->NumbInGroup = 0;
      pOP->IO = '+';
      }
    if ( strcmp( Name, "o_heat_3" ) == 0 )
      {
      Type = 'H';
      pOP->PntGroup = PNT_GROUP_HEAT_3;
      pOP->NumbInGroup = 1;
      pOP->IO = '-';
      }
		if ( Type == ' ' )
		  {
      if ( Напорная )
        {
        pOP->PntGroup = PNT_GROUP_OTHER + 1024;
        pOP->NumbInGroup = kIO;
        }
      else
        {
        pOP->PntGroup = PNT_GROUP_OTHER + kIO;
        pOP->NumbInGroup = 0;
        }
      pNodes[n].nIO = kIO;
      kIO++;
      Type = 'X';
      L_NodeNames += lstrlen ( Name ) + 1;
      kNodeNames++;
// top_u...
      if ( strncmp ( Name, "top_u", 5 ) == 0 )
        Spec = 'u';
      if ( strncmp ( Name, "bot_u", 5 ) == 0 )
        Spec = 'u';
		  }
		if ( Type == ' ' )
		  {
			ModMsg ( "У емкости '%s' неправильный тип входа '%s'", (char*)ObjName, Name );
			Err = true;
		  }
		pNodes[n].Type = Type;
    pNodes[n].Spec = Spec;
//		pNodes[n].Name = Name;
	  }
	if ( Err )
		return 2;
// NodeNames
  NodeNames = NewArr ( char, L_NodeNames + 1 );
  NodeNumbs = NewArr( int, kNodeNames );
  int Pos = 0;
  int nNodeNames = 0;
  for ( int n = 0; n < kNodes; n++ )
    {
    CObjectPoint * pOP = ppNodes[n];
    char * Name = pOP->PntName;
    if ( lstrcmpi_m( "i_heat_1|вход_нагрев", Name ) == 0 )
      continue;
    if ( lstrcmpi_m( "o_heat_1|выход_нагрев", Name ) == 0 )
      continue;
    if ( strcmp( Name, "i_heat_2" ) == 0 )
      continue;
    if ( strcmp( Name, "o_heat_2" ) == 0 )
      continue;
    if ( strcmp( Name, "i_heat_3" ) == 0 )
      continue;
    if ( strcmp( Name, "o_heat_3" ) == 0 )
      continue;
    int L = lstrlen ( Name );
    memcpy ( NodeNames + Pos, Name, L );
    Pos += L;
    NodeNames[Pos++] = ';';
    NodeNumbs[nNodeNames++] = n;
    }
  NodeNames[Pos++] = 0;
  ASS(Pos == L_NodeNames + 1)
//
	kCommIO = nComm;                                                                                                   
	Omega = NewArr(double,kIO);
//	kOmega = SaveVar.NewDbl(kIO);
//  dP_Hydro = SaveVar.NewDbl( kIO );
	pFlow = NewArr(CFlow*, kIO);
	UseTopIn = NewArr(bool, kIO);
	Направление = NewArr(int, kIO);
  NumbNode = NewArr(int, kIO);
	VolBase.H_node = NewArr(double, kIO);
//	dPHydro = NewArr(double, kIO);
	dPGydro_Static = NewArr(double, kIO);
	dP_node = NewArr(double, kIO);
  P_Slow = NewArr(double,kIO);
  VolBase.Vol_Conn = NewArr(int,kIO);
  Mw_Flow = NewArr(double, kIO);
  Flow_No_This = NewArr(double, kIO);
  dPHydro = NewArr(double, kIO);
  dP_Hydro = NewArr(double, kIO);
  kOmega = NewArr(double, kIO);
	return 0;
  }

#include "SetMatr.h"

int CVol::SetMatr( void * pExternals, int PntGroup,  double dt, 
	  double A[/* kExkV */], double B[ /*kE*/ ] )
{
  SET_BP BreakPoint;
	if ( PntGroup == PNT_GROUP_HEAT_1 )
		MATR_OMEGA((Omega_Heat*1000.));
	if ( PntGroup == PNT_GROUP_HEAT_2 )
		MATR_OMEGA((Omega_Heat_2*1000.));
	if ( PntGroup == PNT_GROUP_HEAT_3 )
		MATR_OMEGA((Omega_Heat_3*1000.)); 

#if 0
	if ( PntGroup == PNT_GROUP_COMMIO )
	{
		if ( OverFlow )
		  {
			// Summ F[n] = 0.
			for ( int n = 0; n < kCommIO; n++ )
			  {
				double * Eq = A;
				Eq[F_0 + n*2] = 1.0;
				//Mf[n] = 1.;
			  }
			// P[n] = P[0]
			for ( int n = 1; n < kCommIO; n++ )
			{
				Mp[n*kCommIO+n] = 1.; //Mp[n,n]
				Mp[n*kCommIO] = -1.; //Mp[n,0]
			}
		  }
		else
		  {
			// F[n] = O * ( P[n] - P )
			for ( int n = 0; n < kCommIO; n++ )
			  {
				Mp[n*kCommIO+n] = Omega[n]; //Mp[n,n]
				Mf[n*kCommIO+n] = -1.; //Mf[n,n]
				V[n] = Omega[n] * VolBase.P;
			  }
		  }
		return 0;
	  }  
	if ( PntGroup == PNT_GROUP_GAS )
	{
		// F[n] = O * ( P[n] - P )
		// P = _P + dt * Ksi * ( Summ(F[i] )
#define MP(i,j) Mp[(i)*kCommIO+(j)]
#define MF(i,j) Mf[(i)*kCommIO+(j)]
		double Ksi = dt * RP * ( VolBase.T + TK ) / Vol; 
		for ( int n = 0; n < kCommIO; n++ )
		{
			MP(n,n) = Omega[n]; //Mp[n,n]
			MF(n,n) = -1.;      //Mf[n,n]
			V[n] = Omega[n] * VolBase.P;
			for ( int j = 0; j < kCommIO; j++ )
			{
				MF(n,j) -= Omega[n] * Ksi;
			}
		}
		return 0;
	}  
	SET_BP BreakPoint;
	int n = PntGroup - PNT_GROUP_OTHER; 
#endif
//  if ( strstr ( ObjName, "E-23"))
//    KKK();
  int n = PntGroup - PNT_GROUP_OTHER;
  if ( n == 1024 )
    {
    SET_BP BreakPoint;
// Напорная емкость
    double Summ = 0.;
    for ( int p = 0; p < kIO; p++ )
      {
      int nN = NumbNode[p];
      double Lev_Rel = VolBase.Lev_liq;
      if ( Lev_Rel > 1. )
        Lev_Rel = 1.;
      double dH = Lev_Rel - VolBase.H_node[p];  //??
      if ( pNodes[nN].Spec == 'u' )
        dH = 0.;// сброс гидростата если труба идет наверх
      double dP_Level;
      if ( dH > 0. && Use_Lev_hydro )
        dP_Level = VolBase.Ro_liq * dH * H_Vol * 1e-4;
      else
        dP_Level = 0.;
      if ( dPHydro[p] > 0. && dH > 0. )
        dP_Level += dPHydro[p] * dH;
      double kStatic = 1.;
      if ( dH < 0.1 )
        kStatic = dH * 10.;
      dP_Hydro[p] = dP_Level + dPGydro_Static[p] * kStatic;
      if ( VolBase.pFlow[p] && VolBase.pFlow[p]->ID_COMP )
        {
        CComp * pComp = VolBase.pFlow[p]->Get_Comp();
        Mw_Flow[p] = ::Mw ( pComp );
        Summ += Mw_Flow[p] * VolBase.pFlow[p]->Flow_mol;
        }
      else
        Mw_Flow[p] = 1.;
      }
    VolBase.Summ_Flow_kg = Summ;
////
    int kIO2 = kIO << 1;
#define MP(i,j) A[(i)*kIO2+((j)<<1)]
#define MF(i,j) A[(i)*kIO2+((j)<<1) + 1]
//    if ( strstr ( ObjName, "E-23") || strstr ( ObjName, "К-20") )
//      KKK();
    if ( OverFlow )
      {
      HydroOverflow = true;
      double SO = 0.;
      double SOdP = 0.;
      for ( int p = 0; p < kIO; p++ )
        {
        //if ( VolBase.pFlow[p] && VolBase.pFlow[p]->ID_COMP )
        //  {
        //  CComp * pComp = VolBase.pFlow[p]->Get_Comp();
        //  Mw_Flow[p] = ::Mw ( pComp );
        //  }
        //else
        //  Mw_Flow[p] = 1.;
        double MO = Mw_Flow[p] * Omega[p];
        SO += MO;
        SOdP += MO * dP_Hydro[p];
        }
      SO = 1. / SO;
      SOdP *= SO;
 //     -P = -( sum( Mi * Oi * Pi ) + sum( Mi * Oi * dPi) ) / sum( Mi * Oi );
      // P[p] = P[0]
      for ( int p = 0; p < kIO; p++ )
        {
        MF(p,p) = -1.;
        MP(p,p) = Omega[p];
        B[p] = Omega[p] * ( dP_Hydro[p] - SOdP );
        for ( int q = 0; q < kIO; q++ )
          {
          MP(p,q) -= Omega[p] * Mw_Flow[q] * Omega[q] * SO;
          }
        }
      }
    else
      {
      // F[p] = O * ( P[p] - P )
      HydroOverflow = false;
      for ( int p = 0; p < kIO; p++ )
        {
        MP(p,p) = Omega[p]; //Mp[p,p]
        MF(p,p) = -1.; //Mf[p,p]
        B[p] = Omega[p] * ( VolBase.P + dP_Hydro[p] );
        }
      }
    return 0;
    }
  int nN = NumbNode[n];
  SET_BP BreakPoint;
//  PARM_D( kP_Napor, "Скорость изменения напора", 0.1 )
  if ( UseSumm && !Use_Flow_Balance )
    {
    KKK();
    double O = Omega[n];
    double Ksi = kP_Napor * 1e-4;
    A[P_0] = -O;
    A[F_0] = 1. + O * Ksi * Mw_Flow[n] ;
    B[0] = -O * ( P_Napor + VolBase.P + Ksi * Flow_No_This[n] );
    P_Slow[n] = P_Napor + VolBase.P;
    return 0;
    }
  if ( Use_Flow_Balance && OverFlow && n == nFlow_summ  )
    {
    A[1] = 1.;
    B[0] = -Flow_summ_out;
    return 0;
    }
  if ( Разгерметизация )
    {
    A[0] = 1.;
    B[0] = 1.;
    return 0;
    }
//
  if ( Static_Calc_Mix )
    {
    if ( n == Static_Out_gas && Static_Fix_P )
      {
      A[F_0] = 1.;
      B[0] = -VolBase.Static_Fix_P_Fout;
      return 0;
      }
    if ( n == Static_Out_1F && Static_Fix_L )
      {
      A[F_0] = 1.;
      B[0] = -VolBase.Static_Fix_L_Flow_1;
      return 0;
      }
    if ( n == Static_Out_2F && Static_Fix_L )
      {
      A[F_0] = 1.;
      B[0] = -VolBase.Static_Fix_L_Flow_2;
      return 0;
      }
    }
 //
  if ( pNodes[nN].UseForCtrlLev && LevCtrl_Flow )
    {
    double Delta = Flow_Summ_Ctrl;
    A[F_0] = 1.;
    B[0] = Flow_Ctrl_out - 0.5 * Delta;//Src[n]->pFlow->Flow_mol - Delta; 
    return 0;
    }
  if ( pNodes[nN].Slow >= 1. )
    {
    SET_BP BreakPoint;
    double sdt = dt / pNodes[nN].Slow;
    double F = 0;
    if ( pFlow[n] )
      F = pFlow[n]->Flow_mol;
    double N = 1. / ( 1. + sdt );
    double O = Omega[n];
    A[0] = -O * sdt * N; 
    A[1] = 1.;
    B[0] = N * ( F - O * sdt * VolBase.P );
    return 0;
    }
  SET_BP BreakPoint;
	double O = Omega[n];
	int Знак = Направление[n];
	CFlow * pFlow = VolBase.pFlow[n];
	if ( Знак	&& pFlow )
	  {
		if ( Знак > 0 )
		  {
			if ( pFlow->Flow_mol < 0. )
			  {
				O = kOmega[n] = 0.01;
			  }
			else
			  {
				double K = kOmega[n];
				if ( K < 1. )
				  {
					O *= K;
					kOmega[n] = 0.9 * K + 0.1;
					if ( K > 0.99 )
						kOmega[n] = 1.;
				  }
			  }
		  }
		else
		  {
			if ( pFlow->Flow_mol > 0. )
			  {
				O = kOmega[n] = 0.01;
			  }
			else
			  {
				double K = kOmega[n];
				if ( K < 1. )
				  {
					O *= K;
					kOmega[n] = 0.9 * K + 0.1;
					if ( K > 0.99 )
						kOmega[n] = 1.;
				  }
			  }
		  }
	  }
  SET_BP BreakPoint;
///////////////////////////////////////////////////////////////////////
	if ( Use_Lev_max_hydro && UseTopIn[n] && VolBase.H_node[n] >= 0.99 )
		O *= KsiTop;                            
	if ( n == nFlow_lev_out )
	  {
		A[1] = -1.;
		B[0] = 2. * Flow_lev_out * VolBase.Lev_liq;
		return 0;
	  }
//
  if ( VolBase.Vol_Conn[n] == 2 )
    {
    SET_BPN("Матр:Переток газа") BreakPoint;
    double Ksi = dt * RP * ( VolBase.T + TK ) / Vol;// * VolBase.Eps[n];
    double O = OmegaGas * 1e5;
    A[P_0] = -O;
    A[F_0] = 1. + Ksi * O;   // F > 0 выход
    B[0] = -VolBase.P * O;
    return 0;
    }
//
  if ( VolBase.Vol_Conn[n] == 3 )
    {
    SET_BPN("Матр:Переток жидкости") BreakPoint;
    double O = OmegaLiq * 1e5;
    double IntPress;
    if ( VolBase.Lev_liq < VolBase.H_node[n] )
      IntPress = 1.;
    else
      IntPress = VolBase.Lev_liq - VolBase.H_node[n] + 1.;
    A[P_0] = O;
    A[F_0] = -1.;   // F > 0 выход
    B[0] = O * IntPress;
    return 0;
    }
//
  double Lev_Rel = VolBase.Lev_liq;
	if ( Lev_Rel > 1. )
		Lev_Rel = 1.;
	double dH = Lev_Rel - VolBase.H_node[n];  //??
  if ( pNodes[nN].Spec == 'u' )
    dH = 0.;// сброс гидростата если труба идет наверх
	double dP_Level;
	if ( dH > 0. && Use_Lev_hydro )
		dP_Level = VolBase.Ro_liq * dH * H_Vol * 1e-4;
	else
		dP_Level = 0.;
	if ( dPHydro[n] > 0. && dH > 0. )
		dP_Level += dPHydro[n] * dH;
	double kStatic = 1.;
	if ( dH < 0.1 )
		kStatic = dH * 10.;
	dP_Hydro[n] = dP_Level + dPGydro_Static[n] * kStatic;
	/////////////////////////////////////////////////
  double P;
  if ( !_finite ( VolBase.P ))
	  VolBase.P = 1.0;
  if ( OverFlow && P_Napor > 0. )
    {
    P =  P_Napor + VolBase.P + dP_Hydro[n];
    }
  else
    {
    P = VolBase.P + dP_Hydro[n];
    }
///
  if ( P_Slow[n] == 0. )
    P_Slow[n] = P;
  else
    P_Slow[n] += Slow_P * ( P - P_Slow[n] );
// P = P_ + dt * Ksi * F
  if ( !_finite ( P_Slow[n] ))
    P_Slow[n] = P;
  double Ps = P_Slow[n];
  FINITE(Ps)
  A[P_0] = -O;
  A[F_0] = 1;// + Ksi * O;
  B[0] = -Ps * O;
  SET_BP BreakPoint;
	return 0;
  }

#include "SetVar.h"

int CVol::SetVar( void * pExternals, int PntGroup, double dt,
				 double Var[/*kV*/],	struct CBaseStruct * pVar[/*kV*/] )
  {
  SET_BP BreakPoint;
	if ( PntGroup == PNT_GROUP_HEAT_1 )
	{
		IO_2(pFlow_heat_in,pFlow_heat_out)
			return 0;
	}  
	if ( PntGroup == PNT_GROUP_HEAT_2 )
	{                                                           
		IO_2(pFlow_heat_in_2,pFlow_heat_out_2)
			return 0;
	} 
	if ( PntGroup == PNT_GROUP_HEAT_3 )
	{
		IO_2(pFlow_heat_in_3,pFlow_heat_out_3)
			return 0;
	}  
	if ( PntGroup == PNT_GROUP_WATER )
	{
		pPseudoWater = (CFlow*)pVar[0];
		return 0;
	}  
	if ( PntGroup == PNT_GROUP_COMMIO )
	{
		double P_max = 0.;
		for ( int n = 0; n < kCommIO; n++ )
		{
			CFlow * pFlow = (CFlow*)pVar[n];
			VolBase.pFlow[n] = pFlow;
			double F = VolBase.pFlow[n]->Flow_mol;
			dP_node[n] = F / Omega[n];
			if ( Var[P_0 + 2 * n] > P_max )
				P_max = Var[P_0 + 2 * n];
		}
		P_IO = P_max;
		return 0;
	}  
	if ( PntGroup == PNT_GROUP_GAS )
	  {
		double dP_summ = 0.;
		double Ksi = dt * RP * ( VolBase.T + TK ) / Vol; 
		for ( int n = 0; n < kCommIO; n++ )
		  {
			CFlow * pFlow = (CFlow*)pVar[n];
			VolBase.pFlow[n] = pFlow;
			double F = VolBase.pFlow[n]->Flow_mol;
			dP_node[n] = F / Omega[n];
			dP_summ += Ksi * F;
		  }
		double P_summ = VolBase.P + dP_summ;
		double K = P_summ / VolBase.P;
		VolBase.M_Gas *= K;
		VolBase.P *= K;
		return 0;
	  }  
	SET_BP BreakPoint;
	int n = PntGroup - PNT_GROUP_OTHER;
  if ( n == 1024 )
    {
    SET_BP BreakPoint;
    // Напорная емкость
    if ( HydroOverflow )
      {
      double SO = 0.;
      double SOdP = 0.;
      double SOP = 0.;
      for ( int p = 0; p < kIO; p++ )
        {
        VolBase.pFlow[p] = pFlow[p] = (CFlow*)pVar[p];
        ASS ( Mw_Flow[p] )
        double MO = Mw_Flow[p] * Omega[p];
        SO += MO;
        SOdP += MO * dP_Hydro[p];
        double Pi = Var[p<<1];
        SOP += MO * Pi;
        }
      double P = ( SOP - SOdP ) / SO;
      P_summ0 = P;
      FINITE(P)
      P_Napor = P - VolBase.P;
      if ( P_Napor < 0. )
        OverFlow = false;
      if ( strstr ( ObjName, "E-23") || strstr ( ObjName, "К-20") )
        {
        double Test[4];
        double S = 0.;
        for ( int p = 0; p < kIO; p++ )
          {
          double F = pFlow[p]->Flow_mol;
          double Pi = pFlow[p]->P;
          Test[p] = Omega[p] * ( Pi - P - dP_Hydro[p] ) - F;
          S += Mw_Flow[p] * F;
          }
        KKK();
        }
      return 0;
      }
    for ( int p = 0; p < kIO; p++ )
      {
      VolBase.pFlow[p] = pFlow[p] = (CFlow*)pVar[p];
      }
    return 0;
    }
//////////////////////////////////////////////////////////
  int nN = NumbNode[n];
  if ( Use_Flow_Balance && OverFlow && n == nFlow_summ )
    {
    double Pin = Var[0];
    if ( Pin < 0.1 )
      Pin = 0.1;
    P_Napor += k_napor * 0.01 * ( Pin - VolBase.P - P_Napor );
    if ( P_Napor < 0. )
      OverFlow = false;
    }
  if ( VolBase.Vol_Conn[n] == 2 )
    {
    SET_BPN("Пере:Переток газа") BreakPoint;
    }
  if ( VolBase.Vol_Conn[n] == 3 )
    {
    SET_BPN("Пере:Переток жидкости") BreakPoint;
    }
	VolBase.pFlow[n] = pFlow[n] = (CFlow*)pVar[0];//pFlanec[n]->SetVarIn ( Var[P_0], (CFlow*)pVar[0] );
	ASS(VolBase.pFlow[n])
	double F = VolBase.pFlow[n]->Flow_mol;
///////////
  if ( pNodes[nN].UseForCtrlLev )
    {
    Flow_Summ_Ctrl;
    Flow_Ctrl_out = F;
    }
///////////
	dP_node[n] = F / Omega[n];
  SET_BP BreakPoint;
  if ( n == Static_Out_gas )
    {
    double Pfact = Var[P_0];
    double F = VolBase.pFlow[n]->Flow_mol;
    double Pflow = VolBase.P + F / Omega[n];
    Static_dP_Fix_P = Pfact - Pflow;
    }
  if ( n == Static_Out_1F )
    {
    double Pfact = Var[P_0];
    double F = VolBase.pFlow[n]->Flow_mol;
    double Pflow = VolBase.P + dP_Hydro[n] + F / Omega[n];
    Static_dP_Fix_L_1 = Pfact - Pflow;
    }
  if ( n == Static_Out_2F )
    {
    double Pfact = Var[P_0];
    double F = VolBase.pFlow[n]->Flow_mol;
    double Pflow = VolBase.P + dP_Hydro[n] + F / Omega[n];
    Static_dP_Fix_L_2 = Pfact - Pflow;
    }
  return 0;
}

