#include "extensions/filters/http/user_agent/user_agent_filter.h"

#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.h"
#include "envoy/stats/scope.h"

#include "common/http/utility.h"

#include "extensions/filters/http/well_known_names.h"

#include <cstdint>
#include <memory>
#include <string>

#include "envoy/network/connection.h"
#include "envoy/stats/scope.h"

#include "common/http/headers.h"

namespace Envoy {

namespace Extensions {

namespace HttpFilters {

namespace UserAgent {

UserAgentContext::UserAgentContext(Stats::SymbolTable& symbol_table)
    : symbol_table_(symbol_table), pool_(symbol_table),
      prefix_(pool_.add("http")),
      downstream_cx_length_ms_(pool_.add("downstream_cx_length_ms")),
      ios_(pool_.add("user_agent.ios")), android_(pool_.add("user_agent.android")),
      downstream_cx_total_(pool_.add("downstream_cx_total")),
      downstream_cx_destroy_remote_active_rq_(pool_.add("downstream_cx_destroy_remote_active_rq")),
      downstream_rq_total_(pool_.add("downstream_rq_total")) {}

UserAgentStats::UserAgentStats(Stats::StatName prefix, Stats::StatName device, Stats::Scope& scope,
                               const UserAgentContext& context)
    : downstream_cx_total_(scope.counterFromStatName(Stats::StatName(
          context.symbol_table_.join({prefix, device, context.downstream_cx_total_}).get()))),
      downstream_cx_destroy_remote_active_rq_(scope.counterFromStatName(Stats::StatName(
          context.symbol_table_
              .join({prefix, device, context.downstream_cx_destroy_remote_active_rq_})
              .get()))),
      downstream_rq_total_(scope.counterFromStatName(Stats::StatName(
          context.symbol_table_.join({prefix, device, context.downstream_rq_total_}).get()))),
      downstream_cx_length_ms_(scope.histogramFromStatName(
          Stats::StatName(
              context.symbol_table_.join({prefix, device, context.downstream_cx_length_ms_}).get()),
          Stats::Histogram::Unit::Milliseconds)) {
  downstream_cx_total_.inc();
  downstream_rq_total_.inc();
}


struct UserAgentRegistry : public StreamInfo::FilterState::Object {
  uint64_t active_streams = 0;
};

const std::string& userAgentRegistryKey() { CONSTRUCT_ON_FIRST_USE(std::string, "user_agent"); }

Http::FilterHeadersStatus UserAgentFilter::decodeHeaders(Http::RequestHeaderMap& headers, bool) {
  ENVOY_LOG(debug, "HELLO INSIDE DECODE HEADERS!!!!");

  // We assume that the user-agent is consistent based on the first request.

  absl::string_view key = userAgentRegistryKey();
  const StreamInfo::FilterStateSharedPtr& filter_state = callbacks_->streamInfo().filterState();

  if (filter_state->hasData<UserAgentRegistry>(key)) {
    UserAgentRegistry& registry = filter_state->getDataMutable<UserAgentRegistry>(key);
    registry.active_streams += 1;
  } else {
    const Http::HeaderEntry* user_agent = headers.UserAgent();
    if (user_agent != nullptr) {
      if (user_agent->value().getStringView().find("iOS") != absl::string_view::npos) {
        stats_ = std::make_unique<UserAgentStats>(context_.prefix_, context_.ios_, scope_, context_);
      } else if (user_agent->value().getStringView().find("android") != absl::string_view::npos) {
        stats_ = std::make_unique<UserAgentStats>(context_.prefix_, context_.android_, scope_, context_);
      }

      auto registry_ptr = std::make_unique<UserAgentRegistry>();
      registry_ptr->active_streams = 1;
      filter_state->setData(key, std::move(registry_ptr), StreamInfo::FilterState::StateType::Mutable);
    }

  }

  return Http::FilterHeadersStatus::Continue;
}

void UserAgentFilter::onDestroy() {

  if (stats_ != nullptr) {
  ENVOY_LOG(debug, "HELLO INSIDE ON DESTROY!!!");
    // absl::string_view key = userAgentRegistryKey();
    // const StreamInfo::FilterStateSharedPtr& filter_state = callbacks_->streamInfo().filterState();

    // if (callbacks_->connection()->state != Network::Connection::State::Open ) {
    //     UserAgentRegistry& registry = filter_state->getDataMutable<UserAgentRegistry>(key);
    //     auto active_streams = registry.active_streams;
        

    //     stats_->downstream_cx_destroy_remote_active_rq_.inc();
    //     auto time = callbacks_->connection()->streamInfo().lastDownstreamRxByteReceived()->count();
    //     stats_->downstream_cx_length_ms_.recordValue(time);
    // }

    // if (filter_state->hasData<UserAgentRegistry>(key)) {

    //   else {

    //   }

    //   UserAgentRegistry& registry = filter_state->getDataMutable<UserAgentRegistry>(key);
    //   registry.active_streams -= 1;
    // }

  }
}

// void UserAgentFilter::onEvent(Network::ConnectionEvent event) {
//   ENVOY_LOG(debug, "HELLO INSIDE ON EVENT!!!");

//   if (stats_ != nullptr && event == Network::ConnectionEvent::RemoteClose) {
//     absl::string_view key = userAgentRegistryKey();
//     const StreamInfo::FilterStateSharedPtr& filter_state = callbacks_->streamInfo().filterState();

//     if (filter_state->hasData<UserAgentRegistry>(key)) {
//       UserAgentRegistry& registry = filter_state->getDataMutable<UserAgentRegistry>(key);
//       auto active_streams = registry.active_streams;
//       if (active_streams > 0) {
//         stats_->downstream_cx_destroy_remote_active_rq_.inc();
//       }
//     }


//     auto time = callbacks_->connection()->streamInfo().lastDownstreamRxByteReceived()->count();
//     stats_->downstream_cx_length_ms_.recordValue(time);
//   }
// }

	void UserAgentFilter::setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) {
    callbacks_ = &callbacks;
  }

} // namespace UserAgent
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy