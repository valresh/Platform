#pragma once
#include <rsuModel.h>

class TrendsRSUServer : public IRsuModel
{
public:
	TrendsRSUServer(LPCSTR pszModelName);
private:
protected:
	//IRsuModel
	virtual int Initialize() { return 0; }
	virtual int AfterInit() { return 0; }
	virtual int AfterRestored() { return 0; }
	virtual int CalcStep(int dtMs) { return 0; }
	virtual int StateSave(LPCSTR pszPath) { return 0; }
	virtual int StateRestore(LPCSTR pszPath) { return 0; }
	virtual int ParamsSave(LPCSTR pszPath) { return 0; }
	virtual int ParamsRestore(LPCSTR pszPath) { return 0; }
#ifdef _WIN32
	virtual void ShowObject(LPCSTR pszName);
#else
	virtual void ShowObject(LPCSTR pszName) {}
#endif
public:
public:
	int DoInitialize() { return Initialize(); };
	int DoAfterInit() { return AfterInit(); };
	int DoAfterRestored() { return AfterRestored(); };
	int DoCalcStep(int dtMs) { return CalcStep(dtMs); };
	int DoStateSave(LPCSTR pszPath) { return StateSave(pszPath); };
	int DoStateRestore(LPCSTR pszPath) { return StateRestore(pszPath); };
	int DoParamsSave(LPCSTR pszPath) { return ParamsSave(pszPath); };
	int DoParamsRestore(LPCSTR pszPath) { return ParamsRestore(pszPath); };
	void DoShowObject(LPCSTR pszName)
	{
#ifdef _WIN32
		ShowObject(pszName);
#endif
	};
};
