#include "hash_table.hpp"

#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>

int main()
{
	hashTable<int, int> Squares;
	Squares.put(3, 3*3);
	Squares.put(4, 4*4);
	Squares.put(5, 5*5);

	int Value;
	int Key = 3;
	bool HasIt = Squares.get(Key, &Value);
	if(HasIt)
	{
		printf("%d: %d\n", Key, Value);
	}
	else
	{
		printf("%d: -\n", Key);
	}

	Key = 11; if(Squares.get(Key, &Value)) { printf("%d: %d\n", Key, Value); } else { printf("%d: -\n", Key); }
	Key = -1; if(Squares.get(Key, &Value)) { printf("%d: %d\n", Key, Value); } else { printf("%d: -\n", Key); }
	Squares.put(-1, -1*-1);
	Key = -1; if(Squares.get(Key, &Value)) { printf("%d: %d\n", Key, Value); } else { printf("%d: -\n", Key); }
	Squares.remove(-1);
	Key = -1; if(Squares.get(Key, &Value)) { printf("%d: %d\n", Key, Value); } else { printf("%d: -\n", Key); }
}

//void HashTableFuncs::print(long a) { printf("%ld", a); }
//void HashTableFuncs::print(int a) { printf("%d", a); }
//void HashTableFuncs::print(unsigned long a) { printf("%lu", a); }
//void HashTableFuncs::print(unsigned int a) { printf("%u", a); }
//void HashTableFuncs::print(double a) { printf("%.9f", a); }
//void HashTableFuncs::print(void *a) { printf("%p", a); }
//void HashTableFuncs::print(const char *a) { printf("%s", a); }

unsigned long HashTableFuncs::hash_digest(unsigned long Key) { return Key; }
unsigned long HashTableFuncs::hash_digest(long Key) { return Key; }
unsigned long HashTableFuncs::hash_digest(unsigned int Key) { return Key; }
unsigned long HashTableFuncs::hash_digest(int Key) { return Key; }

unsigned long HashTableFuncs::hash_digest(double Key)
{
	// We do the second simplest/dumbest thing possible (after simply casting floats to integers) which is that we interpret the bits of a floating point number as an integer.
	// There was a lot of talk on stackoverflow on how floating point numbers shouldnt be used as hashtable keys. Off the top of my head I cant really come up with a single example use-case when one might want to use floating point numbers as hash-table keys. But well see...
	printf("hash_digest\n");
	return *((unsigned long *)&Key);
//	unsigned long HashDigest =  *((unsigned long *)&Key);
//	printf("%f -> %lu\n", Key, HashDigest);
//	return HashDigest;
}

// casting a float to double produces hash-digests which divide well by powers of 2 somehow (insignificant bits are very zeroish) and hashes for these digests all start colliding when the table size is a power of 2. Something like that. So we need a separate function for float type.
unsigned long HashTableFuncs::hash_digest(float Key)
{
	// We do the second simplest/dumbest thing possible (after simply casting floats to integers) which is that we interpret the bits of a floating point number as an integer.
	// There was a lot of talk on stackoverflow on how floating point numbers shouldnt be used as hashtable keys. Off the top of my head I cant really come up with a single example use-case when one might want to use floating point numbers as hash-table keys. But well see...
	printf("hash_digest\n");
	return *((unsigned long *)&Key);
//	unsigned int HashDigest =  *((unsigned int *)&Key);
//	printf("%f -> %u\n", Key, HashDigest);
//	return HashDigest;
}

unsigned long HashTableFuncs::hash_digest(const char *Key)
{
	printf("hash_digest\n");
	unsigned int HashDigest = 0;
	for (const char *Ptr = Key; *Ptr; ++Ptr)
	{
		HashDigest += *Ptr;
	}
	return HashDigest;
}

unsigned long HashTableFuncs::hash_digest(void *Key) { return (unsigned long)Key; }

bool HashTableFuncs::are_equal(unsigned long Key1, unsigned long Key2)
{
	return (Key1 == Key2);
}

bool HashTableFuncs::are_equal(unsigned int Key1, unsigned int Key2)
{
	return (Key1 == Key2);
}

bool HashTableFuncs::are_equal(long Key1, long Key2)
{
	return (Key1 == Key2);
}

bool HashTableFuncs::are_equal(int Key1, int Key2)
{
	return (Key1 == Key2);
}

bool HashTableFuncs::are_equal(double Key1, double Key2)
{
	return (Key1 == Key2);
}

bool HashTableFuncs::are_equal(const char *Key1, const char *Key2)
{
	return (strcmp(Key1, Key2) == 0);
}

bool HashTableFuncs::are_equal(void *Key1, void *Key2)
{
	return Key1 == Key2;
}


