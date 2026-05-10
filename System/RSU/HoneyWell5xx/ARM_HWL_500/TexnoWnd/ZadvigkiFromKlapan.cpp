#include "stdafx.h"
#include "ZadvigkiFromKlapan.h"

bool CZadvigkiFromKlapan::IsZadvigkaFromKlapanSborka(char* name)
{
	return (Is4LettersShift(name)||Is5LettersShift(name)||IsDrainIn(name)||IsDrainOut(name));
}

bool CZadvigkiFromKlapan::Is4LettersShift(char* name)
{
	return (IsVentIn(name)||IsVentBp(name));
}

bool CZadvigkiFromKlapan::Is5LettersShift(char* name)
{
	return (IsVentOut(name)||IsVentBp2(name));
}

bool CZadvigkiFromKlapan::IsVentIn(char* name)
{
	return (( _strnicmp( name, ".Âõ.", 4 ) == 0 )||( _strnicmp( name, ".ÂÕ.", 4 ) == 0 ));
}

bool CZadvigkiFromKlapan::IsVentBp(char* name)
{
	return (( _strnicmp( name, ".Áï.", 4 ) == 0 )||( _strnicmp( name, ".ÁÏ.", 4 ) == 0 ));
}

bool CZadvigkiFromKlapan::IsVentOut(char* name)
{
	return (( _strnicmp( name, ".Âûõ.", 5 ) == 0 )||( _strnicmp( name, ".ÂÛÕ.", 5 ) == 0 ));
}

bool CZadvigkiFromKlapan::IsVentBp2(char* name)
{
	return ( _strnicmp( name, ".ÁÏ2.", 5 ) == 0 );
}

bool CZadvigkiFromKlapan::IsDrainIn(char* name)
{
	return (( _strnicmp( name, ".Äð_âõ.", 7 ) == 0 )||( _strnicmp( name, ".ÄÐ_ÂÕ.", 7 ) == 0 ));
}

bool CZadvigkiFromKlapan::IsDrainOut(char* name)
{
	return (( _strnicmp( name, ".Äð_âûõ.", 8 ) == 0 )||( _strnicmp( name, ".ÄÐ_ÂÛÕ.", 8 ) == 0 ));
}

int CZadvigkiFromKlapan::GetKlapanNameShift(CString strName)
{
	if (Is5LettersShift(strName.GetBuffer()))
		return 5;
	else if(Is4LettersShift(strName.GetBuffer()))
		return 4;
		else if (IsDrainIn(strName.GetBuffer()))
				return 7;
			 else if (IsDrainOut(strName.GetBuffer()))
				 return 8;
	return 0;
}