#pragma once

#include <string>
#include <memory>
#include "../BaseModel.h"
#include "rsu_api.h"
#include "rsu_clients_hlpr.h"
#include <SetDataTypes.h>

namespace rsu_cp {
#define RSU_NOT_USED 0
#define RSU_NO_CLIENT 1000
#define RSU_NOT_IMPLEMENTED 1001

	class RsuBaseModel: public IBaseModel {
		RsuClientAPI *m_api = nullptr;
		std::string m_LibName = "";
		std::string m_LibCfg = "";
		std::string m_ObjName = "";
		std::string m_LibPath = "";
	public:
		bool InitState() { return m_api != nullptr; }
		const std::string& LibName() { return m_LibName; }
		const std::string& LibCfg() { return m_LibCfg; }
		const std::string& ObjName() { return m_ObjName; }
		const std::string& LibPath() { return m_LibPath; }
		RsuClientAPI* api() { return m_api; }
		RsuBaseModel (const std::string& a_LibName, const std::string& a_LibCfg, const std::string &a_ObjName, const std::string& a_LibPath, RsuClientAPI* a_api):
			IBaseModel((char*) a_ObjName.c_str()), m_LibName (a_LibName), m_LibCfg (a_LibCfg), m_ObjName (a_ObjName), m_LibPath(a_LibPath)
		{ 
			TypeObj = RSU_Obj;
			Model = "";
			m_api = a_api;			
		}
		virtual ~RsuBaseModel()
		{
			//delete m_api; Библиотека к этому моменту уже выгружена
		}
		virtual int Init(int arg)
		{
			if (m_api)
				return m_api->Init()(arg);
			return RSU_NO_CLIENT;
		}
		virtual int Step0()
		{
			if (m_api)
			{
				auto method = m_api->Step0();
				return method();
			}

			return RSU_NO_CLIENT;
		}
		virtual int Step1()
		{
			if (m_api)
				return m_api->Step1()();
			return RSU_NO_CLIENT;
		}
		virtual int StepT(double dt)
		{
			if (m_api)
				return m_api->StepT()(dt);
			return RSU_NO_CLIENT;
		}
		virtual int SetData(int TypeData, void* pData)
		{
			if (m_api)
				switch (TypeData)
				{
				case sd_SaveRSUParams:
					return m_api->ParamsSave()((LPCSTR)pData);
				case sd_RestoreRSUParams:
					return m_api->ParamsRestore()((LPCSTR)pData);
				case sd_SaveRSUState:
					return m_api->StateSave()((LPCSTR)pData);
				case sd_RestoreRSUState:
					return m_api->StateRestore()((LPCSTR)pData);
				default:
					return -1;
				}							
			return RSU_NO_CLIENT;
		}
		virtual void Finalize()
		{
			return;
		}
		/*Дальше то, что не используется в RSU*/
		virtual int PreStepT() { return RSU_NOT_USED; }
		virtual int GetParams(char* StrName) { return RSU_NOT_USED; }
		virtual int ShowParams(DWORD DataType, struct CShowData* pSD) { return RSU_NOT_USED; }
		virtual int UpdateParam(struct CParams& Param) { return RSU_NOT_USED; }
		virtual int SaveState() { return RSU_NOT_USED; }
		virtual int RestoreState(char* StrName) { return RSU_NOT_USED; }
		//
		virtual int GetProp() { return RSU_NOT_USED; }
#ifdef _WIN32
		virtual void DrawObj(struct CDrawObjData* pDraw) { return; }
		virtual void DrawObject(struct CDrawObjData* pDraw, CGDIResourceMgr* pResMgr) {}
		virtual BOOL NeedDrawBefore() { return FALSE; }
#endif
		//
		virtual void Render(IRenderer&) {};
		virtual int GetConnections(int& Cnt, struct CConnection Connections[/*100*/]) { return RSU_NOT_USED; }
		virtual int OnDefect(struct CDef* pDefect) { return RSU_NOT_USED; }
		virtual void SetName(char* Name, IBaseModel* pMain) { return; }
		virtual bool AddPoint(struct CExtern_Pnt* pNewItem) { return RSU_NOT_USED; }
		//virtual CExtern_Pnt* FindPoint(const char* PntName) { return nullptr; }
		//virtual CExtern_Pnt* GetPoint(const char* PntName) { return nullptr; }
		virtual int SrcMM(char* DataName, struct IMM* pMM) { return RSU_NOT_USED; }// Обращение к источнику прямых ссылок
		virtual int DstMM(char* DataName, struct IMM* pMM) { return RSU_NOT_USED; }// Вызов получателя прямых ссылок
		virtual double* GetValueAddr() { return RSU_NOT_USED; }
		virtual int ResetVariable() { return RSU_NOT_USED; }
		virtual char* GetModelName() { return RSU_NOT_USED; }
		virtual bool GetParamsList(struct CParams** pParams, int* kParams) { return RSU_NOT_USED; }
	};

	typedef std::shared_ptr<RsuBaseModel> PRsuModel;

	class RsuClientsHolder {
		std::vector<PRsuModel> m_Models;
		RsuClientsLdrHelper m_helper;
	public:
		int ErrorCount() {
			int ret = 0;
			for (auto& i : m_Models)
				if (!i->InitState())
					++ret;
			return ret;
		}
		const char* project_path()
		{
			return m_helper.project_path();
		}
		const std::vector<PRsuModel>& models()
		{
			return m_Models;
		}
		IBaseModel* get_client(const std::string& a_LibName, const std::string& a_LibCfg, const std::string& a_ObjName)
		{
			for (auto& m : m_Models)
			{
				if (m->LibName() == a_LibName && m->LibCfg() == a_LibCfg && m->ObjName() == a_ObjName)
					return &(*m);
			}
			return nullptr;
		}
		IBaseModel *create_client(const std::string& a_LibName, const std::string& a_LibCfg, const std::string& a_ObjName, bool& o_ValidLib)
		{
			IBaseModel* ret = get_client(a_LibName, a_LibCfg, a_ObjName);
			if (ret)
				o_ValidLib = true;
			else
			{		
				const std::string lp = "";
				RsuClientAPI* v_api = new RsuClientAPI();
				bool v_res = m_helper.create_client(a_LibName.c_str(), a_LibCfg.c_str(), a_ObjName.c_str(), lp.c_str(), v_api, o_ValidLib) == 0;
				if (v_res)
				{
					PRsuModel p = std::make_shared <RsuBaseModel>(a_LibName, a_LibCfg, a_ObjName, lp, v_api);
					m_Models.emplace_back(std::move(p));
					ret = &(*m_Models.back());
				}
				else
					delete v_api;
			}
			return ret;
		}
		bool show_client(const std::string& a_LibName, const std::string& a_ObjName)
		{
			return m_helper.show_object(a_LibName.c_str(), a_ObjName.c_str());
		}
		void cleanup()
		{
			m_helper.cleanup();
		}
		std::string get_last_error()
		{
			return m_helper.ldr_error();
		}

	};
}
