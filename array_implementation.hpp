template <typename t>
void ArrayInit(array<t> *A)
{
	A->Count = 0;
	A->Allocated = INITIALLY_ALLOCATED;
	A->Data = (t *) malloc(A->Allocated * sizeof(t)); //@
}

template <typename t>
void ArrayFree(array<t> *A)
{
	free(A->Data);
}

template <typename t>
array<t> *ArrayNew()
{
	array<t> *Array = (array<t> *) malloc(sizeof(array<t>));
	ArrayInit(Array);
	return Array;
}

template <typename t>
void ArrayAdd(array<t> *Array, t Element)
{
//	assert(array->count <= array->allocated);
	if (Array->Count == Array->Allocated)
	{
		Array->Allocated *= 2;
		t *P = (t *) malloc(Array->Allocated * sizeof(t)); //@
		for(int I = 0; I < Array->Count; ++I)
		{
			P[I] = Array->Data[I];
		}
		free(Array->Data);
		Array->Data = P;
	}

	Array->Data[Array->Count] = Element;
	Array->Count += 1;
}

//@ wouldnt this be less copying? especially when storing a struct with bunch of data members?
// based on my very quick and sloppy test, this is was maybe slightly faster than above version, but with -O1, the difference, if it is there, is very small. maybe its because the compiler manages to optimize away the shortcomings of the above version, who knows... But it was almost twice as fast to make a struct locally and copy/store the whole struct rather than to malloc() and store a pointer. I only measured how fast it was to store 1 million structs in an array, not how fast it was to access them.
template <typename t>
t *ArrayAdd(array<t> *Array)
{
//	assert(array->count <= array->allocated);
	if (Array->Count == Array->Allocated)
	{
		Array->Allocated *= 2;
		t *P = (t *) malloc(Array->Allocated * sizeof(t)); //@
		for(int I = 0; I < Array->Count; ++I)
		{
			P[I] = Array->Data[I];
		}
		free(Array->Data);
		Array->Data = P;
	}

	// Array->Count += 1;
	return &Array->Data[Array->Count++];
}

template <typename t>
void ArrayAdd(array<t> *Array, t Elements[], int NumElements)
{
	int NewNumElements = Array->Count + NumElements;
	if (NewNumElements > Array->Allocated)
	{
		int AllocatedTwice = Array->Allocated * 2;
		Array->Allocated = (NewNumElements > AllocatedTwice) ? NewNumElements : AllocatedTwice;

		t *Ptr = (t *) malloc(Array->Allocated * sizeof(t)); //@

		for(int i = 0; i < Array->Count; ++i)
		{
			Ptr[i] = Array->Data[i];
		}

		free(Array->Data);
		Array->Data = Ptr;
	}

	int j = Array->Count;
	for(int i = 0; i < NumElements; ++i)
	{
		Array->Data[j] = Elements[i];
		++j;
	}
	Array->Count = NewNumElements;
}

// //@ wouldnt this be less copying? especially when storing a struct with bunch of data members?
// template <typename t>
// t *ArrayAdd(array<t> *Array, int NumElements)
// {
// 	//
// }

template <typename t>
void ArrayAddSlow(array<t> *a, t e[], int n)
{
	for(int i = 0; i < n; ++i)
	{
		ArrayAdd(a, e[i]);
	}
}

template <typename t>
void ArrayRemove(array<t> *Array, int i)
{
	assert(i >= 0 && i < Array->Count);
	for(; i < Array->Count-1; ++i)
	{
		Array->Data[i] = Array->Data[i+1];
	}
	Array->Count -= 1;
}


