#include "stdafx.h"
#include "Electro.h"
#include "Err.h"
//#include "CommProc.h"
#include "Work.h"
#include "SetData.h"
//#include <share.h>

void Split ( char * szBuf, char** Line, int nColumn)
{
	char* pChar = szBuf;
	int i;
	for(i = 0; i < nColumn; i++)
	{
		if(*pChar == '\n')
			*pChar = 0;
		Line[i] = pChar;		
		pChar = strchr(Line[i], ';');
		if(!pChar)
			break;
		*pChar = 0;
		pChar++;
	}
	if(i < nColumn)
	{		
		pChar = strchr(Line[i], '\n');
		if(pChar)
			*pChar = 0;
	}
}

CElectro::CElectro( char * _ObjName ) :	IBaseModel ( _ObjName ) 
#include "ACS_Constr.h"
#include "ACS.h"
{
	ObjName = _ObjName;
#include "IO_Clear.h"
#include "Points.h"
}

CElectro::~CElectro()
{
	aModel = NULL;
	nModel = 0;

}

int CElectro::Init( int Regim )
{
	return 0;
}

int CElectro::Step0( )
{
	char szElectricityPath[MAX_PATH] = {};
    sprintf(szElectricityPath, "%sDATA\\Equipment\\Electricity.csv", PROJECT_ROOT);
    FILE *file = fopen(szElectricityPath, "r");
	if(!file)
	{
		static bool bOutput = true;
		if(bOutput)
		{
			ModMsg ( "Ошибка открытия файла '%s'", szElectricityPath);
			bOutput = false;
		}
		return 0;
    }
    return 0;
    const int nMaxBufLen = 1024;
	char szBuf[nMaxBufLen] = {};	
	fgets(szBuf, nMaxBufLen, file);
	long iPos = ftell(file);
	int nColumn = 0;
	int iColumn = -1;
	char *pChar;
    do
      {
      if(szBuf[0] == '\r' || szBuf[0] == '\n' || szBuf[0] == '\0')
       break;
        pChar = strchr(szBuf, ';');
      // if ( pChar == NULL)
      //   pChar = strchr(szBuf, '\n');
      // if ( pChar == NULL)
      //   pChar = strchr(szBuf, '\r');
      // if ( pChar == NULL)
        continue;
      *pChar = 0;
      if(!strcmp(szBuf, ObjName))
        iColumn = nColumn;
		pChar++;
		strcpy(szBuf, pChar);	
		nColumn++;
	} while(true);
	if(iColumn == -1)
	{
		ModMsg ( "Не найден объект '%s'", ObjName);
		return 0;
	}
	char** szLine;
	szLine = new char*[nColumn]();
	while(fgets(szBuf, nMaxBufLen, file))
	{
		
		Split(szBuf, szLine, nColumn);
		if(szLine[iColumn][0] == '\0')
			continue;
		nModel++;
	}
	aModel = new IElectroModel*[nModel]();
	fseek(file, iPos, SEEK_SET);
	for(int i = 0; i < nModel; i++)
	{
		fgets(szBuf, nMaxBufLen, file);
		Split(szBuf, szLine, nColumn);
		IBaseModel* pModel = IBaseModel::Find(szLine[iColumn]);
		if(!pModel)
		{
			ModMsg ( "Не найден объект '%s' в '%s'", szLine[iColumn], ObjName);
			continue;
		}
		aModel[i] = dynamic_cast<IElectroModel*>(pModel);
		if(!aModel[i])
		{
			ModMsg ( "Объект '%s' в '%s' не является электрическим", szLine[iColumn], ObjName);
			continue;
		}
	}
	delete[] szLine;
	fclose(file);	
	return 0;
}

int CElectro::Step1( )
{
	return 0;
}

int CElectro::StepT ( double dt )
{
	Calc ( dt );
	return 0;
}

int CElectro::SetData( int TypeData, void * pData )
{
	return 1;
}

int CElectro::SaveState( )
{
	S_CLASS("W",_W_Electro)
	return 0;
}

int CElectro::RestoreState( char * StrName )
{
	R_CLASS("W",_W_Electro)
	return 1;
}












