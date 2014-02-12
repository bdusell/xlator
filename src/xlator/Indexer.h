#ifndef _XLATOR_INDEXER_H_
#define _XLATOR_INDEXER_H_

#include <map>

namespace xlator {

template <typename T, typename Int = unsigned int>
class Indexer {

public:

	typedef T key_type;
	typedef Int index_type;

private:

	typedef std::map<key_type, index_type> map_type;

public:

	index_type index_key(const key_type &k, index_type index);

	bool get_index(const key_type &k, index_type &out) const;

	index_type size() const;

	typedef typename map_type::const_iterator const_iterator;
	const_iterator begin() const;
	const_iterator end() const;

	void clear();

private:

	map_type map;

};

template <typename T, typename Int>
typename Indexer<T, Int>::index_type Indexer<T, Int>::index_key(const key_type &k, index_type index) {
	return map.insert(typename map_type::value_type(k, index)).first->second;
}

template <typename T, typename Int>
bool Indexer<T, Int>::get_index(const key_type &k, index_type &out) const {
	typename map_type::const_iterator pos = map.find(k);
	if(pos == map.end()) {
		return false;
	}
	else {
		out = pos->second;
		return true;
	}
}

template <typename T, typename Int>
typename Indexer<T, Int>::index_type Indexer<T, Int>::size() const {
	return map.size();
}

template <typename T, typename Int>
typename Indexer<T, Int>::const_iterator Indexer<T, Int>::begin() const {
	return map.begin();
}

template <typename T, typename Int>
typename Indexer<T, Int>::const_iterator Indexer<T, Int>::end() const {
	return map.end();
}

template <typename T, typename Int>
void Indexer<T, Int>::clear() {
	map.clear();
}

} // namespace xlator

#endif

