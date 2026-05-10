#include "stdafx.h"
#include "Pump_b.h"

void CPump_b::ElectroOn()
{
	if(_pMotor)
		_pMotor->IsElectro = true;

}

void CPump_b::ElectroOff()
{
	if(_pMotor)
		_pMotor->IsElectro = false;
}