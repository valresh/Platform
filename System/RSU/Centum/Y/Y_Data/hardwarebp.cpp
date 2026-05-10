#include "stdafx.h"
#include <windows.h>
#include <rsuErr.h>
#include "hardwarebp.h"

#ifndef _WIN64
void HardwareBreakpoint::Set(void* address, int len, Condition when)
{
	// make sure this breakpoint isn't already set
	//ASS(m_index == -1);

	CONTEXT cxt;
	HANDLE thisThread = GetCurrentThread();

	switch (len)
	{
	case 1: 
    len = 0; break;
	case 2: 
    len = 1; break;
	case 4: 
    len = 3; break;
	default:
    return; // invalid length
	}

	// The only registers we care about are the debug registers
	cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;

	// Read the register values
	if (!GetThreadContext(thisThread, &cxt))
		return;

	// Find an available hardware register
	for (m_index = 0; m_index < 4; ++m_index)
	{
		if ((cxt.Dr7 & (1 << (m_index*2))) == 0)
			break;
	}
	if(m_index == 4) // All hardware breakpoint registers are already being used
    return;

	switch (m_index)
	{
	case 0: 
    cxt.Dr0 = (DWORD) address; break;
	case 1: 
    cxt.Dr1 = (DWORD) address; break;
	case 2: 
    cxt.Dr2 = (DWORD) address; break;
	case 3: 
    cxt.Dr3 = (DWORD) address; break;
	default:
    return; // m_index has bogus value
	}

	SetBits(cxt.Dr7, 16 + (m_index*4), 2, when);
	SetBits(cxt.Dr7, 18 + (m_index*4), 2, len);
	SetBits(cxt.Dr7, m_index*2,        1, 1);

	// Write out the new debug registers
	if (!SetThreadContext(thisThread, &cxt))
		ASS(false);
}


void HardwareBreakpoint::Clear()
{
	if (m_index != -1)
	{
		CONTEXT cxt;
		HANDLE thisThread = GetCurrentThread();

		// The only registers we care about are the debug registers
		cxt.ContextFlags = CONTEXT_DEBUG_REGISTERS;

		// Read the register values
		if (!GetThreadContext(thisThread, &cxt))
			ASS(false);

		// Zero out the debug register settings for this breakpoint
		ASS(m_index >= 0 && m_index < 4); // m_index has bogus value
		SetBits(cxt.Dr7, m_index*2, 1, 0);

		// Write out the new debug registers
		if (!SetThreadContext(thisThread, &cxt))
			ASS(false);

		m_index = -1;
	}
}
#endif
