#include "stdafx.h"
#include "Script.h"
#include "Err.h"
#include "CommProc.h"
#include "Common.h"
#include "ACS_Def.h"
#include "Connect.h"
#ifdef LINUX
#include "WinFiles.h"
#define HANDLE QFile *
#endif

int CScriptVar::ConnectPoint( )
{
    if ( Ref == NULL )
        return -1; // Внутренняя
    char Point[256];
    lstrcpy ( Point, Ref );
    char * P = strchr ( Point, '.' );
    if ( P == NULL )
        return -1; // Нет точки
    *P = 0;
    char * szParam = P + 1;
    char * Obj = Point;
    IBaseModel * pModel = IBaseModel::Find ( Obj );
    if ( pModel == NULL )
    {
        // char Name[256];
        // UINT nID = 0;
        // CBase* pBase = NULL;
        // char Class[256];
        // while ( WhileBase( nID, id_Unknown, &pBase ) )
        //   {
        //  if ( pBase -> ID_PNT_NAME == 0 )
        //    {
        //   // А такое может быть?
        //   continue;
        //    }
        //  GetName ( pBase->ID_PNT_NAME, Class, Name );
        //  int lN = lstrlen(Name);
        //  ASS( lN <= 255 );
        //  if (!lstrcmp(Name, Obj))// &&
        //   //lstrcmp ( StrClass, Class ) == 0 )
        //   break;
        // }
        // if(pBase)
        // {
        //  CLockParams Lock;
        //  GetParams( pBase );
        //  for(int iParam = 0; iParam < CParams::kParams; iParam++)
        //  {
        //   CParams& Param = CParams::Params[iParam];
        //   if(!lstrcmp(Param.ParamName, szParam))
        //   {
        // 	  pValue = Param.Addr;
        // 	  return 0;
        //   }
        //  }
        // }
        return 1;  // Объект не найден
    }
    // ACS
    CExtern_Pnt * pPnt = NULL;
    if ( pModel && pModel->Points.Root )
        pPnt = (CExtern_Pnt*)::Find( &pModel->Points, szParam, false );
    if ( pPnt )
    {
        if ( pPnt->Type[0] == 'A' )
        {
            if ( Type != Dbl )
                return 2;  // Не тот тип
            CAnalog * pA = (CAnalog*)pPnt;
            double * pAddr = (double*)&(pA->Value);
            pValue = pAddr;
            return 0;
        }
        if ( pPnt->Type[0] == 'D' )
        {
            if ( Type != Int )
                return 2;  // Не тот тип
            CDiscrete * pD = (CDiscrete*)pPnt;
            int * pAddr = (int*)&(pD->Value);
            pValue = pAddr;
            return 0;
        }
        return 3;
    }
    // Параметры
    CLockParams Lock;
    char ParamType = 0;
    if(IBaseModel::Find(pModel->ObjName, szParam, ParamType, &pValue) == false)
        return 3;
    if(Type == Dbl && ParamType != 'D')
        return 2;  // Не тот тип
    if(Type == Int && (ParamType != 'I' && ParamType != 'B'))
        return 2;  // Не тот тип
    if(pValue)
        return 0;
    return 4;
}

void CScriptVar::Connect( )
{
    if ( Ref == NULL )
        return;
    char *p;
    p = strchr(Ref, '.');
    if ( !p )
        return;
    int Res = ConnectPoint( );
    if ( Res == 0 )
        return;
    pValue = NULL;
}

void CScriptRsu::Connect( )
{
    char szAsuDir[MAX_PATH] = {};
    sprintf(szAsuDir, "%sDATA\\Acy", PROJECT_ROOT);
    WIN32_FIND_DATA wfd;
    char szFind[MAX_PATH] = {};
    sprintf(szFind, "%s\\*", szAsuDir);
    void *  hFind = FindFirstFile(szFind, &wfd);
    if(hFind == INVALID_HANDLE_VALUE)
        return;
    if(hFind == NULL)
        return;
    do
    {
        if( strstr(wfd.cFileName, "РСУ_"))
        {
            HANDLE hFile;
            char szFilePath[MAX_PATH];
            sprintf(szFilePath, "%s\\%s", szAsuDir, wfd.cFileName);
            hFile = CreateFile(szFilePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
            if (hFile == INVALID_HANDLE_VALUE)
                continue;
            UINT nFileSize = GetFileSize (hFile, NULL);
            char * pCsv = new char[nFileSize];//(char*)HeapAlloc(GetProcessHeap(), 0, nFileSize + 1);
            DWORD dwByte;
            ReadFile(hFile, pCsv, (size_t)nFileSize, &dwByte, NULL);
            char* pRef = strstr(pCsv, Ref);
            char RsuType[16] = {};
            if(pRef)
            {
                char* pStr = pRef;
                for(int i = 0; i < 2; i++)
                {
                    pStr = strchr(pStr, ';');
                    if(pStr)
                    {
                        *pStr = 0;
                        pStr++;
                    }
                }
                char* pType = pStr;
                pStr = strchr(pStr, ';');
                if(pStr)
                    *pStr = 0;
                lstrcpy(RsuType, pType);
                Value.LinkTo(Ref, RsuType, szFilePath);
                delete pCsv;//HeapFree(GetProcessHeap(), 0, pCsv);
                break;
            }
            delete pCsv;//HeapFree(GetProcessHeap(), 0, pCsv);
            CloseHandle(hFile);

        }
    } while(FindNextFile(hFind, &wfd));


}

