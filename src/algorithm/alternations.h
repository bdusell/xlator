#ifndef _ALGORITHM_ALTERNATIONS_H_
#define _ALGORITHM_ALTERNATIONS_H_

namespace algorithm {

template <typename InIter, typename OutContainer>
void generate_alternations(InIter ibegin, InIter iend, OutContainer &output);

template <typename InIter, typename OutContainer>
void generate_alternations(InIter ibegin, InIter iend, OutContainer &output) {
	if(ibegin == iend) {
		output.push_back(typename OutContainer::value_type());
	}
	else {
		typename InIter::value_type::const_iterator
			i = ibegin->begin(), n = ibegin->end();
		OutContainer sub_alts;

		::algorithm::generate_alternations(
			++ibegin, iend, sub_alts);

		for(; i != n; ++i) {
			for(typename OutContainer::const_iterator
				j = sub_alts.begin(), m = sub_alts.end(); j != m; ++j)
			{
				output.push_back(typename OutContainer::value_type());
				output.back().push_back(*i);
				output.back().insert(output.back().end(), j->begin(), j->end());
			}
		}
	}
}

} // namespace algorithm

#endif

