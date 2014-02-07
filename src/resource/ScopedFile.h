#ifndef _RESOURCE_SCOPEDFILE_H_
#define _RESOURCE_SCOPEDFILE_H_

namespace resource {

/*
A RAII-style file stream.

Usage:
	{
		ScopeFile<std::ofstream> fout(file_name);
		// use fout ...
	} // fout is closed when it goes out of scope

*/
template <typename T>
class ScopedFile : public T {

public:

	template <typename U>
	inline ScopedFile(U arg) : T(arg) {}

	inline ~ScopedFile() { this->close(); }

};

} // namespace resource

#endif

