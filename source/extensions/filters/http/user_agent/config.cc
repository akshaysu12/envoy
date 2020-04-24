#include "extensions/filters/http/user_agent/config.h"
#include "extensions/filters/http/user_agent/user_agent_filter.h"

#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.h"
#include "envoy/extensions/filters/http/user_agent/v3/user_agent.pb.validate.h"
#include "envoy/registry/registry.h"


namespace Envoy {
namespace Extensions {
namespace HttpFilters {
namespace UserAgent {

Http::FilterFactoryCb UserAgentFilterConfigFactory::createFilterFactoryFromProtoTyped(
    const envoy::extensions::filters::http::user_agent::v3::UserAgent& proto_config,
    const std::string& stats_prefix, Server::Configuration::FactoryContext& context) {

  return [&context](Http::FilterChainFactoryCallbacks& callbacks) -> void {
    callbacks.addStreamDecoderFilter(std::make_shared<UserAgentFilter>(context.httpContext().userAgentContext(), context.scope()));
  };
}

/**
 * Static registration for the User Agent filter. @see RegisterFactory
 */
REGISTER_FACTORY(UserAgentFilterConfigFactory,
                 Server::Configuration::NamedHttpFilterConfigFactory);

} // namespace UserAgent
} // namespace HttpFilters
} // namespace Extensions
} // namespace Envoy
