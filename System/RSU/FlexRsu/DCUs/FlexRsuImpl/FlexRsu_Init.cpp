#include <crossplatform.h>
#include "FlexRsu.h"
#include <locale.h>
#include <rsus/rsuPaths.h>
#include <macros/FileFind.h>
#include "BlockCreator.h"
#include <FlexRsuConnect.h>
#include <macros/AutoCloser.h>
//#include <io.h>
#include <FlexRsuCommon.h>
#include <rsuNoNames.h>
#include <string>
#include <dylib.hpp>
#include "./../FlexRsuCore/LibImplContainer.h"

using namespace tinyxml2;
using namespace TINYXMLHELPER;

class KReadItem : protected KTinyXmlReader<KReadItem>
{
public:
    struct SProp
    {
        char szName[64 * 4];
        char szValue[256 * 4];
    }m_Props[128];
    int nProps;
public:
    KReadItem()
    {
        id = {};
        szVisibleName = "";
        execOrder = 0;
        nProps = 0;
        //ZeroMemory(this, sizeof(*this));
    }

    GUID id;
    std::string szVisibleName;
    char szType[64];
    int execOrder;
    void Read(tinyxml2::XMLElementPtr& root)
    {
        tHandlersCont_ cont;
        cont["ID"] = &KReadItem::OnID;
        cont["VisibleName"] = &KReadItem::OnVisibleName;
        cont["Type"] = &KReadItem::OnType;
        cont["ExecOrder"] = &KReadItem::OnExecOrder;
        cont["HiddenProperties"] = &KReadItem::OnHiddenProperties;
        KTinyXmlReader<KReadItem>::Read(root, cont, false);
    }

private:
    void OnID(tinyxml2::XMLElementPtr& root)
    {
        GetXMLData(root, id);
    }

    void OnVisibleName(tinyxml2::XMLElementPtr& root)
    {
        GetXMLData(root, szVisibleName);
    }

    void OnType(tinyxml2::XMLElementPtr& root)
    {
        GetXMLData(root, szType);
    }

    void OnExecOrder(tinyxml2::XMLElementPtr& root)
    {
        GetXMLData(root, execOrder);
    }

    void OnHiddenProperties(tinyxml2::XMLElementPtr& root)
    {
        for (tinyxml2::XMLNode* node = root->FirstChild(); node != NULL; node = node->NextSibling())
        {
            ASS(nProps < _countof(m_Props));
            if (nProps == _countof(m_Props))
            {
                //MessageBox( NULL, "Много исходных свойств", "FlexInit", MB_OK );
                break;
            }

            tinyxml2::XMLElementPtr item = node->ToElement();
            LPCSTR name = item->Name();
            LPCSTR value = item->GetText();
            if (!value)
            {
                continue;
            }

            strcpy_s(m_Props[nProps].szName, name);
            strcpy_s(m_Props[nProps].szValue, value);
            ++nProps;
        }
    }
};

class KReadConnection : protected KTinyXmlReader<KReadConnection>
{
    KFlexConnection* m_pcon;
public:
    KReadConnection() : m_pcon(NULL)
    {
    }

    void ReadTo(KFlexConnection& con, tinyxml2::XMLElementPtr& root)
    {
        m_pcon = &con;
        tHandlersCont_ cont;
        cont["SourceID"] = &KReadConnection::OnSourceID;
        cont["SinkID"] = &KReadConnection::OnSinkID;
        cont["SourceConnectorName"] = &KReadConnection::OnSourceConnectorName;
        cont["SinkConnectorName"] = &KReadConnection::OnSinkConnectorName;
        KTinyXmlReader<KReadConnection>::Read(root, cont, false);
    }

private:
    void OnSourceID(tinyxml2::XMLElementPtr& root)
    {
        if (!m_pcon)
        {
            return;
        }

        GetXMLData(root, m_pcon->m_SourceID);
    }

    void OnSinkID(tinyxml2::XMLElementPtr& root)
    {
        if (!m_pcon)
        {
            return;
        }

        GetXMLData(root, m_pcon->m_SinkID);
    }

    void OnSourceConnectorName(tinyxml2::XMLElementPtr& root)
    {
        if (!m_pcon)
        {
            return;
        }

        char szBuf[_countof(m_pcon->m_szSource.text)] = {};
        GetXMLData(root, szBuf);
        m_pcon->m_szSource = szBuf;
    }

    void OnSinkConnectorName(tinyxml2::XMLElementPtr& root)
    {
        if (!m_pcon)
        {
            return;
        }

        char szBuf[_countof(m_pcon->m_szSink.text)] = {};
        GetXMLData(root, szBuf);
        m_pcon->m_szSink = szBuf;
    }
};

void KFlexRsu::GetOutCsvFile(char* pszBuf, size_t _S)
{
}

extern KNoName* g_SharedMemory;
int KFlexRsu::TCoreInitialize(LPCSTR pszCsvPrefix)
{
    g_SharedMemory->Reset();
    InitFlexSystem();
    return Initialize(pszCsvPrefix);
}

int KFlexRsu::Initialize(LPCSTR pszCsvPrefix)
{
    //char* Q = setlocale( LC_ALL, "Russian" );
    //char* pdecpoint = localeconv()->decimal_point;
    //ASSD(pdecpoint && *pdecpoint == '.');

    if (m_pFlexSystem)
    {
        m_pFlexSystem->StartStatus = CFlexSystemA::STATUS_INIT;
    }

    if (0 == m_szDirPrj[0])
    {
        LoadFlexBlocksImpls();
        ns_FlexRsu::GetPath2Logic(m_szDirPrj, m_szObjName);
        ns_FlexRsu::GetPath2Audit(m_szAuditDir, m_szObjName);
        fs::path ap(m_szAuditDir);
        //if( 0!= rsu_cp::cp_access(ap, rsu_cp::cp_path_access::R))
        if (!fs::exists(ap))
        {
            fs::create_directory(ap);
        }
    }
    else
    {
        if (!BeforReInitialize())
        {
            return 0;
        }
    }

    if (ReadProject())
    {
        return 1;
    }

    if (pszCsvPrefix)
    {
        /*
        char szCsvFile[_MAX_PATH] = { NULL };
        _makepath_s( szCsvFile, NULL, rsuGetMemoryPath(), pszCsvPrefix, NULL );
        strcat_s( szCsvFile, m_szObjName );
        strcat_s( szCsvFile, ".csv" );
        char openMode[8] = "wt";
        KAutoCloser<FILE*> fAI(fopen(szCsvFile, openMode), fclose);
        */
        fs::path p = fs::path(rsuGetMemoryPath()) / fs::path(std::string(pszCsvPrefix) + std::string(m_szObjName) + std::string(".csv"));
        FILE* fAI = fopen(p.generic_string().c_str(), "w");
        if (fAI)
        {
            fprintf(fAI, "ED#Точка;ED#Alt;TypeDV#Тип;ST#Описание;LE,Objects#Модель;LS,Type#ТипМодели;Points#ACS;ED#Формула;ST#EngUnit\n");
            DWORD ioMask = KFlexBlockBase::exnInputsIO | KFlexBlockBase::exnOutputsIO;
            for (DWORD d = 0; d < m_nDR; ++d)
            {
                KFlexDR* pDR = m_ppDR[d];
                for (DWORD i = 0; i < pDR->m_nBlocks; ++i)
                {
                    KFlexBlockBase* pBlk = pDR->m_ppBlocks[i];
                    if (pBlk->m_execOrder)
                    {
                        continue;
                    }

                    if (!(pBlk->m_nullOrderExec & ioMask))
                    {
                        continue;
                    }

                    if (2 != strlen(pBlk->m_pszType))
                    {
                        continue;
                    }

                    fprintf(fAI, "%s;;%s;;;;;;\n", (LPCSTR)pBlk->m_szVisibleName, pBlk->m_pszType);
                }
            }

            fclose(fAI);
        }
    }

    return 0;
}

static bool LoadLibs()
{
    static std::vector <std::shared_ptr<dylib>> s_libs;
    static std::vector <std::string> s_libsNames = {
        "FlexRsuBlocks",
        //"FlexRsuOpcIO",
        "FlexRsuModelIO",
        "FlexRsuDcuIO",
        "FlexRsuGraph",
        "FlexRsuAlarmMsg",
        "FlexRsuSimIO"
        /*
        ,
        "FlexRsuTCoreIO",
        "FlexRsuHIMA",
        "FlexRsuSimitIO",
        "FlexRsuTSoftResearch",
        "FlexRsuSura"
        */
    };

    static std::vector <std::string> s_errors;
    if (!s_libs.size())
    {
        auto p = rsuGetExecutionPath();
        for (auto s : s_libsNames)
        {
            s_errors.clear();
            std::string v_Err = s + " error: ";
            try {
                std::shared_ptr<dylib> v_lib = std::make_shared<dylib>(p, s);
                s_libs.push_back(v_lib);
            }
            catch (const dylib::load_error& e) {
                s_errors.push_back(std::string("Load library ") + s + e.what());
            }
            catch (const dylib::symbol_error& e) {
                s_errors.push_back(std::string("Load symbol ") + s + e.what());
            }
            catch (...) {
                s_errors.push_back(s + "Unknow error!");
            }
        }
    }

    return s_errors.size() == 0;
}

void KFlexRsu::LoadFlexBlocksImpls()
{
    LoadLibs();
}

int KFlexRsu::ReadProject()
{
    std::string v_mask = std::string("*.") + ns_FlexRsu::GetLogicFileExtension();
    KFileFind ff(m_szDirPrj, v_mask);
    int countDR = ff.files().size();
    if (!countDR)
    {
        return 0;
    }

    m_ppDR = (KFlexDR**)pFlexMemoryAllocator(sizeof(KFlexDR*) * countDR);
    for (const auto& s : ff.files())
    {
        KFlexDR* pDR = new KFlexDR();
        fs::path fn = fs::path(s).filename().replace_extension();
        snprintf(pDR->m_szFileName, sizeof(pDR->m_szFileName), "%s", fn.generic_string().c_str());
        m_ppDR[m_nDR] = pDR;
        int r = ReadSchema(s.c_str());
        if (r)
        {
            return r;
        }

        ++m_nDR;
    }

    KLibImplContainer::OnProjectReady( this );
    for (DWORD n = 0; n < m_nDR; ++n)
    {
        KFlexDR* pDR = m_ppDR[n];
        pDR->OnProjectReady(this);
    }

    return 0;
}

int KFlexRsu::ReadSchema(LPCSTR pszFile)
{
    tinyxml2::XMLDocument doc;
    XMLError err = doc.LoadFile(pszFile);
    if (XML_SUCCESS != err)
    {
        return 1;
    }

    XMLElementPtr root = doc.RootElement();
    LPCSTR element_name = root->Name();
    if (strcmp(element_name, "FbdLogic") != 0)
    {
        //MessageBox( NULL, "Не реализованная логика", m_szObjName, MB_OK | MB_TOPMOST );
        return 2;
    }

    tHandlersCont_ cont;
    cont["Items"] = &KFlexRsu::OnItems;
    cont["Connections"] = &KFlexRsu::OnConnections;
    KTinyXmlReader<KFlexRsu>::Read(root, cont);
    KFlexDR* pDR = m_ppDR[m_nDR];
    pDR->LinkConnections();
    return 0;
}

int compareFbBlockBynumbOrder(const void* elem1, const void* elem2)
{
    KFlexBlockBase* bl1 = *(KFlexBlockBase**)elem1;
    KFlexBlockBase* bl2 = *(KFlexBlockBase**)elem2;
    if (bl1->m_execOrder < bl2->m_execOrder)
    {
        return -1;
    }

    if (bl1->m_execOrder > bl2->m_execOrder)
    {
        return 1;
    }

    return 0;
}

void KFlexRsu::OnItems(tinyxml2::XMLElementPtr& root)
{
    KFlexDR* pDR = m_ppDR[m_nDR];
    int nBlocks = 0;
    for (tinyxml2::XMLNode* node = root->FirstChild(); node != NULL; node = node->NextSibling())
    {
        tinyxml2::XMLElementPtr item = node->ToElement();
        LPCSTR element_name = item->Name();
        ASS(!strcmp(element_name, "Item"));
        ++nBlocks;
    }

    if (!nBlocks)
    {
        return;
    }

    pDR->m_ppBlocks = (KFlexBlockBase**)pFlexMemoryAllocator(sizeof(KFlexBlockBase*) * nBlocks);
    tHandlersCont_ cont;
    cont["Item"] = &KFlexRsu::OnItem;
    KTinyXmlReader<KFlexRsu>::Read(root, cont);
    qsort(pDR->m_ppBlocks, pDR->m_nBlocks, sizeof(KFlexBlockBase**), compareFbBlockBynumbOrder);
}

void KFlexRsu::OnItem(tinyxml2::XMLElementPtr& root)
{
    KFlexDR* pDR = m_ppDR[m_nDR];
    KReadItem item;
    item.Read(root);
    if (!strcmp(item.szType, "COMMENT"))
    {
        return;
    }

    KFlexBlockBase* pBlock = KBlockCreator::Create(item.szType);
   
    ASS(pBlock);
    if (!pBlock)
    {
        return;
    }

    pDR->m_ppBlocks[pDR->m_nBlocks++] = pBlock;
    pBlock->m_execOrder = item.execOrder;
    pBlock->m_szVisibleName = item.szVisibleName.c_str();
    pBlock->m_id = item.id;
    pBlock->OnCreated();
    for (int i = 0; i < item.nProps; ++i)
    {
        KReadItem::SProp& prop = item.m_Props[i];
        pBlock->AssignValue(prop.szName, prop.szValue);
    }
    
    pBlock->OnAttributesSetted();
}

void KFlexRsu::OnConnections(tinyxml2::XMLElementPtr& root)
{
    KFlexDR* pDR = m_ppDR[m_nDR];
    int nC = 0;
    for (tinyxml2::XMLNode* node = root->FirstChild(); node != NULL; node = node->NextSibling())
    {
        tinyxml2::XMLElementPtr item = node->ToElement();
        LPCSTR element_name = item->Name();
        ASS(!strcmp(element_name, "Connection"));
        ++nC;
    }

    if (!nC)
    {
        return;
    }

    pDR->m_ppConnections = (KFlexConnection**)pFlexMemoryAllocator(sizeof(KFlexConnection*) * nC);
    tHandlersCont_ cont;
    cont["Connection"] = &KFlexRsu::OnConnection;
    KTinyXmlReader<KFlexRsu>::Read(root, cont);
    ASS(nC == pDR->m_nCons);
}

void KFlexRsu::OnConnection(tinyxml2::XMLElementPtr& root)
{
    KFlexDR* pDR = m_ppDR[m_nDR];
    KFlexConnection* pCon = new KFlexConnection;
    ASS(pCon);
    pDR->m_ppConnections[pDR->m_nCons++] = pCon;
    if (!pCon)
    {
        return;
    }

    KReadConnection rc;
    rc.ReadTo(*pCon, root);
}

bool KFlexRsu::BeforReInitialize()
{
    if (!pFlexMemoryResetter)
    {
        return false;
    }

    pFlexMemoryResetter();
    m_ppDR = NULL;
    m_nDR = 0;
    return true;
}
