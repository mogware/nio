#ifndef __NIO_SELECTOR_IMPL__
#define __NIO_SELECTOR_IMPL__

#include <memory>

#include "nio.abstract_selector.h"
#include "nio.selection_key.h"
#include "sio.h"

namespace nio
{
	class selection_key_impl;
	struct selector_provider;
	class selector_impl : public abstract_selector,
		public std::enable_shared_from_this<selector_impl>
	{
		std::set<std::shared_ptr<selection_key>> keys_;
		std::set<std::shared_ptr<selection_key>> selected_keys_;

		sio::socket_t wakeup_in;
		sio::socket_t wakeup_out;
	public:
		selector_impl(const std::shared_ptr<selector_provider>& provider);
		virtual ~selector_impl();
	public:
		const std::set<std::shared_ptr<selection_key>>& keys(void) const;
	public:
		int select(void);
		int select(const long& timeout);
		std::set<std::shared_ptr<selection_key>>& selected_keys(void);
		int select_now(void);
		void wakeup(void) const;
	protected:
		std::shared_ptr<selection_key> enroll(
			const std::shared_ptr<abstract_selectable_channel>& ch,
			const int& ops, const std::shared_ptr<unknown>& attachment);
	protected:
		void implement_close_selector(void);
	public:
		bool query(const char* iid, void** ppv);
	private:
		void check_closed(void) const;
		int select_internal(const long& timeout);
		int do_cancel(void);
	private:
		int poll(const long& timeout);
		static sio::socket_t get_native_socket(const std::shared_ptr<selectable_channel>& ch);
	};
}

#endif
