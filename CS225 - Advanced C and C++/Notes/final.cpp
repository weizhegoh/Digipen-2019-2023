#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>

template <typename T>
class my_basic_string
{
    size_t _length;
    T*     _text;
public:
	my_basic_string();
	my_basic_string(const my_basic_string& rhs);
	my_basic_string(my_basic_string&& rhs);
	my_basic_string(const char* rhs);
    
	my_basic_string<T>& operator=(
		my_basic_string&& rhs
	);
	
	my_basic_string<T>& operator=(
		const my_basic_string& rhs
	);
	
	operator const T*() const;
	
	const T* c_str() const;
	
	~my_basic_string();
};

using my_string  = my_basic_string<char>;
using my_wstring = my_basic_string<wchar_t>;

template <typename T>
my_basic_string<T>::my_basic_string() :
	_length{0},
	_text{new T[1]}
{
	_text[0] = '\0';
}

template <typename T>
my_basic_string<T>::my_basic_string(const char* rhs) :
	_length{ strlen(rhs) },
	_text{ new T[_length + 1] }
{
	std::copy(rhs, rhs + _length + 1, _text);
}

template <typename T>
my_basic_string<T>::my_basic_string(
	const my_basic_string& rhs
) :
	_length{rhs._length},
	_text{new T[rhs._length + 1]}
{
	std::copy(
		rhs._text, rhs._text + rhs._length + 1,
		_text
	);
}

template <typename T>
my_basic_string<T>::my_basic_string(
	my_basic_string&& rhs
) :
	_length{rhs._length},
	_text{rhs._text}
{
	rhs._length = 0;
	rhs._text = new T[1];
	rhs._text[0] = '\0';
}

template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(
	my_basic_string&& rhs
)
{
	if (this != &rhs)
	{
		std::swap(_length, rhs._length);
		std::swap(_text, rhs._text);
	}
	return *this;
}

template <typename T>
my_basic_string<T>& my_basic_string<T>::operator=(
	const my_basic_string& rhs
)
{
	if (this != &rhs)
	{
		T* temp = new T[_length + 1];
		std::copy(rhs._text, rhs._text + rhs._length + 1, temp);
		delete[] _text;
		_text = temp;
		_length = rhs._length;
	}
	return *this;
}

template <typename T>
my_basic_string<T>::~my_basic_string()
{
	delete[] _text;
}

template <typename T>
const T* my_basic_string<T>::c_str() const
{
	return _text;
}

#if 0

template <typename T>
my_basic_string<T>::operator const T*() const
{
	return _text;
}

#else

template <typename T>
std::ostream& operator<<(std::ostream& stream, const my_basic_string<T> rhs)
{
	return (stream << rhs.c_str());
}

#endif

int main()
{
	my_string s1{};
	my_string s2{"ABC"};
	my_string s3{s2};
	my_string s4{std::move(s3)};
	
	std::cout << s1 << "|" << s2 << "|" << s3 << "|" << s4 << "." << std::endl;
}