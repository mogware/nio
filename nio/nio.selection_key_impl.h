#ifndef __NIO_SELECTION_KEY_IMPL__
#define __NIO_SELECTION_KEY_IMPL__

#include <memory>

#include "nio.abstract_selection_key.h"

namespace nio
{
	struct selectable_channel;
	class selector_impl;
	struct selector;
	struct unknown;
	class selection_key_impl : public abstract_selection_key
	{
		std::shared_ptr<selectable_channel> channel_;
		int interest_ops_;
		int ready_ops_;
		std::shared_ptr<selector_impl> selector_;
	public:
		selection_key_impl(const std::shared_ptr<selectable_channel>& channel,
			const int& operations, const std::shared_ptr<unknown>& object,
			const std::shared_ptr<selector_impl> selector);
		virtual ~selection_key_impl(void);
	public:
		static std::shared_ptr<selection_key> of(const std::shared_ptr<selectable_channel>& channel,
			const int& operations, const std::shared_ptr<unknown>& object,
			const std::shared_ptr<selector_impl> selector);
	public:
		bool query(const char* iid, void** ppv);
	public:
		std::shared_ptr<selectable_channel> channel(void) const;
		int interest_ops(void) const;
		int interest_ops_nocheck(void) const;
		void interest_ops(const int& ops);
		int ready_ops(void) const;
		void ready_ops(const int& ops);
		std::shared_ptr<nio::selector> selector(void) const;
		bool is_connected(void) const;
	private:
		void check_valid() const;
	};
}

#endif


