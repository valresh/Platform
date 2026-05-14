#include "RsuX.h"
#include "RsuX_types.h"

namespace nRSUx
{
	void operator << ( SParamInfo &parm, double* pD )
	{
		parm.pMem = (BYTE*)pD;
		parm.size = sizeof( *pD );
		parm.type = e_double;
	}

	void operator << ( SParamInfo &parm, float* pD )
	{
		parm.pMem = (BYTE*)pD;
		parm.size = sizeof( *pD );
		parm.type = e_float;
	}

	void operator << ( SParamInfo &parm, int* pI )
	{
		parm.pMem = (BYTE*)pI;
		parm.size = sizeof( *pI );
		parm.type = e_int;
	}

	void operator << ( SParamInfo &parm, bool* pI )
	{
		parm.pMem = (BYTE*)pI;
		parm.size = sizeof( *pI );
		parm.type = e_bool;
	}

  void operator << ( SParamInfo &parm, short* pI )
  {
    parm.pMem = (BYTE*)pI;
    parm.size = sizeof( *pI );
    parm.type = e_short;
  }

  void operator << ( SParamInfo &parm, BYTE* pI )
  {
    parm.pMem = (BYTE*)pI;
    parm.size = sizeof( *pI );
    parm.type = e_byte;
  }
}
