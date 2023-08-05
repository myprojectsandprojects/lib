template <typename t>
void ArrayInit(array<t> *Array)
{
	Array->Count = 0;
	Array->Allocated = INITIALLY_ALLOCATED;
	Array->Data = (t *) malloc(Array->Allocated * sizeof(t)); //@
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


