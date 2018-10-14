#ifndef __NIO_SELECTION_KEY__
#define __NIO_SELECTION_KEY__

#include <memory>

#include "nio.unknown.h"

namespace nio
{
	struct selectable_channel;
	struct selector;
	class selection_key : public unknown
	{
		std::shared_ptr<unknown> attachment_;
	public:
		enum { op_read = 1, op_write = 4, op_connect = 8, op_accept = 16 };
	public:
		selection_key(void);
		virtual ~selection_key(void);
	public:
		std::shared_ptr<unknown> attach(const std::shared_ptr<unknown>& object);
		std::shared_ptr<unknown> attachment(void) const;
	public:
		bool is_acceptable(void) const;
		bool is_connectable(void) const;
		bool is_readable(void) const;
		bool is_writable() const;
	public:
		virtual void cancel(void) = 0;
		virtual std::shared_ptr<selectable_channel> channel(void) const = 0;
		virtual int interest_ops(void) const = 0;
		virtual int interest_ops_nocheck(void) const = 0;
		virtual void interest_ops(const int& ops) = 0;
		virtual bool is_valid(void) const = 0;
		virtual int ready_ops(void) const = 0;
		virtual void ready_ops(const int& ops) = 0;
		virtual std::shared_ptr<nio::selector> selector(void) const = 0;
		virtual bool is_connected(void) const = 0;
	public:
		selection_key(const selection_key&) = delete;
		selection_key& operator =(const selection_key&) = delete;
	};
}

#endif



