#pragma once

#include <string>

#include "envoy/extensions/filters/network/user_agent/v3/user_agent.pb.h"
#include "envoy/extensions/filters/network/user_agent/v3/user_agent.pb.validate.h"

#include "extensions/filters/network/common/factory_base.h"
#include "extensions/filters/network/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace UserAgent {

/**
 * Config registration for the user agent filter. @see NamedNetworkFilterConfigFactory.
 */
class UserAgentFilterConfigFactory
    : public Common::FactoryBase<envoy::extensions::filters::network::user_agent::v3::UserAgent> {
public:
  UserAgentFilterConfigFactory() : FactoryBase(NetworkFilterNames::get().UserAgent) {}

private:
  Network::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::extensions::filters::network::user_agent::v3::UserAgent& proto_config,
      Server::Configuration::FactoryContext& context) override;
};

} // namespace UserAgent
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
