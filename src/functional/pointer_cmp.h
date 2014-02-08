#ifndef _FUNCTIONAL_POINTER_CMP_H_
#define _FUNCTIONAL_POINTER_CMP_H_

namespace functional {

template <typename T>
struct pointer_less {
	bool operator()(const T *a, const T *b) const {
		return *a < *b;
	}
};

} // functional

#endif

