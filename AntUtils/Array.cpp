#include "stdafx.h"
#include "Array.h"

template<class T> Array<T>::Array()
	:_array_count(0),
	_array_size(1),
	_grow_step(1)
{
	this->_array = (T*)malloc(sizeof(T)*this->_array_size);
}


template<class T> Array<T>::~Array()
{
	free(this->_array);
}

template<class T> void Array<T>::Push(T item){
	if (this->_array_count == this->_array_size){
		this->_array_size += this->_grow_step;
		realloc(this->_array, sizeof(T)*this->_array_size);
	}
	this->_array[this->_array_count++] = item;
}

template<class T> void Array<T>::ForEach(std::function<void(T)> func){
	for (int i = 0; i < this->_array_count; i++){
		func(this->_array[i]);
	}
}