#include "../StdAfx.h"
#include "Dsd.h"
#include "HTML.h"
#include "Common.h"
//
CDsd::CDsd()
{
}

CDsd::~CDsd()
{
    Clear();
}

void CDsd::Clear()
{
    INT_PTR nCount = size();
    CDsd**     obj = data();
    for ( INT_PTR n = 0; n < nCount; n++ )
        delete obj[n];
    clear();
}
//
bool CDsd::Create( CHTML& html, const char* pszPath, const char* pszFileName )
{
    Clear();
    CharMP szFile;
    sprintf_s( szFile, SizeMP, "%s%s_files\\DS_datasource1.dsd", pszPath, pszFileName );
    //
    CFileXML xml;
    int nReturn = xml.LoadEx(szFile, pszFileName);
    //
    char* szBuffer = ::FileToBuff( szFile );
    //
    bool bReturn = false;
    if ( szBuffer != NULL )
    {
        char* ptr = szBuffer;
        bReturn = Parser( html, ptr );
        ::BuffFree( szBuffer );
    }
    return bReturn;
}
//
CDsd* CDsd::FindHDX( int nBind )
{
    if ( m_nID == nBind )
        return this;
    //
    INT_PTR nCount = size();
    CDsd**     obj = data();
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        CDsd* find = obj[n]->FindHDX( nBind );
        if ( find != NULL )
            return find;
    }
    return NULL;
}
//
int CDsd::Save(CHTML& html)
{
    if ( m_nCountDSD ) return m_nPlaceDSD;
    //
    INT_PTR nCount = size();
    CDsd**     obj = data();
    if ( !html.AddDSD((int)nCount+1) ) return -2;
    //
    m_nPlaceDSD = html.CurrDsd(0);
    SBaseDSD* list = html.DSD(m_nPlaceDSD);
    SBaseDSD* This = (SBaseDSD*)this;
    list[0] = *This;
    //
    for ( INT_PTR n = 0; n < nCount; n++ )
    {
        if ( !obj[n]->IsYou( html, "property" ) ) continue;
        int N = obj[n]->FindItem( html, "name" );
        if ( N < 0 ) continue;
        if ( lstrcmpi( obj[n]->GetValue(html,N), "version" ) == 0 ) continue;
        //
        This = (SBaseDSD*)obj[n];
        list[n+1] = *This;
        m_nCountDSD++;
    }
    list[0].m_nCountDSD = m_nCountDSD;
    html.CurrDsd(m_nCountDSD+1);
    //
    list[0].Pipe( html );
    //
    return m_nPlaceDSD;
}
//
typedef void   (*LPAnaliz)( CDsd* This, STegScn* teg );
void DsdDATAOBJECT( CDsd* This, STegScn* teg );
//
bool CDsd::Parser( CHTML& html, char*& ptr )
{
    struct SLocal { char* name; LPAnaliz analiz; };
    static SLocal list[] =
        {
         { "datasource"  , NULL          },
         { "hmipage"     , NULL          },
         { "property"    , NULL          },
         { "propertybag" , NULL          },
         { "dataobject"  , DsdDATAOBJECT },
         }InitLocal;
    //
    CDsd* obj;
    while ( *ptr )
    {
        int nReturn;
        STegScn* teg = theApp.ParsXML( ptr, nReturn );
        if ( nReturn > 0 ) //Все нормально
        {
            if ( *teg[0].name == '/' )
            {
                ASSERT( IsYou( html, teg[0].name+1 ) );
                return true;
            }
            else
            {
                SLocal* find = FindLocal( teg[0].name );
                if ( find )
                    obj = new CDsd();
                else
                {
                    if ( lstrcmpi( teg[0].name, "![CDATA[" ) == 0 )
                    {
                        char* p = strstr( ptr, "]]>" );
                        ASSERT( p != NULL );
                        if ( p == NULL )
                            return false;
                        ptr = p+3;
                        continue;
                    }
                    ASSERT(0);
                    return false;
                }
                ASSERT( obj != NULL );
                //
                bool bChild = ::IsChilds( teg, nReturn );
                obj->AddItems( html, nReturn-1, teg+1 );
                obj->SetLastName( html, find->name );
                //
                if ( find->analiz )
                    (*find->analiz)( obj, teg+1 );
                //Add( obj );
                push_back(obj);
                //
                if ( bChild )
                {
                    bool bReturn  = obj->Parser( html, ptr );
                    if ( !bReturn )
                        return false;
                }
            }
        }
        else
            if ( nReturn == -1 ) //Текстовое оформление
            {
                cross::CString str = "</"; str += teg[0].name;
                char* p = strstr( teg[0].value, str );
                ASSERT( p );
                if ( p == 0 )
                    return false;
                *p = 0;
                if ( teg[0].value && *teg[0].value )
                    SetLastValue( html, teg[0].value );
                *p = '<'; ptr = p;
            }
            else
                if ( nReturn == 0 ) //Конец документа
                {
                    return true;
                }
                else //Ошибка в формате
                {
                    ASSERT(0);
                    return false;
                }
    }
    return true;
}
//
//
#undef  STD_VAR
#define STD_VAR CDsd* a, char* value
typedef void (*LPFunct)( STD_VAR );
//
void dsdDoID( STD_VAR )
{
    ASSERT( ::IsInt(value) );
    a->m_nID = atoi(value);
}
//
void dsdDoType( STD_VAR )
{
}
//
void dsdDoFormat( STD_VAR )
{
    ASSERT( lstrcmpi( value, "propertybag" ) == 0 );
}
//
void DsdDATAOBJECT( CDsd* This, STegScn* teg )
{
    struct SLocal { char* name; LPFunct func; };
    static SLocal list[] =
        {
         { "id"    , dsdDoID     },
         { "type"  , dsdDoType   },
         { "format", dsdDoFormat },
         }InitLocal;
    //
    int n = 0;
    while ( teg[n].name )
    {
        SLocal* find = FindLocal( teg[n].name );
        ASSERT( find );
        (*find->func)(This,teg[n].value);
        n++;
    }
}
//
