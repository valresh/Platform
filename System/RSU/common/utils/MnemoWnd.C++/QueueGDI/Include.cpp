#include "Queue.h"
#include "../Lang.h"

void AddInclude(DefineTegs)
{
    bool bExt = true;
    //
    STegObj obj[] =
    {
    { 'B', "ext" , &bExt },
    };
    ::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));

    // Подгружаем файл
    auto buffer = managed::ParsingCallbacks.ReadIncludeFile(teg[0].value);
    char* ptr = buffer.Data;
    if (ptr)
    {
        CLang::ms_nStack++;
        ::ParserQueue(def.m_queue, ptr, rLog);
        CLang::ms_nStack--;
    }
    //
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
