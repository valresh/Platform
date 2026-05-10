#pragma once
#include <macros/NonCopyable.h>
#include "./IStateSer.h"

#ifdef STATESERHONEY_EXPORTS
#define STATESERHONEY_API _EXP
#else
#define STATESERHONEY_API _IMP
#endif

#pragma warning( push )
#pragma warning( disable : 4275)
class STATESERHONEY_API KStateSer 
  : public IStateSer
  , protected n_RSUs::NonCopyable
{
public:
  KStateSer(void);
  ~KStateSer(void);
  IStateSer* GetInterface();
protected:
  virtual void WriteImpl( const void *pObj, DWORD objSize );
  virtual bool ReadImpl( void *pObj, DWORD objSize );
};
#pragma warning( pop )
