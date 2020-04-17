#include "envoy/extensions/filters/network/user_agent/v3/user_agent.pb.h"
#include "envoy/extensions/filters/network/user_agent/v3/user_agent.pb.validate.h"
#include "envoy/registry/registry.h"
#include "envoy/server/filter_config.h"

#include "extensions/filters/network/common/factory_base.h"
#include "extensions/filters/network/user_agent/user_agent.h"
#include "extensions/filters/network/well_known_names.h"

namespace Envoy {
namespace Extensions {
namespace NetworkFilters {
namespace UserAgent {

/**
 * Config registration for the user_agent filter. @see NamedNetworkFilterConfigFactory.
 */
class UserAgentConfigFactory
    : public Common::FactoryBase<envoy::extensions::filters::network::user_agent::v3::UserAgent> {
public:
  UserAgentConfigFactory() : FactoryBase(NetworkFilterNames::get().UserAgent) {}

private:
  Network::FilterFactoryCb
  createFilterFactoryFromProtoTyped(const envoy::extensions::filters::network::user_agent::v3::UserAgent&,
                                    Server::Configuration::FactoryContext&) override {
    return [](Network::FilterManager& filter_manager) -> void {
      filter_manager.addReadFilter(std::make_shared<UserAgentFilter>());
    };
  }

};

/**
 * Static registration for the user_agent filter. @see RegisterFactory.
 */
REGISTER_FACTORY(UserAgentConfigFactory,
                 Server::Configuration::NamedNetworkFilterConfigFactory){"envoy.user_agent"};

} // namespace UserAgent
} // namespace NetworkFilters
} // namespace Extensions
} // namespace Envoy
