#include "Serializer.h"
#include <nlohmann/json.hpp>
#include "BaseType.h"
#include "Common.h"

namespace serverexbase
{
	namespace internal
	{
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MixedResponse, ErrorCode, ErrorMessage);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SendRequest, Command, ClientId, Username);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SetupListRequest, Command, ClientId, OverrideListId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SetupListResponse, ErrorCode, ErrorMessage, ListId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RefreshListRequest, Command, ClientId, ListId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetDataRequest, Command, ClientId, ListId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ClearListRequest, Command, ClientId, ListId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetAlarmsRequest, Command, ClientId, RsuId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetEventsRequest, Command, ClientId, RsuId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GetMsgToOperatorRequest, Command, ClientId, RsuId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FindHoneyRequest, Command, ClientId, RsuId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UpdateSoundRequest, Command, ClientId, Username, RsuId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(BusyRequest, Command, ClientId);
		NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TrendsRequest, Command, ClientId);

		static nlohmann::json Parse(const network::Request& request)
		{
			return nlohmann::json::parse(request.data(), request.data() + request.size());
		}

		static databuffer GetBinaryData(const network::Request& request)
		{
			auto jsonLength = strlen(request.data());
			if (jsonLength >= request.size())
			{
				// Just controling "out of range" and correctness of request.
				// It's not enough, but something at least.
				return databuffer();
			}

			// +1 - \0 at the end of json
			jsonLength++;
			return databuffer(
				(char*)(request.data() + jsonLength),
				request.size() - jsonLength);
		}

		template<class T>
		static T Deserialize(const network::Request& request)
		{
			nlohmann::json j = Parse(request);
			T arg;
			j.get_to(arg);
			return arg;
		}

		static network::Response Serialize(const std::string & jsonResponse, const databuffer* databuffer)
		{
			int length = jsonResponse.size() + 1;
			network::Response networkResponse((const char*)&length, sizeof(length), false);
			networkResponse.append(jsonResponse.c_str(), length);
			if (databuffer)
			{
				// Mixed data: length(4 bytes) + Json + Binary content
				networkResponse.append(databuffer->data(), databuffer->size());
			}

			return networkResponse;
		}

		network::Response Serializer::Serialize(const MixedResponse& mixedResponse)
		{
			auto jsonResponse = nlohmann::json(mixedResponse).dump();
			return internal::Serialize(jsonResponse, mixedResponse.ErrorCode == ErrorCode::Success ? &mixedResponse.Data : nullptr);
		}

		SendRequest Serializer::Send::Deserialize(const network::Request& request)
		{
			auto sendRequest = internal::Deserialize<SendRequest>(request);
			return SendRequest{ sendRequest.Command, sendRequest.ClientId, sendRequest.Username, GetBinaryData(request) };
		}

		GetDataRequest Serializer::GetData::Deserialize(const network::Request& request)
		{
			auto getRequest = internal::Deserialize<GetDataRequest>(request);
			return GetDataRequest{ getRequest.Command, getRequest.ClientId, getRequest.ListId, GetBinaryData(request) };
		}

		SetupListRequest Serializer::SetupList::Deserialize(const network::Request& request)
		{
			auto getListRequest = internal::Deserialize<SetupListRequest>(request);
			return SetupListRequest{ getListRequest.Command, getListRequest.ClientId, getListRequest.OverrideListId, GetBinaryData(request) };
		}

		network::Response Serializer::SetupList::Serialize(const SetupListResponse& response)
		{
			auto jsonResponse = nlohmann::json(response).dump();
			return internal::Serialize(jsonResponse, response.ErrorCode == ErrorCode::Success ? &response.Data : nullptr);
		}

		GetAlarmsRequest Serializer::GetAlarms::Deserialize(const network::Request& request)
		{
			auto getAlarmsRequest = internal::Deserialize<GetAlarmsRequest>(request);
			return GetAlarmsRequest{ getAlarmsRequest.Command, getAlarmsRequest.ClientId, getAlarmsRequest.RsuId, GetBinaryData(request) };
		}

		GetEventsRequest Serializer::GetEvents::Deserialize(const network::Request& request)
		{
			auto getEventsRequest = internal::Deserialize<GetEventsRequest>(request);
			return GetEventsRequest{ getEventsRequest.Command, getEventsRequest.ClientId, getEventsRequest.RsuId, GetBinaryData(request) };
		}

		GetMsgToOperatorRequest Serializer::GetMsgToOperator::Deserialize(const network::Request& request)
		{
			auto getMsgRequest = internal::Deserialize<GetMsgToOperatorRequest>(request);
			return GetMsgToOperatorRequest{ getMsgRequest.Command, getMsgRequest.ClientId, getMsgRequest.RsuId, GetBinaryData(request) };
		}

		FindHoneyRequest Serializer::FindHoney::Deserialize(const network::Request& request)
		{
			auto findHoneyRequest = serverexbase::internal::Deserialize<FindHoneyRequest>(request);
			return FindHoneyRequest{ findHoneyRequest.Command, findHoneyRequest.ClientId, findHoneyRequest.RsuId, GetBinaryData(request) };
		}

		UpdateSoundRequest Serializer::UpdateSound::Deserialize(const network::Request& request)
		{
			auto updateRequest = internal::Deserialize<UpdateSoundRequest>(request);
			return UpdateSoundRequest{ updateRequest.Command, updateRequest.ClientId, updateRequest.Username, updateRequest.RsuId, GetBinaryData(request) };
		}

		BusyRequest Serializer::Busy::Deserialize(const network::Request& request)
		{
			auto busyRequest = internal::Deserialize<BusyRequest>(request);
			return BusyRequest{ busyRequest.Command, busyRequest.ClientId, GetBinaryData(request) };
		}

		TrendsRequest Serializer::Trends::Deserialize(const network::Request& request)
		{
			auto trendsRequest = internal::Deserialize<TrendsRequest>(request);
			return TrendsRequest{ trendsRequest.Command, trendsRequest.ClientId, GetBinaryData(request) };
		}

		RefreshListRequest Serializer::RefreshList::Deserialize(const network::Request& request)
		{
			auto refreshRequest = internal::Deserialize<RefreshListRequest>(request);
			return RefreshListRequest{ refreshRequest.Command, refreshRequest.ClientId, refreshRequest.ListId };
		}
	};
};
