#pragma once
#include <string>
#include "network/Commands.h"
#include "databuffer.h"
#include "crossplatform.h"

namespace serverexbase
{
    namespace internal
    {
        enum ErrorCode
        {
            Success,
            Error
        };

        struct GetDataRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string ListId;
            databuffer Data;
        };

        struct MixedResponse
        {
            MixedResponse(const std::string & errorMessage)
            {
                ErrorCode = ErrorCode::Error;
                ErrorMessage = errorMessage;
            }

            MixedResponse(const databuffer & databuffer)
                : Data(databuffer.data(), databuffer.size())
            {
                ErrorCode = ErrorCode::Success;
            }

            MixedResponse()
            {
                ErrorCode = ErrorCode::Success;
            }

            internal::ErrorCode ErrorCode;
            std::string ErrorMessage;
            databuffer Data;
        };

        struct SendRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string Username;
            databuffer Data;
        };

        struct SetupListRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string OverrideListId;
            databuffer Data;
        };

        struct SetupListResponse : MixedResponse
        {
            SetupListResponse(const std::string & errorMessage)
                : MixedResponse(errorMessage)
            {
            }

            SetupListResponse(const std::string & listId, const databuffer& databuffer)
                : MixedResponse(databuffer)
            {
                ListId = listId;
            }

            SetupListResponse()
            {
            }

            std::string ListId;
        };

        struct RefreshListRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string ListId;
        };

        struct ClearListRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string ListId;
        };

        struct GetAlarmsRequest
        {
            network::Commands Command;
            std::string ClientId;
            uint32_t RsuId;
            databuffer Data;
        };

        struct GetEventsRequest
        {
            network::Commands Command;
            std::string ClientId;
            uint32_t RsuId;
            databuffer Data;
        };

        struct GetMsgToOperatorRequest
        {
            network::Commands Command;
            std::string ClientId;
            uint32_t RsuId;
            databuffer Data;
        };

        struct FindHoneyRequest
        {
            network::Commands Command;
            std::string ClientId;
            uint32_t RsuId;
            databuffer Data;
        };

        struct UpdateSoundRequest
        {
            network::Commands Command;
            std::string ClientId;
            std::string Username;
            uint32_t RsuId;
            databuffer Data;
        };

        struct BusyRequest
        {
            network::Commands Command;
            std::string ClientId;
            databuffer Data;
        };

        struct TrendsRequest
        {
            network::Commands Command;
            std::string ClientId;
            databuffer Data;
        };
    };
};
