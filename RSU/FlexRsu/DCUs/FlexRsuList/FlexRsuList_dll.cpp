#include <crossplatform.h>
#include "SharedListValue.h"

FLEXRSULIST_API SListValue _theList;

// Instead Of DllMain()/__attribute__((constructor))
static struct Library
{
	Library()
	{
		_theList.Attach();
	}

	~Library()
	{
	}
} s_library;
