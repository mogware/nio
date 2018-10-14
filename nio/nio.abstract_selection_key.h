#ifndef __NIO_ABSTRACT_SELECTION_KEY__
#define __NIO_ABSTRACT_SELECTION_KEY__

#include "nio.selection_key.h"

namespace nio
{
	class abstract_selection_key : public selection_key,
		public std::enable_shared_from_this<abstract_selection_key>
	{
		volatile bool is_valid_;
	public:
		abstract_selection_key(void);
		virtual ~abstract_selection_key(void);
	public:
		void cancel(void);
		bool is_valid(void) const;
		friend class abstract_selector;
	};
}

#endif

