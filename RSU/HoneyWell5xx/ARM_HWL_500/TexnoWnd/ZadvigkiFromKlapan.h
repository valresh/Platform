#pragma once
class CZadvigkiFromKlapan
{
public:
	static bool IsZadvigkaFromKlapanSborka(char* name);
	static bool Is4LettersShift(char* name);
	static bool IsVentIn(char* name);
	static bool IsVentBp(char* name);
	static bool IsVentOut(char* name);
	static bool IsVentBp2(char* name);
	static bool IsDrainIn(char* name);
	static bool IsDrainOut(char* name);
	static bool Is5LettersShift(char* name);
	static int GetKlapanNameShift(CString strName);
};
