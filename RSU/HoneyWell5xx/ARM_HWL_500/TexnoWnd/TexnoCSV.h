#pragma once
#include <map>
#include <string>
#include "SoftGDI.h"

class CTexnoCSV
{
public:
	CTexnoCSV();
	std::string GetTexnoshemeNameCSV();
	std::string GetHoneywellStartNameCSV();
	std::string GetHoneywellMenuNameCSV();
	std::string GetHoneywellInfoName();
	int GetStandartDialogNumberFromCSVFile(EDataTypes eData, UINT eScheme, std::string strName);
	std::string GetElectroZadvDialogCSV();
	void FillMapFromFile();
	std::string GetScreenshotSizesString();
	bool IsDefaultKlapanDialog(std::string strDialogName);
    std::string GetTexnoValueByKey(std::string key);
	int GetWindow1FromCSV();
    int GetWindow4FromCSV();
	std::string GetSensorWndRectString();
	std::string GetPanoramDirFromCSV();
	std::string GetPanoramDirExeFromCSV();
private:
	bool IsTexCSV(){return m_bIsTexCSV;};
	std::string ToString(int number);
	int GetKlapanDialogCSV(std::string strName);
	int GetAVODialogCSV(std::string strName);
	int GetPumpDialogCSV(std::string strName);
	int GetZadvDialogCSV(std::string strName);
	int GetZaglushkaDialogCSV(std::string strName);
	int GetVentilDialogCSV(std::string strName);
	int Get3xZadvDialogCSV(std::string strName);
	int GetZadvZaglushkaDialogCSV(std::string strName);
	int GetBallZadvDialogCSV(std::string strName);
    int GetGasAnalizDialogCSV(std::string strName);
	int GetStandartKlapanDialogCSV(std::string strName);


private:
	bool m_bIsTexCSV;
	typedef std::map<std::string, std::string> tTexnoCSVtype;
	tTexnoCSVtype m_mapTexnoCSV;
	int m_nNumberOfSpecKlapans;
};