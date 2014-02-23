#ifndef _ALGORITHM_SETINTERSECTOR_H_
#define _ALGORITHM_SETINTERSECTOR_H_

#include <algorithm>
#include <iterator>

namespace algorithm {

template <typename SetType>
class SetIntersector {

public:

	typedef typename SetType::value_type value_type;

	SetType set;

	SetIntersector();

	void add(const SetType &new_set);

private:

	bool first;

};

template <typename SetType>
SetIntersector<SetType>::SetIntersector()
	: first(true)
{
}

template <typename SetType>
void SetIntersector<SetType>::add(const SetType &new_set) {
	if(first) {
		set = new_set;
		first = false;
	}
	else {
		SetType temp_set;
		std::set_intersection(
			set.begin(), set.end(),
			new_set.begin(), new_set.end(),
			std::inserter(temp_set, temp_set.begin()));
		set.swap(temp_set);
	}
}

} // namespace algorithm

#endif

