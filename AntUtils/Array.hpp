#pragma once
#include <malloc.h>
#include <functional>
#include <stack>

#include "Tuple.hpp"

template<class T> class Array
{
public:
	Array(int size) : _array_count(0),
		_array_size(size),
		_grow_step(1){
		this->_array = (T*)malloc(sizeof(T)*this->_array_size);
	}

	Array() : _array_count(0),
		_array_size(1),
		_grow_step(1){
		this->_array = (T*)malloc(sizeof(T)*this->_array_size);
	}

	~Array(){
		//free(this->_array);
	}

	void Push(T item){
		if (this->_array_count == this->_array_size){
			this->_array_size += this->_grow_step;
			realloc(this->_array, sizeof(T)*this->_array_size);
		}
		this->_array[this->_array_count++] = item;
	}

	Array<T> ForEach(std::function<void(T)> func){
		for (int i = 0; i < this->_array_count; i++){
			func(this->_array[i]);
		}
		return *this;
	}

	Array<T> ForEach(std::function<void(int, T)> func, int start_position){
		for(int i = start_position; i < this->_array_count; i++){
			func(i, this->_array[i]);
		}
		return *this;
	}

	template<class D> Array<D> Map(std::function<D(T)> func){
		Array<D> mapArray(this->_array_count);
		this->ForEach([&](T i){
			mapArray.Push(func(i));
		});
		return mapArray;
	}

	Tuple_2<Array<T>, bool> Exists(std::function<bool(T)> func){
		Tuple_2<bool, Array<T>> result(false, *this);
		this->ForEach([&result, &func](T i){
			if (result.Item1) return;
			result.Item1 = func(i);
		});
		return result;
	}

	Array<T> Find(std::function<bool(T)> func){
		Array<T> result;
		this->ForEach([&result, &func](T i){
			if (func(i)) result.Push(i);
		});
		return result;
	}

	Array<T> Sort(std::function<bool(const T&, const T&)> func){
		stack<Tuple_2<int,int>> pSt;
		pSt.push(*(new Tuple_2<int,int>(0, this->_array_count - 1)));
		do{
			Tuple_2<int,int> state = pSt.top(); pSt.pop();
			int p = state.Item1;
			int pBg = state.Item1;
			int pEd = state.Item2;
			while (pBg < pEd){
				while (p < pEd && func(this->_array[p], this->_array[pEd])) pEd--;
				if (p < pEd){
					T mid = this->_array[p];
					this->_array[p] = this->_array[pEd];
					this->_array[pEd] = mid;
					p = pEd;
				}
				while (pBg < p && func(this->_array[pBg], this->_array[p])) pBg++;
				if (pBg < p){
					T mid = this->_array[p];
					this->_array[p] = this->_array[pBg];
					this->_array[pBg] = mid;
					p = pBg;
				}
			}
			if (state.Item1 < p - 1){
				Tuple_2<int, int> fr(state.Item1, p - 1);
				pSt.push(fr);
			}
			if (p + 1 < state.Item2){
				Tuple_2<int, int> nx(p + 1, state.Item2);
				pSt.push(nx);
			}
		} while (!pSt.empty());
		return *this;
	}

	Array<T> Remove(int index){
		this->ForEach([this](int p, T item){
			_array[p - 1] = _array[p];
		}, index + 1);
		_array_count--;
		return *this;
	}

	int Count() const { return this->_array_count; }
private:
	T* _array;
	int _array_size;
	int _array_count;
	int _grow_step;
};