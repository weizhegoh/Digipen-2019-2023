#include "index_sequence.h"

#ifdef CPP11
	#include "cpp11.h"
#else
	#include "cpp17.h"
#endif

int main()
{
	make_sequence<N>::print();
}
