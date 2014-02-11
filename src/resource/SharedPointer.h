#ifndef _RESOURCE_SHAREDPOINTER_H_
#define _RESOURCE_SHAREDPOINTER_H_

#include <algorithm>

namespace resource {

template <typename T>
class SharedPointer {

public:

	typedef unsigned int counter_type;

	inline SharedPointer();

	inline SharedPointer(T *ptr);

	inline SharedPointer(const SharedPointer &copy);

	~SharedPointer();

	inline SharedPointer<T> &operator=(SharedPointer<T> that);

	void swap(SharedPointer<T> &that);

	inline counter_type references() const;

	inline T &operator*();
	inline const T &operator*() const;

	inline T *operator->();
	inline const T *operator->() const;

	inline operator bool() const;

private:

	T *_data;
	counter_type *_references;

};

template <typename T>
SharedPointer<T>::SharedPointer()
	: _data(NULL)
	, _references(NULL)
{
}

template <typename T>
SharedPointer<T>::SharedPointer(T *ptr)
	: _data(ptr)
	, _references(new counter_type(1))
{
}

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer &copy)
	: _data(copy._data)
	, _references(copy._references)
{
	if(_references) ++*_references;
}

template <typename T>
SharedPointer<T>::~SharedPointer() {
	if(_references && --*_references == 0) {
		delete _data;
		delete _references;
	}
}

template <typename T>
SharedPointer<T> &SharedPointer<T>::operator=(SharedPointer<T> that) {
	swap(that);
	return *this;
}

template <typename T>
void SharedPointer<T>::swap(SharedPointer<T> &that) {
	std::swap(_data, that._data);
	std::swap(_references, that._references);
}

template <typename T>
typename SharedPointer<T>::counter_type SharedPointer<T>::references() const {
	return *_references;
}

template <typename T>
T &SharedPointer<T>::operator*() {
	return *_data;
}

template <typename T>
const T &SharedPointer<T>::operator*() const {
	return *_data;
}

template <typename T>
T *SharedPointer<T>::operator->() {
	return _data;
}

template <typename T>
const T *SharedPointer<T>::operator->() const {
	return _data;
}

template <typename T>
SharedPointer<T>::operator bool() const {
	return _data;
}

} // namespace resource

#endif

