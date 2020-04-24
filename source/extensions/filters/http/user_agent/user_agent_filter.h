#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "envoy/http/header_map.h"
#include "envoy/network/connection.h"
#include "envoy/stats/scope.h"
#include "envoy/stats/stats_macros.h"
#include "common/stats/symbol_table_impl.h"

#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.h"
#include "envoy/http/filter.h"

// #include "extensions/filters/http/user_agent/user_agent_filter.h"
// #include "extensions/filters/http/user_agent/user_agent_stats.h"

namespace Envoy {

namespace Extensions {

namespace HttpFilters {

namespace UserAgent {

  /**
 * Captures the stat tokens used for recording user-agent stats. These are
 * independent of scope.
 */
struct UserAgentContext {
  UserAgentContext(Stats::SymbolTable& symbol_table);

  Stats::SymbolTable& symbol_table_;
  Stats::StatNamePool pool_;
  Stats::StatName prefix_;
  Stats::StatName downstream_cx_length_ms_;
  Stats::StatName ios_;
  Stats::StatName android_;
  Stats::StatName downstream_cx_total_;
  Stats::StatName downstream_cx_destroy_remote_active_rq_;
  Stats::StatName downstream_rq_total_;
};

/**
 * Captures the stats (counters and histograms) for user-agents. These are
 * established within a stats scope. You must supply a UserAgentContext so that
 * none of the symbols have to be looked up in the symbol-table in the
 * request-path.
 */
struct UserAgentStats {
  UserAgentStats(Stats::StatName prefix, Stats::StatName device, Stats::Scope& scope,
                 const UserAgentContext& context);

  Stats::Counter& downstream_cx_total_;
  Stats::Counter& downstream_cx_destroy_remote_active_rq_;
  Stats::Counter& downstream_rq_total_;
  Stats::Histogram& downstream_cx_length_ms_;
};


class UserAgentFilter : public Http::StreamDecoderFilter, 
												Logger::Loggable<Logger::Id::user_agent> {
public:
  UserAgentFilter(const UserAgentContext& context, Stats::Scope& scope)
      : context_(context), scope_(scope) {}
	
 // Http::StreamDecoderFilter
  Http::FilterHeadersStatus decodeHeaders(Http::RequestHeaderMap&, bool end_stream) override;
	void setDecoderFilterCallbacks(Http::StreamDecoderFilterCallbacks& callbacks) override;

  Http::FilterDataStatus decodeData(Buffer::Instance&, bool) override {
    return Http::FilterDataStatus::Continue;
  }

  Http::FilterTrailersStatus decodeTrailers(Http::RequestTrailerMap&) override {
    return Http::FilterTrailersStatus::Continue;
  }

  	// Network::ConnectionCallbacks
  // void onEvent(Network::ConnectionEvent event) override;
  // void onAboveWriteBufferHighWatermark() override {}
  // void onBelowWriteBufferLowWatermark() override {}

    // Http::StreamFilterBase
  void onDestroy() override;

private:
  std::unique_ptr<UserAgentStats> stats_;
  const UserAgentContext& context_;
  Stats::Scope& scope_;
  Http::StreamDecoderFilterCallbacks* callbacks_{};
};

} // namespace UserAgent
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
