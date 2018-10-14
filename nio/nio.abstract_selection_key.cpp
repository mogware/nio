#include "nio.abstract_selection_key.h"
#include "nio.abstract_selector.h"

nio::abstract_selection_key::abstract_selection_key(void)
	: is_valid_(true)
{
}

nio::abstract_selection_key::~abstract_selection_key(void)
{
}

void nio::abstract_selection_key::cancel(void)
{
	if (is_valid_) {
		is_valid_ = false;
		abstract_selector* as = nullptr;
		if (selector()->query(IID_PPV_ARG(abstract_selector, &as)))
			as->cancel(shared_from_this());
	}
}

bool nio::abstract_selection_key::is_valid(void) const
{
	return is_valid_;
}
