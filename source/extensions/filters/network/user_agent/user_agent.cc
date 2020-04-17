#include "extensions/filters/network/user_agent/user_agent.h"

#include "envoy/buffer/buffer.h"
#include "envoy/network/connection.h"

#include "common/common/assert.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace UserAgent {

Network::FilterStatus UserAgentFilter::onData(Buffer::Instance& data, bool end_stream) {
  ENVOY_LOG(debug, "USER AGENT FILTER LOG!!!");
  ENVOY_LOG(debug, "user agent ondata: {} bytes available", data.length());

  if (!end_stream) {
    ENVOY_LOG(debug, "NOT ENDING STREAM!!!");
  }
  const Http::RequestHeaderMap* headers;
  // headers = read_callbacks_->connection().streamInfo().getRequestHeaders();
  headers = read_callbacks_->connection().streamInfo().getRequestHeaders();
  if (headers) {
    const Http::HeaderEntry* user_agent;
    user_agent = headers->UserAgent();
  }
  // ENVOY_LOG(debug, "USER AGENT LOG!!!", headers->UserAgent()->value().getStringView());
  // read_callbacks_->connection().write(data, end_stream);
  // ASSERT(0 == data.length());
  return Network::FilterStatus::Continue;
}

} // namespace UserAgent
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
