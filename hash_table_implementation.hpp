#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

template <typename KEY_TYPE, typename VALUE_TYPE>
hashTable<KEY_TYPE, VALUE_TYPE> :: hashTable()
{
	int NumBucketsAvailableInitially = NUM_BUCKETS_AVAILABLE_INITIALLY;
	userPrintKeyFunc *UserPrintKeyFunc = 0;
	userPrintValueFunc *UserPrintValueFunc = 0;
	init(NumBucketsAvailableInitially, UserPrintKeyFunc, UserPrintValueFunc);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
hashTable<KEY_TYPE, VALUE_TYPE> :: hashTable(int NumBucketsAvailableInitially)
{
	userPrintKeyFunc *UserPrintKeyFunc = 0;
	userPrintValueFunc *UserPrintValueFunc = 0;
	init(NumBucketsAvailableInitially, UserPrintKeyFunc, UserPrintValueFunc);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
hashTable<KEY_TYPE, VALUE_TYPE> :: hashTable(userPrintKeyFunc *UserPrintKeyFunc, userPrintValueFunc *UserPrintValueFunc)
{
	int NumBucketsAvailableInitially = NUM_BUCKETS_AVAILABLE_INITIALLY;
	init(NumBucketsAvailableInitially, UserPrintKeyFunc, UserPrintValueFunc);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
hashTable<KEY_TYPE, VALUE_TYPE> :: hashTable(int NumBucketsAvailableInitially,
	userPrintKeyFunc *UserPrintKeyFunc, userPrintValueFunc *UserPrintValueFunc)
{
	init(NumBucketsAvailableInitially, UserPrintKeyFunc, UserPrintValueFunc);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void hashTable<KEY_TYPE, VALUE_TYPE> :: init(int NumBucketsAvailableInitially,
	userPrintKeyFunc *AUserPrintKeyFunc, userPrintValueFunc *AUserPrintValueFunc)
{
	NumTotalBuckets = NumBucketsAvailableInitially;
	Buckets = (bucket<KEY_TYPE, VALUE_TYPE> *) malloc(NumTotalBuckets * sizeof(bucket<KEY_TYPE, VALUE_TYPE>));
	for (int i = 0; i < NumTotalBuckets; ++i)
	{
		Buckets[i].IsSet = false;
	}
	NumOccupiedBuckets = 0;

	UserPrintKeyFunc = AUserPrintKeyFunc;
	UserPrintValueFunc = AUserPrintValueFunc;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void hashTable<KEY_TYPE, VALUE_TYPE> :: print()
{
//	using namespace HashTableFuncs;

	for (int i = 0; i < NumTotalBuckets; ++i)
	{
		if (Buckets[i].IsSet)
		{
			KEY_TYPE Key = Buckets[i].Key;
			VALUE_TYPE Value = Buckets[i].Value;
			unsigned int Hash = HashTableFuncs::hash_digest(Key) % NumTotalBuckets;
			printf("%d: ", i);
//			HashTableFuncs::print(Key);
			if(UserPrintKeyFunc) UserPrintKeyFunc(&Key); else printf("?");
			printf(", ");
//			HashTableFuncs::print(Value);
			if(UserPrintValueFunc) UserPrintValueFunc(&Value); else printf("?");
			printf(", [hash: %u]", Hash);
			printf("\n");
		}
		else
		{
			printf("%d: -\n", i);
		}
	}
	printf("%d / %d buckets used.\n\n", NumOccupiedBuckets, NumTotalBuckets);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void hashTable<KEY_TYPE, VALUE_TYPE> :: expand_table()
{
	int OldNumTotalBuckets = NumTotalBuckets;
	bucket<KEY_TYPE, VALUE_TYPE> *OldMemory = Buckets;

//	init(Table, OldSize * 2);
//	this->hashTable(OldSize * 2); //@ should we do that?
	NumTotalBuckets *= 2;
	Buckets = (bucket<KEY_TYPE, VALUE_TYPE> *) malloc(NumTotalBuckets * sizeof(bucket<KEY_TYPE, VALUE_TYPE>));
	for (int i = 0; i < NumTotalBuckets; ++i)
	{
		Buckets[i].IsSet = false;
	}
	NumOccupiedBuckets = 0;

	for (int i = 0; i < OldNumTotalBuckets; ++i)
	{
		if (OldMemory[i].IsSet)
		{
			put(OldMemory[i].Key, OldMemory[i].Value);
		}
	}
	free(OldMemory);
}

//@ We dont occupy a new bucket if the key is already in the table (we overwrite the value) but the table might expand nonetheless.
template <typename KEY_TYPE, typename VALUE_TYPE>
void hashTable<KEY_TYPE, VALUE_TYPE> :: put(KEY_TYPE Key, VALUE_TYPE Value)
{
	using namespace HashTableFuncs;

	//	assert(NumOccupiedBuckets < NumTotalBuckets/2);
	if (NumOccupiedBuckets == NumTotalBuckets/2)
	{
		expand_table();
	}

	unsigned long HashDigest = hash_digest(Key);
	unsigned long Hash = HashDigest % NumTotalBuckets;
//	printf("Digest: %lu, Hash: %lu, NumTotalBuckets: %d\n", HashDigest, Hash, NumTotalBuckets);

	while (Buckets[Hash].IsSet)
	{
		if (are_equal(Buckets[Hash].Key, Key))
		{
			// Key already there. Overwrite the previous value.
			Buckets[Hash].Value = Value;
			return;
		}
		Hash += 1;
		Hash %= NumTotalBuckets;
	}
	Buckets[Hash].IsSet = true;
	Buckets[Hash].Key = Key;
	Buckets[Hash].Value = Value;
	NumOccupiedBuckets += 1;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
bool hashTable<KEY_TYPE, VALUE_TYPE> :: get(KEY_TYPE Key, VALUE_TYPE *OutValue)
{
	using namespace HashTableFuncs;

	unsigned int Hash = hash_digest(Key) % NumTotalBuckets;

//	int num_comparsions = 0;
	bool Result = false;
	while (Buckets[Hash].IsSet)
	{
//		num_comparsions += 1;
		if (are_equal(Buckets[Hash].Key, Key))
		{
			Result = true;
			*OutValue = Buckets[Hash].Value;
			break;
		}
		Hash += 1;
		Hash %= NumTotalBuckets;
	}
//	printf("num_comparsions for \"%s\": %d\n", str, num_comparsions);
	return Result;
}

//template <typename KEY_TYPE, typename VALUE_TYPE>
//void hashTable<KEY_TYPE, VALUE_TYPE> :: remove(KEY_TYPE Key)
//{
//	using namespace HashTableFuncs;
//
//	unsigned int Hash = hash_digest(Key) % NumTotalBuckets;
//
////	int num_comparsions = 0;
//	while (Buckets[Hash].IsSet)
//	{
////		num_comparsions += 1;
//		if (are_equal(Buckets[Hash].Key, Key))
//		{
////			Buckets[Hash].IsSet = false;
//			int Src, Dest;
//			Src = Dest = Hash;
//			Src += 1;
////			Src %= NumTotalBuckets;
//			while(Buckets[Src % NumTotalBuckets].IsSet)
//			{
//				unsigned int PreferredIndex = hash_digest(Buckets[Src % NumTotalBuckets].Key) % NumTotalBuckets;
//
//				//@this is wrong nonono
//				if(Src > NumTotalBuckets-1 && (Src % NumTotalBuckets) >= PreferredIndex) PreferredIndex += NumTotalBuckets;
////				if(Src > NumTotalBuckets-1) PreferredIndex += NumTotalBuckets;
//
//				printf("%s, Preferred: %u, Dest: %d, Src: %d\n",
//					Buckets[Src % NumTotalBuckets].Value,
//					PreferredIndex,
//					Dest, Src);
//
//				if(PreferredIndex <= Dest)
////				if(PreferredIndex <= Dest || PreferredIndex > Src)
//				{
//					Buckets[Dest % NumTotalBuckets] = Buckets[Src % NumTotalBuckets];
//					Dest = Src;
//					Src += 1;
//					continue;
//				}
//				else
//				{
//					Src += 1;
//					continue;
//				}
//
////				Buckets[Dest % NumTotalBuckets] = Buckets[Src % NumTotalBuckets];
////				Src += 1;
//////				Src %= NumTotalBuckets;
////				Dest += 1;
//////				Dest %= NumTotalBuckets;
//			}
//			Buckets[Dest % NumTotalBuckets].IsSet = false;
//			NumOccupiedBuckets -= 1;
//			break;
//		}
//		Hash += 1;
//		Hash %= NumTotalBuckets;
//	}
//}

//@ definately clean up this code and test heavily (especially these outlier cases with large clumps of buckets "wrapping around the table")
template <typename KEY_TYPE, typename VALUE_TYPE>
void hashTable<KEY_TYPE, VALUE_TYPE> :: remove(KEY_TYPE Key)
{
	using namespace HashTableFuncs;

	unsigned int Hash = hash_digest(Key) % NumTotalBuckets;

	while (Buckets[Hash].IsSet)
	{
		if (are_equal(Buckets[Hash].Key, Key))
		{
			break;
		}

		Hash += 1;
		Hash %= NumTotalBuckets;
	}

	if(Buckets[Hash].IsSet)
	{
		// We have a bucket to delete

		// Find index of the first bucket in the clump of buckets
		int Index = Hash; while(Buckets[((NumTotalBuckets + Index) % NumTotalBuckets)].IsSet) Index -= 1; //@ Hash is unsigned long, Index is int
		int IndexFirst = ((NumTotalBuckets + Index) % NumTotalBuckets) + 1;
//		printf("IndexFirst: %d\n", IndexFirst);

		int Dest, Src;
		// Have Dest and Src be offsets from the IndexFirst
		// Buckets[(Src + IndexFirst) % NumTotalBuckets]
		Dest = (Hash >= IndexFirst) ? Hash - IndexFirst : NumTotalBuckets - IndexFirst + Hash;
		Src = Dest + 1;
//		printf("Dest: %d, Src: %d\n", Dest, Src);

		while(Buckets[(Src + IndexFirst) % NumTotalBuckets].IsSet)
		{
			// check if bucket should be copied over
			unsigned int PreferredIndex = hash_digest(Buckets[(Src + IndexFirst) % NumTotalBuckets].Key) % NumTotalBuckets;
//			assert(PreferredIndex >= IndexFirst || ((PreferredIndex < IndexFirst) && (PreferredIndex <= (Src + IndexFirst) % NumTotalBuckets)));
			int PreferredOffset =
				(PreferredIndex >= IndexFirst) ? PreferredIndex - IndexFirst : NumTotalBuckets - IndexFirst + PreferredIndex;
//			printf("%s, Src: %d, Dest: %d, PrefferedOffset: %d\n",
//				Buckets[(Src + IndexFirst) % NumTotalBuckets].Value, Src, Dest, PreferredOffset);
			// if PreferredOffset is between IndexFirst and Dest (including these) we should copy
			if(PreferredOffset <= Dest)
			{
				Buckets[(Dest + IndexFirst) % NumTotalBuckets] = Buckets[(Src + IndexFirst) % NumTotalBuckets];
				Dest = Src;
				Src += 1;
			}
			else
			{
				Src += 1;
			}
//			Src += 1;
		}
		Buckets[(Dest + IndexFirst) % NumTotalBuckets].IsSet = false;
		NumOccupiedBuckets -= 1;
	}
	else
	{
		// Couldnt find the bucket
	}
}


