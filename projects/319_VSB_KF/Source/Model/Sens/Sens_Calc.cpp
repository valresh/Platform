#include "stdafx.h"
#include "Sens.h"
#include "Err.h"
#include "CommProc.h"
#include "Work.h"
#include "Chem.h"


void CSens::Calc( double dt )
{
	SET_BP
	bool UseSlow = true;  
	if ( ЦиклИзмерений > 0. )
	{
		CycleTime += dt * 3600.;
		if ( CycleTime < ЦиклИзмерений )
			return;
		UseSlow = false;
		CycleTime = 0.;
	}
	if ( Анализатор )
	{
		if ( kStep % 10 == 0 )
		{
			if ( КривыеКипения )
				NewAnalyz( );
			else
				Analyz( );
		}
		return;
	}
	if ( pF == NULL )
	{
		return;
	}
	double V = 0.;
	switch ( Type )
	{
	case Нет_типа:
		V = Value;
		break;
	case Тип_Температура:
		{
			V = Calc_T( dt );
			UseSlow = false;
		}
		break;
	case Тип_Давление_и:
		{
			bool Abs = false;
			if (( MesZ / 100 ) & 0x01 )
				Abs = true;
			V = Calc_P( Abs ) * Scale( MesZ );
		}
		break;
	case Тип_Давление_а: // Абс давление
		{
			V = Calc_P( true ) * Scale( MesZ );
		}
		break;
	case Тип_Перепад: // Перепад
		{
			V = Calc_D( ) * Scale( MesZ );
		}
		break;
	case Тип_Плотность: 
		{
			V = Calc_Ro( ) * Scale( MesZ );
		}
		break;
	case Тип_Поток:
		{
			V = Calc_F( );
			if ( Нет_продукта.Use() || Заполнение.Use())
			{
				if ( pF && pF->ID_COMP )
				{
					SensT.PH ( pF );
					if ( SensT.E > 0.3 )
						Нет_продукта = 1;
					else
						Нет_продукта = 0;
					Заполнение =  100. * ( 1. - SensT.E );
					V = Заполнение;
				}
			}
		}
		break;
	case Тип_Уровень:
		{
			//      V = Calc_F( );
			if ( Нет_продукта.Use() || Заполнение.Use())
			{
				if ( pF && pF->ID_COMP )
				{
					SensT.PH ( pF );
					if ( SensT.E > 0.3 )
						Нет_продукта = 1;
					else
						Нет_продукта = 0;
					double Зап =  100. * ( 1. - SensT.E );
					V = Зап;
				}
			}
		}
		break;
	case Тип_Концентрация:
		{
			V = 0.;
			CComp * pComp = pF->Get_Comp();
			if ( pComp == NULL )
				break;
			double M_dry = 1.;
			if ( Сухой )
			{
				double C_H2O = 0.;
				if ( pComp->IsComp[G_H2O] )
					C_H2O = pComp->Cmol[G_H2O];
				if ( C_H2O < 0.9999 )
					M_dry = 1. / ( 1. - C_H2O );
				else
					M_dry = 0.;
			}
      if ( Conc < LAST_GAS )
			  V = pComp->Cmol[Conc] * M_dry;
      else
	      V = AdditonalComp( pComp, Conc - LAST_GAS, M_dry  );
			double M = Scale( MesZ );
			if ( M > 0. )
				V *= M;
			else
			{
				if ( M == -7 )
				{
					const double To_mg = 34e6 / 22.4;
					V *= To_mg;
				}
			}
		}
		break;
	case Тип_Молекулярный_вес:
		{
			CComp * pComp = pF->Get_Comp();
			V = Mw(pComp);
			double M = Scale( MesZ );
			if ( M > 0. )
				V *= M;
			break;
		}
	}
	if(bPow)
		V = pow(V, 2);
	if ( UseSlow )
		Value = Slow( V, dt );
	else
		Value = V;
	if ( !_finite( Value ))
		Value = V;
	_Value = CorrA * Value + CorrB;
	//
	if ( (int)Defect >= 0 )
		_Value = SetDefect( _Value );
	if ( Type == Тип_Поток )
		Сумма = Сумма + dt * _Value;
	//
	if ( UseMin > 0 )
	{
		if ( _Value < MinV )
		{
			switch ( UseMax )
			{
			case 1:
				_Value = MinV + 0.1 * ( _Value - MinV );
				break;
			case 2:
				_Value = MinV + 0.01 * ( _Value - MinV );
				break;
			case 3:
				_Value = MinV + 0.001 * ( _Value - MinV );
				break;
			default:
				_Value = MinV;
				break;
			}
			InvalidMin = true;
		}
		else
			InvalidMin = false;
	}
	if ( UseMax > 0 )
	{
		if ( _Value > MaxV )
		{
			switch ( UseMax )
			{
			case 1:
				_Value = MaxV + 0.1 * ( _Value - MaxV );
				break;
			case 2:
				_Value = MaxV + 0.01 * ( _Value - MaxV );
				break;
			case 3:
				_Value = MaxV + 0.001 * ( _Value - MaxV );
				break;
			default:
				_Value = MaxV;
				break;
			}
			InvalidMax = true;
		}
		else
			InvalidMax = false;
	}
	if ( Заполнение.Use())
		Заполнение = _Value;
	Показания = _Value;
	//  pSens->PhysValue = _Phys = Phys;
	//  pSens->ShowValue = _Value;
}

/*double CSens::Calc_Ro_Water( )
  {
  if(SensT.T != 0.0)
	  Phys = Water.Rol ( SensT.T, SensT.P );
  return Phys;
  }*/

/*double CSens::Calc_k_Ro ( double T )
{
	if ( kRo_test_max <= kRo_test_min )
		return kRo_test_max;
	if ( T < T_test_min )
		T = T_test_min;
	if ( T > T_test_max )
		T = T_test_max;
	double V = ( T - T_test_min ) / ( T_test_max - T_test_min );
	double K = kRo_test_min + V * ( kRo_test_max - kRo_test_min );
	return K;
}*/

double CSens::Calc_Ro( )
{
	if (( kStep % PeriodT == 1 ) )
	{                         
		SensT.T_min = T_min;
    SensT.T_max = T_max;
		SensT.PH ( pF );
	}
	if ( ThisGas )
	{
		Phys = Calc_Ro_Gas_Std( );
	}
	else
	{
		CComp * pComp = pF->Get_Comp();
		if( ! pComp )
			return 0.;
		//  double k_Ro = Calc_k_Ro ( SensT.T );
		Phys = Ro_liq (  SensT.T, pComp );
	}
	return Phys;
}

double CSens::Calc_T( double dt )
{
	if ( pF == NULL )
		return T_air;
	double Flow_mol = fabs ( pF ->Flow_mol );
	if ( Flow_mol >= Flow_min )
	{                         
		WasCalcT = true;
		SensT.T_min = T_min;
    SensT.T_max = T_max;
	if ( pF->ID_COMP )//&& kStep % 5 == 0 )
      {
      if ( pGamma )
        {
        SensT.pGamma = pGamma;
        SensT.Use_H_liq = true;
        }
      SensT.BP = BreakPoint;
		  SensT.PH ( pF );
      }
	}
	if ( Flow_mol < Flow_min )
	{
		if ( Tau_air > 0. )
		{
			SensT.T_min = T_min;
      SensT.T_max = T_max;
			double Ksi = dt * 60. / Tau_air;
			SensT.T	= ( SensT.T + T_air * Ksi )/ ( 1. + Ksi );
		}
		else
		{
			To_Air(SensT.T);
		}
		Phys = SensT.T;
		return Phys;
	}
	Phys = SensT.T;
	double New_Val;
	if ( Tau > 0.01 )
	{
		double Ksi = dt * 3600. / Tau;
		New_Val = ( Val_old + Ksi * Phys ) / ( 1. + Ksi );
	}
	else
		New_Val = Phys;
	double dT = New_Val - Val_old;
	if ( dT < -dT_max )
		dT = -dT_max;
	if ( dT > dT_max )
		dT = dT_max;
	Val_old += dT;
	return Val_old;
}

double CSens::Calc_Ro_Gas_Std( )
{
	double Mw = ::Mw ( pF->Get_Comp());
	Phys = Mw / M3;
	return Phys;
}

double CSens::Calc_F( )
{
	if ( pF == NULL )
		return 0.;
	double Mw = pF -> To_kg;
	double Flow_mol = fabs ( pF ->Flow_mol );
	Phys = Flow_mol;
	double M = Scale( MesZ );
	if ( M == -2. )
	{
		// кг/ч
		return Flow_mol * Mw;
	}
	if ( M == -4. )
	{
		// т/ч
		return Flow_mol * Mw * 1e-3;
	}
	if ( M == -5. )
	{
		//м3/ч(газ)
		if (( kStep % PeriodT == 1 ))
		{                         
			WasCalcT = true;
			SensT.T_min = T_min;
      SensT.T_max = T_max;
			SensT.PH ( pF );
		}
		double TC = SensT.T;
		return Flow_mol * RP * ( TC + TK ) / pF->P;
	}
	//  double Flow_kg = Flow_mol * Mw;
	if ( M == -1. || M == -3. )
	  {
		// м3/ч
		// л/ч
		if (( kStep % PeriodT == 1 ))
		  {                         
			WasCalcT = true;
			SensT.T_min = T_min;
      SensT.T_max = T_max;
			SensT.PH ( pF );
		  }
		double TC = SensT.T;
		/////
		//double k_Ro = Calc_k_Ro ( TC );
    double To_m3;
    CComp * pComp = pF->Get_Comp();
    __declspec (dllimport) double Press_For_Ro_Water;

//    Press_For_Ro_Water = 100.;
    if ( TC > 300. )
      TC = 300.;
		To_m3 = To_m3_liq( TC, pComp );
//    Press_For_Ro_Water = 100.;
		double Flow_m3 = Flow_mol * To_m3 /** k_Ro*/;
		if ( M == -3. )
			return Flow_m3 * 1e3;
		return Flow_m3;
	}
	return Flow_mol * M;
}

double CSens::Calc_D( )
{
	Phys = fabs ( Pin - Pout );
	return Phys;
}

double CSens::Calc_P( bool Abs )
{
	if ( Перепад.Use())
	{
		Phys = Pin - Перепад - 1;  //Фёдор
		return Phys;
	}
	if ( Abs )
		Phys = Pin;
	else
		Phys = Pin - 1;
	return Phys;
}

double CSens::Slow( double Val, double dt )
{
	double New_Val;
	if ( Tau > 0.01 )
	{
		double Ksi = dt * 3600. / Tau;
		New_Val = ( Val_old + Ksi * Val ) / ( 1. + Ksi );
	}
	else
		New_Val = Val;
	Val_old = New_Val;
	return New_Val;
}


char * AddComp[] = {
  "C2",   //0
  "C3",   //1
  "C4",   //2
  "C5",   //3
  "C3H8", //4
  "C3H6",   //5
  "CH4+C2H6", //6
  "C2H6+C3H8", //7
  "MAPD"       //8
  };
int kAdd = sizeof ( AddComp ) / sizeof ( char * );

#define ID(COMP) \
  static int G_##COMP = -1;

#define ID2(COMP,NAME) \
  static int G_##COMP = -1;

ID(METHANE)
ID(ETHANE)
ID(ETHYLENE)
ID(ACETYLENE)
ID(PROPANE)
ID(PROPYLENE)
ID(CYCLOPROPANE)
ID(METHYLACETYLENE)
ID(PROPADIENE)
ID(cyclopropene)
ID(ISOBUTANE)
ID2(n_BUTANE,"n-BUTANE")
ID2(1_BUTENE,"1-BUTENE")
ID(ISOBUTENE)
ID2(cis_2_BUTENE,"cis-2-BUTENE")
ID2(trans_2_BUTENE, "trans-2-BUTENE")
ID2(1_3_BUTADIENE,"1,3-BUTADIENE")
ID2(1_3_cyclobutadiene,"1,3-cyclobutadiene")
ID(ISOPENTANE)
ID2(n_PENTANE,"n-PENTANE")
ID(CYCLOPENTANE)
ID(CYCLOPENTENE)


#undef ID
#define ID(COMP) if ( G_##COMP < 0 ) G_##COMP = GetGasID ( #COMP );  \
ASS(G_##COMP>=0)\
if ( pComp->IsComp[G_##COMP] ) V += pComp->Cmol[G_##COMP];  

#undef ID2
#define ID2(COMP,NAME) if ( G_##COMP < 0 ) G_##COMP = GetGasID ( NAME );  \
ASS(G_##COMP>=0)\
if ( pComp->IsComp[G_##COMP] ) V += pComp->Cmol[G_##COMP];  

double CSens::AdditonalComp( CComp * pComp, int nAdd, double Dry  )
  {
  double V = 0;
  switch ( nAdd )
    {
    case 0:
      {
      ID(ETHANE)
      ID(ETHYLENE)
      ID(ACETYLENE)
      }
    break;
    case 1:
      {
      ID(PROPANE)
      ID(PROPYLENE)
      ID(CYCLOPROPANE)
      ID(METHYLACETYLENE)
      ID(PROPADIENE)
      ID(cyclopropene)
      }
    break;
    case 2:
      {
      ID(ISOBUTANE)
      ID2(n_BUTANE,"n-BUTANE")
      ID2(1_BUTENE,"1-BUTENE")
      ID(ISOBUTENE)
      ID2(cis_2_BUTENE,"cis-2-BUTENE")
      ID2(trans_2_BUTENE, "trans-2-BUTENE")
      ID2(1_3_BUTADIENE,"1,3-BUTADIENE")
      ID2(1_3_cyclobutadiene,"1,3-cyclobutadiene")
      }
    break;
    case 3:
      {
      ID(ISOPENTANE)
      ID2(n_PENTANE,"n-PENTANE")
      ID(CYCLOPENTANE)
      ID(CYCLOPENTENE)
      }
    break;
    case 4:
      {
      ID(PROPANE)
      }
    break;
    case 5:
      {
      ID(PROPYLENE)
      ID(CYCLOPROPANE)
      }
    break;
    case 6:
      {
      ID(METHANE)
      ID(ETHANE)
      }
    break;
    case 7:
      {
      ID(ETHANE)
      ID(PROPANE)
      }
    break;
    case 8:
      {
      ID(METHYLACETYLENE)
      ID(PROPADIENE)
      }
    break;
    }
  return V * Dry;
  }
