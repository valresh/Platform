#include "Y_MainLogic.h"
#include <vector>
#include <filesystem>
#include <yokoHeaders.h>
#include <ConnectY.h>
#include <macros/AutoCloser.h>
#include <rsuIsType.h>

struct STypeToType
{
    const char* szPRS;
    DWORD y_type;
    const char* model;
    bool bSkipType;
    char type;
};

static const STypeToType types[] =
{
  {"IO_BOOL", id__Z, "Z", false, 'D' },
  {"ECW_B", id__WB, "WB", false, 'D' },
  {"ECW_I", id__WB, "WB", false, 'D' },
  {"BOOL", id__WB, "WB", false, 'D' },
  {"REAL", id__WB, "WB", false, 'A' },
  {"DINT", id__WB, "WB", false, 'D' },
  {"IO_REAL", id__WB, "WB", false, 'A' },
  {"ANLGI", id_S_ANLGI, "S_ANLGI", false, ' ' },
  {"ANN", id__AN, "AN", false, 'D' },
  {"MOB_11", id_S_MOB_11, "S_MOB_11", false, ' ' },
  {"MOB_21", id_S_MOB_21, "S_MOB_21", false, ' ' },
  {"MOB_RS", id_S_MOB_RS, "S_MOB_RS", false, ' ' },
  {"VEL", id_S_VEL, "S_VEL", false, ' ' },
  {"ANLG_S", id_S_ANLG_S, "S_ANLG_S", false, ' ' },
  {"ECW_R", id__WB, "WB", false, 'A' },
  {"ANN_FUP", id__AN, "AN", false, 'D' },
  {"MOA", id_S_MOA, "S_MOA", false, ' ' },
  {"OVR_B", id_S_OVR_B, "S_OVR_B", false, ' ' },
  {"PASSWD", id_S_PASSWD, "S_PASSWD", false, ' ' },
  {"GOV_B", id_S_GOV_B, "S_GOV_B", false, ' ' },
  {"IO_BOOL", id__Y, "Y", false, 'D' },
};

//#define _trace

bool CY_MainLogic::ReadSCS_O(const char* File, const char* pszScsName)
{
    char* Q = setlocale(LC_ALL, "Russian");

    std::filesystem::path filePath(File);
    std::filesystem::path dir = filePath.parent_path().parent_path();

#ifdef _trace
    char szFileDebugOut[_MAX_PATH] = { 0 };
    sprintf_s(szFileDebugOut, sizeof(szFileDebugOut) - 1, "%sTEMP/%s_P_refs.csv", GetBasePath(), pszScsName);
    std::ofstream of(szFileDebugOut, std::ios_base::out | std::ios_base::trunc);
    of << "!RecNumber;Name;Ref;Type;Y_model;ObjName;Comment\n";
#endif

    std::vector<BYTE> readBuf;
    BYTE* pData = NULL;
    {
        std::fstream hFile(filePath, std::ios::in | std::ios::binary);
        if (!hFile.is_open())
        {
            //Fsend ( "Y_err.txt", "Ошибка открытия %s\n", File );
            return false;
        }
        hFile.seekg(0, std::ios::end);
        DWORD L = hFile.tellg();
        hFile.seekg(0, std::ios::beg);
        readBuf.resize(L);
        pData = &readBuf[0];
        hFile.read(reinterpret_cast<char*>(pData), L);
    }

    SYokogawa200* hdr = (SYokogawa200*)pData;

    ASS(CY_MainLogic::kDR < _countof(DR))

    CY_DR* pDR = new CY_DR;
    CY_MainLogic::DR[CY_MainLogic::kDR++] = pDR;
    strcpy_s(pDR->FCS, "SCS");
    strncat_s(pDR->FCS, pszScsName + 3, 4);
    pDR->NumbFCS = atoi(pszScsName + 5);
    pDR->domain = atoi(pszScsName + 3) / CY_DR::s_dwFcsNumModulus;

    std::filesystem::path path_buffer = std::filesystem::path(dir) / pDR->FCS;
    ReadFcsRef(path_buffer.string().c_str());

    std::vector<BYTE> tagEdf;
    BYTE* pdataTagEdf = NULL;
    path_buffer += "/YOKOGAWA_SCS/INTEGRATION/TAG/Tag.edf";
    {
        std::fstream hFile(path_buffer, std::ios::in | std::ios::binary);
        if (hFile.is_open())
        {
            hFile.seekg(0, std::ios::end);
            DWORD L = hFile.tellg();
            hFile.seekg(0, std::ios::beg);
            tagEdf.resize(L);
            pdataTagEdf = &tagEdf[0];
            hFile.read(reinterpret_cast<char*>(pdataTagEdf), L);
        }
    }

    SYokogawaID* dat = FindYokoID(hdr, "TAGD");
    if (dat)
        HandleTAGD(dat, pData, pDR, pdataTagEdf);

    readBuf.clear();
    pData = NULL;
    tagEdf.clear();
    pdataTagEdf = NULL;
    {
        path_buffer = std::filesystem::path(dir) / pDR->FCS / "/YOKOGAWA_SCS/SAFETY/CONFIGURATION/LinkTrans.edf";

        std::fstream hFile(path_buffer, std::ios::in | std::ios::binary);
        if (!hFile.is_open())
        {
            //Fsend ( "Y_err.txt", "Ошибка открытия %s\n", File );
            return false;
        }
        hFile.seekg(0, std::ios::end);
        DWORD L = hFile.tellg();
        hFile.seekg(0, std::ios::beg);
        readBuf.resize(L);
        pData = &readBuf[0];
        hFile.read(reinterpret_cast<char*>(pData), L);
    }

    hdr = (SYokogawa200*)pData;
    dat = FindYokoID(hdr, "SLWR");
    if (dat)
        HandleSLWR(dat, pData, pDR);

    ManualCreateSystemSWs(pszScsName);
    return true;
}

void CY_MainLogic::AddSystemSW(int NumbSCS, int domain, int NumbSW, LPCSTR pszComment /*= NULL*/)
{
    CY_DI* pDI = FindDI(NumbSCS, domain, NumbSW, id__SW);
    if (pDI)
        return;

    char szSwName[64];
    sprintf_s(szSwName, "%%SW%04d", NumbSW);
    pDI = CreateDobj(szSwName, id__SW, NumbSCS, domain, NumbSW);
    if (!pDI)
        return;
    pDI->Numb = NumbSW;
    strcpy_s(pDI->Name, szSwName);
    strcpy_s(pDI->Model, "SW");
    pDI->NumbFCS = NumbSCS;
    pDI->domain = domain;
    pDI->pointTypeZ = CY_DI::eptDigital;
    pDI->IOZ = 'O';
    pDI->modelTypeZ = CY_DI::eptDigital;
    ASS(CY_MainLogic::kD_Obj < _countof(pD_Obj));
    CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
    pDI->SetObjName(m_domain * CY_DR::s_dwFcsNumModulus + NumbSCS, 'S');
    if (!pszComment)
        strcpy_s(pDI->Comment, "Common Switches for System");
    else
        strncpy_s(pDI->Comment, pszComment, _TRUNCATE);
}

void CY_MainLogic::ManualCreateSystemSWs(LPCSTR pszSCS)
{
    const int NumbSCS = atoi(pszSCS + 3) % CY_DR::s_dwFcsNumModulus;
    const int domain = atoi(pszSCS + 3) / CY_DR::s_dwFcsNumModulus;

    for (int i = 33; i <= 42; ++i)
        AddSystemSW(NumbSCS, domain, i, "Status of nodes 1 to 16");
    for (int i = 65; i <= 72; ++i)
        AddSystemSW(NumbSCS, domain, i, "CPU idle time per minute (seconds)");
    for (int i = 73; i <= 80; ++i)
        AddSystemSW(NumbSCS, domain, i, "Communication load per second (%)");
    for (int i = 97; i <= 104; ++i)
        AddSystemSW(NumbSCS, domain, i, "CPU node input/output module status");
    for (int i = 105; i <= 176; ++i)
        AddSystemSW(NumbSCS, domain, i, "Status of i/o module (each of the 8 slots) of the 2nd to the 10th nodes (I/O nodes)");
    //for( int i=177; i<=208; ++i )
    //  AddSystemSW( NumbSCS, domain, i, "Status of i/o module (each of the 8 slots) of the 11th to the 14th nodes (I/O nodes)" );
}

#include <prsheaders.h>
void CY_MainLogic::HandleSLWR(SYokogawaID* dat, BYTE* pData, CY_DR* pDR)
{
    if (!dat)
        return;
    bool bitCreated[128] = { false };
    const int Type = id__GS;
    char name[20];
    LPCSTR pszFormat = "%%GS%03d%02d";
    DWORD maxBitNumb = 0;

    for (DWORD i = 0; i < dat->dwCount; i++)
    {
        SLinkSnd* pLink = (SLinkSnd*)dat->Item(pData, i);

        if (!pLink->name[0])
            continue;

        ASSD(pLink->BitNo < _countof(bitCreated));
        if (pLink->BitNo < _countof(bitCreated) - 1)
            bitCreated[pLink->BitNo] = true;
        maxBitNumb = __max(maxBitNumb, pLink->BitNo);

        sprintf_s(name, pszFormat, pLink->BitNo, pDR->NumbFCS);
        CY_DI* pDI = CreateDobj(name, Type, pDR->NumbFCS, pDR->domain, pLink->BitNo);
        pDI->Numb = pLink->BitNo;
        strncpy_s(pDI->Name, name, 16);
        pDI->Name[16] = 0;
        pDI->Y_Type = Type;
        strcpy_s(pDI->Model, "GS");
        pDI->NumbFCS = pDR->NumbFCS;
        pDI->domain = pDR->domain;
        strcpy_s(pDI->Comment, pLink->name);
        pDI->pointTypeZ = CY_DI::eptDigital;
        pDI->IOZ = 'I';//по отношению к FCS

        ASS(CY_MainLogic::kD_Obj < _countof(pD_Obj));
        CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
        pDI->SetObjName(m_domain * CY_DR::s_dwFcsNumModulus + pDR->NumbFCS);
    }

    for (DWORD i = 1; i < maxBitNumb; i++)
    {
        if (bitCreated[i])
            continue;

        sprintf_s(name, pszFormat, i, pDR->NumbFCS);
        CY_DI* pDI = CY_MainLogic::FindDI(pDR->NumbFCS, pDR->domain, i, Type);
        if (pDI)
            continue;
        pDI = CreateDobj(name, Type, pDR->NumbFCS, pDR->domain, i);
        pDI->Numb = i;
        strncpy_s(pDI->Name, name, 16);
        pDI->Name[16] = 0;
        pDI->Y_Type = Type;
        strcpy_s(pDI->Model, "GS");
        pDI->NumbFCS = pDR->NumbFCS;
        pDI->domain = pDR->domain;
        strcpy_s(pDI->Comment, "созданна вручную");
        pDI->pointTypeZ = CY_DI::eptDigital;
        pDI->IOZ = 'I';//по отношению к FCS

        ASS(CY_MainLogic::kD_Obj < _countof(pD_Obj));
        CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
        pDI->SetObjName(m_domain * CY_DR::s_dwFcsNumModulus + pDR->NumbFCS);
    }
}

static BYTE* GetITag(BYTE* pTagEdfIntegration, LPCSTR pszID, LPCSTR pszName)
{
    if (!pTagEdfIntegration)
        return NULL;
    SYokogawa200* hdr = (SYokogawa200*)pTagEdfIntegration;
    SYokogawaID* dat = FindYokoID(hdr, pszID);
    if (!dat)
        return NULL;

    for (DWORD c = 0; c < dat->dwCount; c++)
    {
        void* p = dat->Item(pTagEdfIntegration, c);
        char szName[32];
        memcpy(szName, p, 16);
        if (strcmp(szName, pszName))
            continue;
        return (BYTE*)p;
    }

    return NULL;
}

void CY_MainLogic::HandleTAGD(SYokogawaID* dat, BYTE* pData, CY_DR* pDR, BYTE* pTagEdfIntegration)
{
    if (!dat)
        return;
    if (dat->dwSize < 0x190)
        return;

    void* p = NULL;
#ifdef _trace
    DWORD recNumber = 0;
#endif

    for (DWORD c = 0; c < dat->dwCount; c++)
    {
#ifdef _trace
        recNumber++;
        of << recNumber << ';';
#endif
        p = dat->Item(pData, c);
        S_190_COMMON* p1A0 = (S_190_COMMON*)p;

        if (p1A0->szName[0] < '%')
            continue;

        char szName[128];
        strncpy_s(szName, p1A0->szName, 16);

        if (!*p1A0->szType || !*p1A0->szRef)
        {
            ASS(*p1A0->szType);
#ifdef _trace
            of << szName << ";;" << p1A0->szType << ";;;Игнорирую, т.к. нет ссылки;\n";
#endif
            continue;
        }

        const STypeToType* pDataType = NULL;
        for (DWORD i = 0; i < _countof(types); i++)
        {
            if (!strcmp(p1A0->szType, types[i].szPRS))
            {
                pDataType = &types[i];
                break;
            }
        }

        ASS(pDataType);
        if (!pDataType)
            continue;

        if (id__Y == p1A0->y_type)
        {
            for (DWORD i = 0; i < _countof(types); i++)
            {
                if (p1A0->y_type == types[i].y_type)
                {
                    pDataType = &types[i];
                    break;
                }
            }
        }

#ifdef _trace
        of << tmp << ';' << p1A0->szRef << ';' << p1A0->szType << ';';
#endif
        if (pDataType->bSkipType)
        {
            BOOL bSkip = TRUE;

            if (bSkip)
            {
#ifdef _trace
                of << ";;;Пропускаю, т.к. будет привязка по инфе из файла;\n";
#endif
                continue;
            }
        }

        //проверка на уже существующий объект
        CY_Base* pB_Old = NULL;
        pB_Old = CY_MainLogic::FindYObjTyped(szName, true, pDR->NumbFCS, pDR->domain, pDataType->y_type);
        if (pB_Old)
        {
            ASSD(0);
#ifdef _trace
            of << pB_Old->Model << ';' << pB_Old->ObjName << ";Пропускаю, т.к. есть похожее имя;\n";
#endif
            continue;
        }
#ifdef _trace
        of << ";;Создаю новый объект;\n";
#endif

        char szBlNumber[48] = { 0 };
        {
            tNamedString_::iterator it = mNumberOfTags.find(szName);
            //ASSD( mNumberOfTags.end()!=it );
            if (mNumberOfTags.end() != it)
                strcpy_s(szBlNumber, it->second.c_str());
            else
                strcpy_s(szBlNumber, szName);
        }

        CY_Base* pB = NULL;
        switch (pDataType->y_type)
        {
        default:
        {
            CY_DI* pDI = CreateDobj(szName, pDataType->y_type, pDR->NumbFCS, pDR->domain, p1A0->ushNumber);
            pDI->IOZ = ' ';
            if ('R' == *pDataType->szPRS)
                pDI->pointTypeZ = CY_DI::eptAnalog;
            else if ('A' == pDataType->type)
                pDI->pointTypeZ = CY_DI::eptAnalog;
            else
                pDI->pointTypeZ = CY_DI::eptDigital;
            ASS(pDI);
            if (p1A0->szComment[0])
                strncpy_s(pDI->Comment, p1A0->szComment, _countof(p1A0->szComment) - 1);
            else if (p1A0->szRef[0])
            {
                strncpy_s(pDI->Comment, p1A0->szRef, __min(_countof(pDI->Comment) - 1, sizeof(p1A0->szRef)));
            }
            pB = pDI;
            ASS(CY_MainLogic::kD_Obj < _countof(pD_Obj));
            pDI->NumbFCS = pDR->NumbFCS;
            pDI->domain = pDR->domain;
            pDI->Numb = p1A0->ushNumber;
            CY_MainLogic::pD_Obj[CY_MainLogic::kD_Obj++] = pDI;
            if (id__AN == pDataType->y_type)
            {
                ASS(p1A0->cAlarmLevel);
                pDI->AlarmLevel = p1A0->cAlarmLevel;
            }
        }
        break;
        case id_S_MOA:
        case id_S_PASSWD:
        case id_ANN_FUP:
        case id_S_ANLG_S:
        case id_S_ANLGI:
        case id_S_MOB_11:
        case id_S_MOB_21:
        case id_S_MOB_RS:
        case id_S_OVR_B:
        case id_S_VEL:
        case id_S_GOV_B:
            pB = CY_Create::Create((char*)pDataType->model, szName, pDR->NumbFCS, pDR->domain);

            pB->SetParmList();

            char lh[20];
            switch (pDataType->y_type)
            {
            case id_S_MOA:
            {
                DWORD* pDW = (DWORD*)p1A0;
                pDW += 80;
                strncpy_s(lh, (char*)pDW, 8);
                CY_S_MOA* pAn = (CY_S_MOA*)pB;
                pAn->W->SH = atos(lh);
                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->SL = atos(lh);
                pDW += 2;

                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->MSH = atos(lh);
                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->MSL = atos(lh);
                pAn->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_ANLG_S:
            {
                DWORD* pDW = (DWORD*)p1A0;
                pDW += 80;
                strncpy_s(lh, (char*)pDW, 8);
                CYS_ANLG_S* pAn = (CYS_ANLG_S*)pB;
                pAn->W->SH = atos(lh);
                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->SL = atos(lh);
                pAn->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_ANLGI:
            {
                DWORD* pDW = (DWORD*)p1A0;
                pDW += 80;
                strncpy_s(lh, (char*)pDW, 8);
                CY_ANLGI* pAn = (CY_ANLGI*)pB;
                pAn->W->SH = atos(lh);
                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->SL = atos(lh);
                pAn->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_VEL:
            {
                DWORD* pDW = (DWORD*)p1A0;
                pDW += 80;
                strncpy_s(lh, (char*)pDW, 8);
                CY_VEL* pAn = (CY_VEL*)pB;
                pAn->W->SH = atos(lh);
                pDW += 2;
                strncpy_s(lh, (char*)pDW, 8);
                pAn->W->SL = atos(lh);
                pAn->W->AlarmLevel = p1A0->cAlarmLevel;;
            }
            break;
            case id_S_OVR_B:
            {
                CY_OVR_B* pO = (CY_OVR_B*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_GOV_B:
            {
                CY_GOV_B* pO = (CY_GOV_B*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_ANN_FUP:
            {
                CY_ANN_FUP* pO = (CY_ANN_FUP*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_PASSWD:
            {
                CYS_PASSWD* pO = (CYS_PASSWD*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            case id_S_MOB_11:
            {
                CY_MOB_11* pO = (CY_MOB_11*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
                BYTE* pI = GetITag(pTagEdfIntegration, "MOB", szName);
                if (pI)
                {
                    BYTE answer_backAlarm = pI[344 + 5];
                    switch (answer_backAlarm)
                    {
                    case 0://No
                    case 1://Both
                    case 2://On ANS+
                    case 3://Off ANS-
                        pO->W->AnswerBack_Alarm = answer_backAlarm;
                        break;
                    }
                }
            }
            break;
            case id_S_MOB_21:
            {
                CY_MOB_21* pO = (CY_MOB_21*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
                BYTE* pI = GetITag(pTagEdfIntegration, "MOB", szName);
                if (pI)
                {
                    BYTE answer_backAlarm = pI[344 + 5];
                    switch (answer_backAlarm)
                    {
                    case 0://No
                    case 1://Both
                    case 2://On ANS+
                    case 3://Off ANS-
                        pO->W->AnswerBack_Alarm = answer_backAlarm;
                        break;
                    }
                }
            }
            break;
            case id_S_MOB_RS:
            {
                CY_MOB_RS* pO = (CY_MOB_RS*)pB;
                pO->W->AlarmLevel = p1A0->cAlarmLevel;
            }
            break;
            default:
                ASS(0);
            }

            ASS(pB);
            pB->pDR = pDR;
            pB->controllerType = CY_Base::ectPAZ;
            pB->SetObjName(pDR->FCS);
            OnYobjFullNameReady(pB);
            if (szBlNumber[0])
                strcpy_s(pB->szNumber, szBlNumber);
            CY_MainLogic::AddYObj(pB);
            continue;
        }
        ASS(pB);
        if (!pB)
            continue;

        if (szBlNumber[0])
            strcpy_s(pB->szNumber, szBlNumber);

        pB->controllerType = CY_Base::ectPAZ;
        pB->SetObjName(pDR->FCS);
        OnYobjFullNameReady(pB);
        strcpy_s(pB->Model, pDataType->model);
    }
}
