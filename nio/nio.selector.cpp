#include "nio.selector.h"
#include "nio.selector_provider.h"

nio::selector::~selector()
{
}

std::shared_ptr<nio::selector> nio::selector::open(void)
{
	return selector_provider::provider()->open_selector();
}