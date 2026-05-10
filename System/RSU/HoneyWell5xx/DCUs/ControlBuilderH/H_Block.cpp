#include <rsuErr.h>
#include <H_Class.h>

bool SH_Block::s_DebuggerInitializing = false;
bool SH_Block::s_bProjectLoaded = false;

void SH_Block::StepT( SStepCalcParams &dt )
{
  InputConnectionsTransfer();
  OutputConnectionsTransfer();
}

void SH_Block::InputConnectionsTransfer()
{
  for( size_t i=0; i<inConsC; ++i )
    pInConns[i].Transfer();
}

void SH_Block::OutputConnectionsTransfer()
{
}

BOOL SH_Block::IsBlock()
{
  return TRUE;
}

KBmBase* SH_Block::WhoHasConnection( LPCSTR pszObj, LPCSTR pszFld, bool bOutput, LPCSTR *ppFld )
{
  if( bOutput )
  {
    for( DWORD n=0; n<inConsC; ++n )
    {
      SConnectionMB &con = pInConns[n];
      if( strcmp(con.szOutObj, pszObj) )
        continue;
      if( strcmp( con.szOutFld, pszFld) )
        continue;
      if( ppFld )
        *ppFld = con.szInFld;
      return this;
    }
  }
  return NULL;
}
