#include <crossplatform.h>
#include <mio.hpp>

BYTE* OnDiskAllocator(mio::mmap_sink*& mapper, bool &isNew, DWORD requestedSeze, LPCSTR fileName, LPCSTR fileExtension, bool forceCreate);