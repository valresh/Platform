#include "Queue.h"
#include "../Lang.h"
#include <string>
#include <sstream>
//
enum eDataType
{
	DataBool,
	DataInt,
	DataDouble
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Структура работы с задвижкой
struct Queue_DataST : public Queue_PRS
{
	bool bValue;
	int nValue;
	double dValue;
	eDataType dataType;
	//
	bool bCheckZacepka;
	// vladexl CShKlapan sh;
	bool mZacepka;      //текущее состояние зацепления
	bool  m_PrevZacepka; //предыдущее состояние зацепления
	double dDelta;      //между значением положения дублера и реальным положнием - вычисляется в момент зацепления 
	SPriborBase::MINMAX m_MinMax; //значения пределов для PribDouble c учетом dDelta для коррекции пределов вращния
	//
	char szName[64];
	char szParam[64];
	//
};

const char HookOn[] = "#Зацепление";
const char PositionDubl[] = "#Положение колеса дублера";

//// Функция отображения
int PaintDataST(DefinePaint)
{
	QueueP(Queue_DataST);
	if (!p->m_bPressed)
	{
		if (q.m_pSocket->IsConnected())
		{
			switch (p->dataType)
			{
			case DataBool:
				q.m_pSocket->GetParam(p->szName, p->szParam, p->bValue);
				p->send.bOld = p->bValue;
				break;
			case DataInt:
				q.m_pSocket->GetParam(p->szName, p->szParam, p->nValue);
				p->send.nOld = p->nValue;
				break;
			case DataDouble:
				q.m_pSocket->GetParam(p->szName, p->szParam, p->dValue);
				p->send.dOld = p->dValue;
				break;
			}
		}
	}

	if (p->pSocket->IsConnected())
	{
		if (p->bCheckZacepka)
		{
			p->m_PrevZacepka = p->mZacepka;
			p->pSocket->GetParam(p->szName, HookOn, p->mZacepka);
			CShKlapan* sh = (CShKlapan*)p->pSocket->Sh(p->nSocketNumber);
			p->pSocket->GetParam(p->szName, PositionDubl, p->dValue);
			p->dDelta = p->dValue - sh->Vent;
			if (p->mZacepka)
			{
				p->m_MinMax.min = 0. + p->dDelta;
				p->m_MinMax.max = 100. + p->dDelta;
			}
			else
			{
				p->m_MinMax.min = -1000000.;
				p->m_MinMax.max = 1000000.;
			}
		}
	}

	return 1;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static void fnSendData(void* base, void* pVoid, void* pOldValue, bool bMessage)
{
	ASSERT(base);
	Queue_DataST* p = (Queue_DataST*)base;
	if (p->pSocket->IsConnected())
	{
		//if (pVoid == &p->bValue)
		if (p->dataType == DataBool)
		{
			std::string strOld = *((bool*)pOldValue) ? "1" : "0";
			if (*((bool*)pVoid))
			{
				p->pSocket->SetParam(p->szName, p->szParam, p->szParam, "1", strOld.c_str(), bMessage, id_ModelData, enumValueInt);
			}
			else
			{
				p->pSocket->SetParam(p->szName, p->szParam, p->szParam, "0", strOld.c_str(), bMessage, id_ModelData, enumValueInt);
			}

			if (bMessage)
			{
				p->send.bOld = p->bValue;
			}
		}
		else if (p->dataType == DataInt)
		{
			p->pSocket->GetParam(p->szName, p->szParam, p->nValue);
			p->send.nOld = p->nValue;
			std::ostringstream strs;
			strs << *((int*)pVoid);
			std::string strValue = strs.str();
			strs.str("");
			strs << p->send.nOld;
			std::string strOldValue = strs.str();
			p->pSocket->SetParam(p->szName, p->szParam, p->szParam, strValue.c_str(), strOldValue.c_str(), bMessage, id_ModelData, enumValueInt);
			if (bMessage)
			{
				p->send.nOld = p->nValue;
			}
		}
		else if (p->dataType == DataDouble)
		{
			p->pSocket->GetParam(p->szName, p->szParam, p->dValue);
			p->send.dOld = p->dValue;
			std::ostringstream strs;
			strs << *((double*)pVoid);
			std::string strValue = strs.str();
			strs.str("");
			strs << p->send.dOld;
			std::string strOldValue = strs.str();
			bool bSend = true;
			if (p->bCheckZacepka)
			{
				bSend = p->mZacepka;
				p->send.eType = enumValueDbl;
				CShKlapan* sh = (CShKlapan*)p->pSocket->Sh(p->nSocketNumber);
				if (bSend)
				{
					p->m_MinMax.min = 0. + p->dDelta;
					p->m_MinMax.max = 100. + p->dDelta;
					p->send.SetNew(pVoid);
					if (((sh->Vent >= 100.0) && (p->send.dNew > p->send.dOld)) ||
						((sh->Vent <= 0.0) && (p->send.dNew < p->send.dOld)))
					{
						bSend = false;
					}
				}
				else
				{
					p->m_MinMax.min = -1000000.;
					p->m_MinMax.max = 1000000.;
					if (sh->Vent != p->send.dOld)
					{
						*((double*)pVoid) = p->send.dNew = sh->Vent;
						bSend = true;
						bMessage = false;
					}
				}

				if (bSend)
				{
					//здесь в каждой из переменных szValue и szValueOld посылаем по ДВА значения разделенных ;
					//первое для изменения положения дублера в симуляторе, второе для записи в журнал (с коррецией 
					//на dDelta для для приведения к требуемому положению клапана)
					double dValue = *(double*)pVoid;
					char szValue[32], szValueOld[32];
					double dtrueNew = dValue - p->dDelta;
					double dOld = p->send.dOld - p->dDelta;
					if (dtrueNew < 1.)
					{
						dtrueNew = 0.;
					}

					if (dtrueNew > 99.)
					{
						dtrueNew = 100.;
					}

					sprintf_s(szValue, "%0.5f;%0.5f", dValue, dtrueNew);
					sprintf_s(szValueOld, "%0.5f;%0.5f", p->send.dOld, dOld);
					p->pSocket->SetParam(p->szName, p->szParam, p->szParam, szValue, szValueOld, bMessage, id_ModelData, p->send.eType);
					bSend = false; //здесь послали - не посылать ниже
					p->send.dOld = dValue;
				}
			}

			if (bSend)
			{
				p->pSocket->SetParam(p->szName, p->szParam, p->szParam, strValue.c_str(), strOldValue.c_str(), bMessage, id_ModelData, enumValueDbl);
				if (bMessage)
				{
					p->send.dOld = p->dValue;
				}
			}
		}
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void FillData(SPriborBase* d, void* pp)
{
	d->fnData = fnSendData;
	Queue_DataST* p = (Queue_DataST*)pp;
	if ((d->nType == 1) || (d->nType == 14))
	{
		p->dataType = DataBool;
		d->pVoid = &p->bValue;
	}
	else if (d->nType == 10)
	{
		p->dataType = DataInt;
		d->pVoid = &p->nValue;
	}
	else if ((d->nType == 0) || (d->nType == 11) || (d->nType == 15))
	{
		p->dataType = DataDouble;
		d->pVoid = &p->dValue;
	}

	if ((d->nType == 3) || (d->nType == 4))
	{
		p->dataType = DataBool;
		d->eType = enumValueBol;
		d->pVoid = &p->bValue;
	}

	if (p->bCheckZacepka)
	{
		d->pMinMax = &p->m_MinMax;
	}
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void AddDataST(DefineTegs)
{
	//
	AddQueueEx(Queue_DataST);
	//
	//
	// Код, посылаемый в АРМ
	p->btSendType = QUEUE_NO;
	// Определение функции вывода
	p->paint = &PaintDataST;
	p->bValue = false;
	p->nValue = 0;
	p->dValue = 0;
	p->dataType = DataBool;
	p->dDelta = 0.;
	p->m_MinMax.min = 0.;
	p->m_MinMax.max = 100.;
	char szShift[64] = "\0";
	//
	STegObj obj[] =
	{
	{ 'S', "shift", szShift, 63},
	EXT_QUEUE
	}; ::ParserObjScn(teg, obj, sizeof(obj) / sizeof(STegObj));
	//
	strcpy(p->szName, teg->value);
	strcpy(p->szParam, szShift);
	if (strcmp(szShift, PositionDubl) == 0)
	{
		p->bCheckZacepka = true;
	}
	else
	{
		p->bCheckZacepka = false;
	}

	if (strcmp(p->szName, ".") == 0)
	{
		int nActive = def.GetActiveObjSocket();
		const char* name = def.GetModelName(nActive);
		strcpy(p->szName, name);
	}

	auto hPribor = AddPribors(def, teg, Buffer, p, &FillData, id_ModelData);
	managed::RegisterElement(teg->name, p, hPribor);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
