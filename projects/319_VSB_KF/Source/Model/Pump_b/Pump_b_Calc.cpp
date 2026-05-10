#include "stdafx.h"
#include "Pump_b.h"
#include "Err.h"
#include "CommProc.h"
#include "SysDataTypes.h"

double CPump_b::CalcRo(double E, double ro_l, double M_l, double p, double T, double M_g)
{
	double ro;
	if(E == 0.0)
		ro = ro_l;
	else if(E == 1.0)
		ro = p * M_g / ( RP * (T + TK));
	else
	{
		double r_g = p * AtmToPa * M_g / kMolToMol / ( RG * (T + TK));
		ro = (M_l * (1. - E) + M_g * E) * ro_l * r_g / (M_l * (1. - E) * r_g + M_g * E * ro_l);
		
	}
	if(ro == 0)
		ro = 1000.0;
	return ro;
}

void CPump_b::Calc()
{
	Перепад = Head;
	Мощность = Head * _Flow_Work_m3 / 3.6;
  //Абс_обороты = Speed * НоминальныеОбороты;
  //Cos = CosFi;
  //кВА = Мощность / CosFi;
  //Ток = Номинальный_ток * Speed;
}

