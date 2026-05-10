#include "stdafx.h"
#include "ACScomponent.h"
#include "ACS_Def.h"
#include "Info.h"


SAdressValue ACScomponent::GetACS( const char * Name, const char * Parameter)
{
	SAdressValue result;
	result.Letter = '0';
	result.Point = NULL;
	IBaseModel *pModel = IBaseModel::Find((char*)Name);
	if(!pModel) return result;
	CExtern_Pnt *pPnt = NULL;
	std::string paramter = Parameter;
    //##
    // if(pModel && pModel->Points.Root)
    // {
    // 	if(paramter.empty()) return result;
    // 	if(paramter.length() > 1 && paramter[0] == '@') paramter.erase(0, 1);
    // 	pPnt = (CExtern_Pnt*)::Find(&pModel->Points, (void*)paramter.c_str(), false);
    // }
	if(pPnt)
	{
		if(pPnt->Type[0] == 'A')
		{
			CAnalog *pA = (CAnalog*)pPnt;
			result.Letter = 'D';
			result.Point = &pA->Value;
			return result;
		}
		if(pPnt->Type[0] == 'D')
		{
			CDiscrete *pD = (CDiscrete*)pPnt;
			result.Letter = 'I';
			result.Point = &pD->Value;
			return result;
		}
		return result;
	}
	return result;
}
