#include "nio.selector_provider.h"
#include "nio.selector_provider_impl.h"

nio::selector_provider::~selector_provider(void)
{
}

std::shared_ptr<nio::selector_provider> nio::selector_provider::provider(void)
{
	static std::shared_ptr<selector_provider_impl> provider =
		std::make_shared<selector_provider_impl>();
	return provider;
}

