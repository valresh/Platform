#include <map>
#include <vector>
#include <mutex>
#include "databuffer.h"
#include "network/ICommandHandler.h"
#include "crossplatform.h"
#include "RsuX.h"
#include "CBase.h"
#include "ShBase.h"
#include "UniBuffer.h"

#ifdef _MSC_VER
	#ifdef SERVEREXBASE_EXPORTS
		#define SERVEREXBASE_API __declspec(dllexport)
	#else
		#define SERVEREXBASE_API __declspec(dllimport)
	#endif
#else
	#ifdef SERVEREXBASE_EXPORTS
		#define SERVEREXBASE_API __attribute__((visibility("default")))
	#else
		#define SERVEREXBASE_API
	#endif
#endif

namespace serverexbase
{
    namespace internal
    {
        class Serializer;
    };

    struct DataEntry
    {
        CBase* pBase;
        EDataTypes mType;
        char       name[128];
        KRsuX pnt;
        DataEntry(CBase* pB, char* pszName, EDataTypes mT, KRsuX& _pnt)
            : pBase(pB)
            , mType(mT)
            , pnt(_pnt)
        {
            strcpy_s(name, pszName);
        };
    };

    class SERVEREXBASE_API CommandHandler : public network::ICommandHandler
    {
    private:
        struct SUniSh : public SUniChar
        {
            SUniSh() : SUniChar(1024)
            {
            };

            CShBase* Sh() const
            {
                return (CShBase*)m_szBuffer;
            };

            bool Resize(int nSize)
            {
                if (nSize <= 0) return false;
                nSize += sizeof(CShBase) * 2;
                if (nSize <= m_nMaxBuff) return true;
                return Realloc(nSize - Count());
            }
        };
        class List
        {
        private:
            std::string _id;

        public:
            List();
            
            const std::string & Id() const;

            std::vector<DataEntry> DataEntries;
            SUniSh mSh;
        };
        internal::Serializer* _serializer;
        std::map<std::string, std::shared_ptr<List>> _lists;
        std::mutex _listsMutex;
        BYTE ModelStatus;

    public:
        CommandHandler();

        virtual ~CommandHandler();

        virtual network::Response OnCommand(network::Commands command, const std::string& clientId, const network::Request& request) override;

    private:
        network::Response OnGetDataCommand(List& list, const databuffer& request, bool isExtendedInfo);
        network::Response OnFindHoney(DWORD rsuID, const databuffer& request);
        network::Response OnUpdateSound(DWORD rsuID, databuffer& request, const std::string & userName);
        network::Response OnClearList(const std::string & listId);
        network::Response OnTrends(databuffer& request);
        network::Response OnSetupList(List& list, bool isExtendedInfo, const databuffer& request);
        network::Response OnRefreshList(List& list, bool isExtendedInfo, std::function<network::Response (const databuffer &)>);
        network::Response OnGetAlarms(DWORD rsuID, databuffer& request);
        network::Response OnGetEvents(DWORD rsuID, databuffer& request);
        network::Response OnGetMsgToO(DWORD rsuID, databuffer& request);
        network::Response Success(const databuffer& databuffer) const;
        network::Response Success() const;
        network::Response Error(const std::string & error) const;
        std::shared_ptr<List> AppendList();
        std::shared_ptr<List> GetList(const std::string& listId);
    };
};