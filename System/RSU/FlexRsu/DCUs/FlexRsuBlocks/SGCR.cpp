#include "BlockCreator.h"
#include <FlexPrivateDeclare.h>
#include <float.h>

struct S_SGCR_P
{
#include "PrivateVars/SGCR.h"
};


class K_SGCR 
  : public KFlexBlockBase
  , protected S_SGCR_P
{
  KFlexConnection **m_ppOut;
  DWORD m_nOuts;
  KFlexConnection **m_ppIn;
  DWORD m_nIns;
public:
  static KFlexBlockBase *Create();
protected:
  virtual void OnCreated();
  virtual void AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts );
  virtual SReturn StepCalc( SCallParams &params );
  static void IniWParm();
};

static KBlockCreator _SGCR("SGCR", K_SGCR::Create);

KFlexBlockBase *K_SGCR::Create()
{
  return new K_SGCR;
}

#include <_InitInfo.h>
DECLARE_PRIVATE_VARSINFO( K_SGCR, 22 )
//////////////////////////////////////////////////////////////////////////
void K_SGCR::IniWParm()
{
#include "PrivateVars/SGCR.h"
  qsort( s_privateVarInfo, s_kprivateVarInfo, sizeof( SVarInfo ), CompVarInfo );
}

void K_SGCR::OnCreated()
{
  if( !s_kprivateVarInfo )
    IniWParm();

  m_kPrivateVarsInfo = s_kprivateVarInfo;
  m_pPrivateVarsInfo = s_privateVarInfo;
  m_pPrivateObject = (BYTE*)this;

  SetInitialVals<S_SGCR_P>( this, &s_private );
}

void K_SGCR::AssignConnections( KFlexConnection **ppIns, DWORD nIns, KFlexConnection **ppOuts, DWORD nOuts )
{
  m_ppOut = CloneConnections( m_nOuts, ppOuts, nOuts );
  m_ppIn = CloneConnections( m_nIns, ppIns, nIns );
  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    (*pIn) >> in;
  }
  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    (*pOut) << out;
  }
}

tAnalog32 APPR( tAnalog32 *X, tAnalog32 *Y, tAnalog32 ArgX, int C )
{
	int n = 0;
	if ( ArgX < X[0])
		{
		n = 0;
		ArgX = X[0];
		}
	else
	{
		while( n < (C-1) )
		{
			if ( ArgX < X[n+1])
				break;
			n++;
		}
		if( n >= (C-1) )
			n--;
		if( ArgX > X[n+1])
			ArgX = X[n+1];
	}
	tAnalog32 X0 = X[n];
	tAnalog32 dX = ArgX - X0;
	tAnalog32 mX = X[n+1] - X0;
	tAnalog32 Y0 = Y[n];
	tAnalog32 mY = Y[n+1] - Y0;
	tAnalog32 y = Y0;
  if( 0 != mX)
    y += dX / mX * mY;
	return y;
}

KFlexBlockBase::SReturn K_SGCR::StepCalc( SCallParams &params )
{
  SReturn sr;

  for( DWORD i=0; i<m_nIns; ++i )
  {
    KFlexConnection *pIn = m_ppIn[i];
    pIn->GetData();
  }

  out = APPR( &X1, &Y1, in, 10 );

  for( DWORD i=0; i<m_nOuts; ++i )
  {
    KFlexConnection *pOut = m_ppOut[i];
    pOut->DataReady2Send();
  }
  return sr;
}
