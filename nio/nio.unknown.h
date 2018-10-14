#ifndef __NIO_UNKNOWN__
#define __NIO_UNKNOWN__

#define IID_PPV_ARG(T,E) typeid(T).name(), \
		reinterpret_cast<void**>(static_cast<T**>(E))

namespace nio
{
	struct unknown
	{
		virtual bool query(const char* iid, void** ppv) = 0;
		virtual ~unknown() {}
	};
}

#endif

