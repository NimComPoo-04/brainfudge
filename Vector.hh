#ifndef _VECTOR_HH_
#define _VECTOR_HH_

#include <cassert>

template<typename T>
class Vector
{
	private:
		T *data;
		size_t length;
		size_t size;

	public:
		Vector(size_t size = 64)
		{
			Vector::size = size < 64 ? 64 : size;
			Vector::length = 0;
			Vector::data = new T[Vector::size];
		}

		~Vector()
		{
			delete[] data;
		}

		T &operator[](size_t index)
		{
			assert(index < length && "Out of bounds execption");
			return data[index];
		}

		T *get_data()
		{
			return data;
		}

		size_t len() const
		{
			return length;
		}

		size_t cap() const
		{
			return size;
		}

		void push(T c)
		{
			if(length >= size)
			{
				size = size * 2;
				T *d = new T[size];
				for(size_t i = 0; i < length; i++)
				{
					d[i] = data[i];
				}
				delete[] data;
				data = d;
			}

			data[length++] = c;
		}

		T pop()
		{
			assert(length > 0 && "Stack Underflow!");
			return data[--length];
		}
};

#endif
