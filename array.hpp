#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>

#define INITIALLY_ALLOCATED 100

template <typename t>
struct array
{
	int Count;
	int Allocated;
	t *Data;
};

#include "array_implementation.hpp"

#endif


