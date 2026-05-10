#pragma once

struct SInterlockContainer
{
	bool g_SetComboboxValues;
	int g_IECGates;
	int g_CM_ArrayPointer;
	int g_Gate1;
	int g_Gate2;
	int g_Gate3;
	int g_Gate4;
	bool g_GateError;
	int g_DisplayErrorMessages;
	bool g_UpdateAllowed;

	void Reset()
	{
		g_SetComboboxValues = true;		
		g_IECGates = 0;
		g_CM_ArrayPointer = 0;
		g_Gate1 = 99;
		g_Gate2 = 99;
		g_Gate3 = 99;
		g_Gate4 = 99;
		g_GateError = false;
		g_DisplayErrorMessages = 0;
		g_UpdateAllowed = true;
	}
    SInterlockContainer() {Reset();}
};