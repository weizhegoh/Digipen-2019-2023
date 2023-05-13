#pragma once
// A simplified skeleton of
// https://en.cppreference.com/w/cpp/container/vector
template <typename T>
class vector
{
	size_t	_count;
	T* _data;
public:
	//vector() :
	//	_count{ 0 },
	//	_data{ new T[_count] }
	//{
	//	std::cout << "vector::vector()" << std::endl;
	//}

	//~vector()
	//{
	//	delete[] _data;
	//	std::cout << "vector::~vector()" << std::endl;
	//}

	//// Copy semantics
	//vector(const vector& rhs) :
	//	_count{ rhs._count },
	//	_data{ new T[_count] }
	//{
	//	std::copy(rhs._data, rhs._data + rhs._count, _data);
	//	std::cout << "vector::vector(copy)" << std::endl;
	//}

	//vector& operator=(const vector& rhs)
	//{
	//	std::cout << "vector::op=(copy)" << std::endl;
	//	if (this != &rhs)
	//	{
	//		T* temp = new T[rhs._count];
	//		std::copy(rhs._data, rhs._data + rhs._count, temp);
	//		delete[] _data;
	//		_data = temp;
	//		_count = rhs._count;
	//	}
	//	return *this;
	//}

	//// Move semantics
	//vector(vector&& rhs) :
	//	_count{ rhs._count },
	//	_data{ rhs._data }
	//{
	//	rhs._count = 0;
	//	rhs._data = new T[rhs._count];
	//	std::cout << "vector::vector(move)" << std::endl;
	//}

	//vector& operator=(vector&& rhs)
	//{
	//	std::cout << "vector::op=(move)" << std::endl;
	//	swap(_count, rhs._count);
	//	swap(_data, rhs._data);
	//	return *this;
	//}

	vector();

	vector(const vector& rhs);
	vector(vector&& rhs);

	vector& operator=(const vector& rhs);
	vector& operator=(vector&& rhs);


	~vector();
}; 