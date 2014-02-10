#ifndef _FUNCTIONAL_POINTER_CMP_H_
#define _FUNCTIONAL_POINTER_CMP_H_

namespace functional {

#define _DEFINE_POINTER_CMP(name, op) \
template <typename T> \
struct pointer_ ## name { \
	bool operator()(T a, T b) const { \
		return *a op *b; \
	} \
};

/* pointer_less */
_DEFINE_POINTER_CMP(less, <)

/* pointer_equal */
_DEFINE_POINTER_CMP(equal, ==)

#undef _DEFINE_POINTER_CMP

} // functional

#endif

