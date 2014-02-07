#ifndef _META_EXCEPTION_H_
#define _META_EXCEPTION_H_

#include <stdexcept>

#define EXCEPTION_CLASS( name ) \
struct name : public std::runtime_error { \
	template <typename T> \
	name(T arg) : std::runtime_error(arg) {} \
};

#endif

