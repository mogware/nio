#ifndef __NIO_CONFIG__
#define __NIO_CONFIG__

#if defined(_MSC_VER)
#define NIO_MSVC
#if !defined(_DEBUG) && !defined(__NIO_INLINE__)
#define __NIO_INLINE__
#endif
#endif

#if defined(__GNUC__)
#define NIO_GCC
#if __GNUC__ < 4
#define NIO_GCC3
#endif
#if !defined (__NIO_INLINE__)
#define __NIO_INLINE__
#endif
#endif

#if defined(NIO_LACKS_INLINE_FUNCTIONS) && !defined(NIO_NO_INLINE)
#define NIO_NO_INLINE
#endif

#if defined(NIO_NO_INLINE)
#undef __NIO_INLINE__
#endif

#if defined(__NIO_INLINE__)
#define NIO_INLINE inline
#else
#define NIO_INLINE
#endif

#endif

