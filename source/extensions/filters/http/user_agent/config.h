#pragma once

#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.h"
#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.validate.h"

#include "extensions/filters/http/common/factory_base.h"
#include "extensions/filters/http/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UserAgent {

/**
 * Config registration for the UserAgent filter. @see NamedHttpFilterConfigFactory.
 */
class UserAgentFilterConfigFactory
    : public Common::FactoryBase<envoy::extensions::filters::http::user_agent::v3::UserAgent> {
public:
  UserAgentFilterConfigFactory() : FactoryBase(HttpFilterNames::get().UserAgent) {}

private:
  Http::FilterFactoryCb createFilterFactoryFromProtoTyped(
      const envoy::extensions::filters::http::user_agent::v3::UserAgent& proto_config,
      const std::string& stats_prefix, Server::Configuration::FactoryContext& context) override;
};

} // namespace UserAgent
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
