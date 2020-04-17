#pragma once

#include "envoy/network/filter.h"

#include "common/common/logger.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace UserAgent {

/**
 * Implementation of a basic UserAgent filter.
 */
class UserAgentFilter : public Network::ReadFilter, Logger::Loggable<Logger::Id::filter> {
public:
  // Network::ReadFilter
  Network::FilterStatus onData(Buffer::Instance& data, bool end_stream) override;
  Network::FilterStatus onNewConnection() override { 
    ENVOY_LOG(debug, "**********************************USER AGENT FILTER LOG!!!**********************************************");
    return Network::FilterStatus::Continue; 
    }
  void initializeReadFilterCallbacks(Network::ReadFilterCallbacks& callbacks) override {
    read_callbacks_ = &callbacks;
  }

private:
  Network::ReadFilterCallbacks* read_callbacks_{};
};

} // namespace UserAgent
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
