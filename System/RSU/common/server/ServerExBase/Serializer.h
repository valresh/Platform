#pragma once
#include "Types.h"
#include "network/Request.h"
#include "network/Response.h"

namespace serverexbase
{
    namespace internal
    {
        class Serializer
        {
        public:
            network::Response Serialize(const MixedResponse &);

            struct GetData
            {
                GetDataRequest Deserialize(const network::Request& request);
            } GetData;

            struct Send
            {
                SendRequest Deserialize(const network::Request& request);
            } Send;

            struct SetupList
            {
                SetupListRequest Deserialize(const network::Request& request);
                network::Response Serialize(const SetupListResponse& response);
            } SetupList;

            struct RefreshList
            {
                RefreshListRequest Deserialize(const network::Request& request);
            } RefreshList;

            struct GetAlarms
            {
                GetAlarmsRequest Deserialize(const network::Request& request);
            } GetAlarms;

            struct GetEvents
            {
                GetEventsRequest Deserialize(const network::Request& request);
            } GetEvents;

            struct GetMsgToOperator
            {
                GetMsgToOperatorRequest Deserialize(const network::Request& request);
            } GetMsgToOperator;

            struct FindHoney
            {
                FindHoneyRequest Deserialize(const network::Request& request);
            } FindHoney;

            struct UpdateSound
            {
                UpdateSoundRequest Deserialize(const network::Request& request);
            } UpdateSound;

            struct Busy
            {
                BusyRequest Deserialize(const network::Request& request);
            } Busy;

            struct Trends
            {
                TrendsRequest Deserialize(const network::Request& request);
            } Trends;
        };
    };
};