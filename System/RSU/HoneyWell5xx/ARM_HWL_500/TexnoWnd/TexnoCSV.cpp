#include "stdafx.h"
#include "TexnoCSV.h"
#include <fstream>
#include <sstream>
#include <macros/csvrow.h>

CTexnoCSV::CTexnoCSV()
:m_bIsTexCSV(false)
, m_nNumberOfSpecKlapans(0)
{
}

void CTexnoCSV::FillMapFromFile()
{
  char Path[512] = {0}; 
  lstrcpy ( Path, GetBasePath() );
  lstrcat ( Path, "Data\\Equipment\\Texnoshema.csv" );

  if (Version!=RCOL_SI)
  {
	  std::ifstream file( Path );

	  int nCount=0;
	  if( file.is_open() )
	  {
		tCSVRow_ row(';');
		while( file >> row )
		{
		  if( row.size()<2 )
			continue;
		  if (nCount!=0)//первая строка заголовок
		  {
			  std::string strKey=row.m_data[0];
			 m_mapTexnoCSV[ strKey ] = row.m_data[1];
			 std::string strKeyKlapan="Клапан";
			 if (!strKey.compare(0, strKeyKlapan.size(), strKeyKlapan))
				 m_nNumberOfSpecKlapans++;
		  }
		  nCount++;
		}
	  }
	  m_bIsTexCSV=(nCount!=0);
  }
  else
  {
	  FILE * pFile;
	  long lSize;
	  char * buffer;
	  size_t result;
	  //m_nNumberOfDialogsInCSV=0;

	  pFile = fopen ( Path , "rb" );
	  CString strError;
	  if (pFile==NULL) 
	  {
      return;
	  }

	  fseek (pFile , 0 , SEEK_END);
	  lSize = ftell (pFile);
	  rewind (pFile);
	  buffer = (char*) malloc (sizeof(char)*lSize);
	  if (buffer == NULL) 
	  {	  
		  return;
	  }
	  result = fread (buffer,1,lSize,pFile);
	  if (result != lSize) 
	  {
		  return;
	  }
	  fclose (pFile);

	  CString strNameParametr="";
	  int nColumn=0;
	  int nRow=0;
	  std::string strKey;
	  std::string strValue;
	  for (long i=0;i<lSize;i++)
	  {
		  if ((buffer[i]!=';')&&(buffer[i]!='\r')&&(buffer[i]!='\t'))
				strNameParametr+=buffer[i];
		  else
		  {
			  if (buffer[i]==';')
			  {
			    if (nRow!=0)
				{
					if (nColumn==0)
						strKey=std::string(strNameParametr.GetBuffer());
					if (nColumn==1)
					{
						strValue=std::string(strNameParametr.GetBuffer());
						m_mapTexnoCSV[ strKey ] = strValue;
					}
				}
				nColumn++;
				strNameParametr="";
			  }
			  if (buffer[i]=='\r')
			  {
				if (nRow!=0)
				{
					if (nColumn==0)
						strKey=std::string(strNameParametr.GetBuffer());
					if (nColumn==1)
					{
						strValue=std::string(strNameParametr.GetBuffer());
						m_mapTexnoCSV[ strKey ] = strValue;
					}
				}
				nRow++;
				strNameParametr="";
				nColumn=0;
				i++;
			  }
		  }
	  }
	  if (nRow!=0)
	  {
		  if (nColumn==1)
		  {
				strValue=std::string(strNameParametr.GetBuffer());
				m_mapTexnoCSV[ strKey ] = strValue;
		  }
	  }
	  m_bIsTexCSV=(nRow!=0);
	  free (buffer);
  }
}

std::string CTexnoCSV::GetTexnoshemeNameCSV()
{
	std::string strName=GetTexnoValueByKey("Имя_Техсхемы");
	return strName;
}

std::string CTexnoCSV::GetHoneywellStartNameCSV()
{
	std::string strName=GetTexnoValueByKey("Honeywell_Start");
	return strName;
}

std::string CTexnoCSV::GetHoneywellMenuNameCSV()
{
	std::string strName=GetTexnoValueByKey("Honeywell_Menu");
	return strName;
}

std::string CTexnoCSV::GetHoneywellInfoName()
{
	std::string strName=GetTexnoValueByKey("Honeywell_InfoName");
	if (strName=="")
		strName="АРМ";
	return strName;
}

std::string CTexnoCSV::GetTexnoValueByKey(std::string key)
{
	std::string strValue="";
	if (IsTexCSV())
	{
		tTexnoCSVtype:: iterator it=m_mapTexnoCSV.find(key);
		if (it!=m_mapTexnoCSV.end())
			strValue=it->second;
	}
	return strValue;
}

int CTexnoCSV::GetAVODialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_аво");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetPumpDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_насоса");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetZadvDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_задвижки");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetZadvZaglushkaDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_задвижки_заглушки");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetBallZadvDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_шаровой_задвижки");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetZaglushkaDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_заглушки");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetVentilDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_дренаж_вентиля");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::Get3xZadvDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_3х_задвижка");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetGasAnalizDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_анализатора");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

int CTexnoCSV::GetStandartKlapanDialogCSV(std::string strName)
{
	int nDialog=-1;
	std::string strValue=GetTexnoValueByKey("Номер_диалога_клапана");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nDialog=atoi(strValue.c_str());
	}
	return nDialog;
}

std::string CTexnoCSV::GetElectroZadvDialogCSV()
{
	std::string strValue=GetTexnoValueByKey("Электрозадвижка");
	return strValue;
}

std::string CTexnoCSV::GetPanoramDirFromCSV()
{
	std::string strName=GetTexnoValueByKey("Папка_панорам");
	return strName;
}

std::string CTexnoCSV::GetPanoramDirExeFromCSV()
{
	std::string strName=GetTexnoValueByKey("Папка_панорам_Exe");
	return strName;
}

std::string CTexnoCSV::ToString(int nValue)
{
    std::ostringstream oss;
    oss << nValue;
    return oss.str();
}

bool CTexnoCSV::IsDefaultKlapanDialog(std::string strDialogName)
{
	std::string dlg="#Клапана/Dialog";
	std::string strValue=GetTexnoValueByKey("Номер_диалога_клапана");
	if (strValue!="")
		dlg+="-"+strValue;
	if (strDialogName==dlg)
		return true;
	return false;
}

int CTexnoCSV::GetKlapanDialogCSV(std::string strName)
{
	int nDialog=GetStandartKlapanDialogCSV(strName);
	std::string strKeyKlapan="Клапан";
	if (m_nNumberOfSpecKlapans>10)
		return nDialog;
	for (int i=0;i<m_nNumberOfSpecKlapans;i++)
	{
		std::string strNumberOfKlapan=ToString(i+1);
		std::string strKey=strKeyKlapan+strNumberOfKlapan;
		//UV__46 первые буквы имени клапана__номер диалога
		std::string strValue=GetTexnoValueByKey(strKey);
        if (strValue=="")
			return nDialog;
		size_t nFind=strValue.find("__");
		if (nFind==std::string::npos)
			return nDialog;
		std::string strFirst=strValue.substr(0,nFind);
		if (!strName.compare(0, strFirst.size(), strFirst))
		{
			size_t nLength=strValue.length();
			size_t nStartOfNumber=nFind+2;
			std::string strNumber=strValue.substr(nStartOfNumber,nLength-nStartOfNumber);
			if (::IsInt(strNumber.c_str()))
				nDialog=atoi(strNumber.c_str());
			else
				return nDialog;
			return nDialog;
		}
	}
	return nDialog;
}

int CTexnoCSV::GetStandartDialogNumberFromCSVFile(EDataTypes eData, UINT eScheme, std::string strName)
{
	int nDialog=-1;
	switch (eData)
	{
		case id_Pump:  return GetPumpDialogCSV(strName); break;
		case id_KVO:  return GetAVODialogCSV(strName); break;
		case id_Klapan:  return GetKlapanDialogCSV(strName); break;
		case id_GasAnalyz: return GetGasAnalizDialogCSV(strName); break;
        case id_HS: return GetZaglushkaDialogCSV(strName); break;
		case id_ModelData:
			{
				if (eScheme==id_Ventil)
					return GetVentilDialogCSV(strName);
				if (eScheme==id_3xZadv)
					return Get3xZadvDialogCSV(strName);
			}
			break;
		case id_Zadv: 
			{
				if (eScheme==id_Zadv)
					return GetZadvDialogCSV(strName);
				if (eScheme==id_ZadvZaglushka)
					return GetZadvZaglushkaDialogCSV(strName);
				if (eScheme==id_ShBallZadv)
					return GetBallZadvDialogCSV(strName);
			}
			break;
	}
	return nDialog;
}

std::string CTexnoCSV::GetScreenshotSizesString()
{
	std::string strValue=GetTexnoValueByKey("Скриншоты_размеры");
	return strValue;
}

int CTexnoCSV::GetWindow1FromCSV()
{
	int nWnd=0;
	std::string strValue=GetTexnoValueByKey("Окно1");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nWnd=atoi(strValue.c_str());
	}
	return nWnd;
}

int CTexnoCSV::GetWindow4FromCSV()
{
	int nWnd=0;
	std::string strValue=GetTexnoValueByKey("Окно4");
	if (strValue!="")
	{
		if (::IsInt(strValue.c_str()))
			nWnd=atoi(strValue.c_str());
	}
	return nWnd;
}

std::string CTexnoCSV::GetSensorWndRectString()
{
	std::string strValue=GetTexnoValueByKey("Окно4_Прямоугольник");
	return strValue;
}