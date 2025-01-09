#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define INITIALLY_ALLOCATED 1
//#define INITIALLY_ALLOCATED 1024

template <typename t>
struct array
{
	int Count;
	int Allocated; // are we counting in bytes or elements?
	t *Data;
};

//struct array {
//	int NumElements;
//	int NumElementsAllocated;
////	int NumElemAlloc;
//	t *Elements;
//};

#include "array_implementation.hpp"

#endif


