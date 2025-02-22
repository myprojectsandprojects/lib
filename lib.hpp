#ifndef LIB_H
#define LIB_H

// Nothing in here should be specific to any platform
// But we do depend on C standard library (which should be cross platform?)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //@ is this linux specific?
#include <sys/time.h> //@?
#include <assert.h>

#include <stdint.h>
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

//@ are these 32 and 64 bits for all architectures?
typedef float r32;
typedef double r64;

// Determines the length of an array at compile time.
#define COUNT(a) (sizeof(a) / sizeof(a[0]))

#define LIB_MIN(a, b) (a) < (b) ? (a) : (b)
#define LIB_MAX(a, b) (a) > (b) ? (a) : (b)

// Using this on a float or a double before casting it to an integer yields a more accurate result.
#define CAST_ROUND(n) ((n) + (((n) > 0) ? 0.5 : -0.5))

namespace Lib
{
	uint64_t SafeSignedToUnsigned(int64_t Signed);
	int64_t SafeUnsignedToSigned(uint64_t Unsigned);

	bool ReadFile(const char *FileName, u8 **Contents, size_t *NumBytes);
	bool WriteFile(const char *FileName, u8 *Contents, size_t NumBytes);
	bool ReadTextFile(const char *FileName, char **Contents); // returns 0-terminated contents
	//bool WriteTextFile(const char *FileName, char **Contents); // expects 0-terminated contents

	//void integer_as_binary(uint8_t Buffer[65], uint64_t Value);
	uint8_t *i2b(uint64_t Value); // integer as binary
	void unpack_double_value(double Value, bool *Sign, int *Exponent, double *Mantissa);
	//double round(double D);
	//int get_random_int(int Min, int Max);
	double random_num();
	int random_num(int RangeStart, int RangeEnd);
	// bool find_bytes(uint8_t const *needle, int32_t needle_len, uint8_t const *haystack, int32_t haystack_len, int32_t *p_offset);
	int32_t c_str_len(const char *c_str);
	int32_t forward_find(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len);
	int32_t backward_find(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len);
	int32_t forward_find_bm(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len);
	int32_t backward_find_bm(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len);

	const char *basename_get_extension(const char *basename);
	const char *filepath_get_basename(const char *filepath);

	double get_time_secs();
	long get_time_us();

	void ZeroMemory(uint8_t * Memory, int64_t NumBytes);

	//@ todo (things that could be improved probably):
	// dynamically grow at some point (when?)
	// improved hash
	// improved probing
	// right now we just store strings so that someone can check if we have a particular string or not, but we might want to store other things as well (?)
	
	#define HASH_TABLE_SIZE 512
	
	struct HashTable
	{
		const char *data[HASH_TABLE_SIZE];
		int num_occupied_slots;
	};

	void hash_table_init(HashTable *table);
	void hash_table_store(HashTable *table, const char *str);
	bool hash_table_has(HashTable *table, const char *str);
	void hash_table_print(HashTable *table);


//	struct stringSlice
//	{
//		int Length;
//		const char *Data;
//	};
//
//	stringSlice make_stringSlice(const char *Original);
//	void print_stringSlice(stringSlice Slice);
}


#ifdef LIB_INCLUDE_IMPLEMENTATION

namespace Lib
{
	uint64_t SafeSignedToUnsigned(int64_t Signed) {
	    assert(Signed > -1);
	    return Signed;
	}

	int64_t SafeUnsignedToSigned(uint64_t Unsigned) {
		uint64_t FirstUnsignedToBecomeNegative = 0x8000000000000000;
	    assert(Unsigned < FirstUnsignedToBecomeNegative);
	    return Unsigned;
	}

	// bool IsGreater(int64_t Signed, uint64_t Unsigned) {}
	// bool IsLess(int64_t Signed, uint64_t Unsigned) {}
	// bool IsEqual(int64_t Signed, uint64_t Unsigned) {}

	// ? CompareSignedAndUnsigned(int64_t Signed, uint64_t Unsigned) {
	// 	if(Unsigned > 0x7fffffffffffffff) {
	// 		// 'Unsigned' is greater
	// 	} else if(Signed < 0) {
	// 		// 'Unsigned' is greater
	// 	} else {
	// 		if(Signed > Unsigned) {
	// 			// 'Signed' is greater
	// 		} else if(Signed < Unsigned) {
	// 			// 'Unsigned' is greater
	// 		} else {
	// 			// equal
	// 		}
	// 	}
	// }

	bool ReadFile(const char *FileName, u8 **Contents, size_t *ContentsNumBytes)
	{
		FILE *F = fopen(FileName, "rb"); // Docs say that 'b' has no effect on POSIX-systems, but on Windows it disables special handling of '\n'.
		if (!F)
		{
			fprintf(stderr, "Failed to open file: \"%s\"!\n", FileName);
			return false;
		}

		fseek(F, 0, SEEK_END);
		size_t FileSize = ftell(F);
		fseek(F, 0, SEEK_SET); // Back to the beginning...
		void *Memory = malloc(FileSize);
		if (!Memory)
		{
			fprintf(stderr, "Failed to allocate memory for file: \"%s\"!\n", FileName);
			if (fclose(F) == EOF)
			{
				fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			}
			return false;
		}

		// We expect file size to be the same it was when we last queried it (ftell()).
		size_t BytesRead = fread(Memory, sizeof(u8), FileSize, F);
		if (BytesRead != FileSize)
		{
			fprintf(stderr, "Failed to read file: \"%s\"!\n", FileName);
			if (fclose(F) == EOF)
			{
				fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			}
			free(Memory);
			return false;
		}

		if (fclose(F) == EOF)
		{
			fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			free(Memory);
			return false;
		}

		*Contents = (u8 *) Memory;
		*ContentsNumBytes = FileSize;

		return true;
	}

	bool WriteFile(const char *FileName, u8 *Contents, size_t NumBytes)
	{
		FILE *F = fopen(FileName, "w"); //@ wb?
		if (!F)
		{
			fprintf(stderr, "Failed to open file: \"%s\"!\n", FileName);
			return false;
		}

		size_t BytesWritten = fwrite((void *) Contents, sizeof(u8), NumBytes, F);
		if (BytesWritten != NumBytes)
		{
			fprintf(stderr, "Failed to write file: \"%s\"!\n", FileName);
			if (fclose(F) == EOF)
			{
				fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			}
			return false;
		}

		if (fclose(F) == EOF)
		{
			fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			return false;
		}

		printf("Successfully wrote %lu bytes to \"%s\"\n", BytesWritten, FileName);
		return true;
	}

	bool ReadTextFile(const char *FileName, char **Contents)
	{
	//	FILE *F = fopen(FileName, "r");
		FILE *F = fopen(FileName, "rb"); //@ Windows needs 'b'. Why?
		if (!F)
		{
			fprintf(stderr, "Failed to open file: \"%s\"!\n", FileName);
			return false;
		}

		fseek(F, 0, SEEK_END);
		size_t FileSize = ftell(F);
		fseek(F, 0, SEEK_SET); // Back to the beginning...
		void *Memory = malloc(FileSize + 1);
		if (!Memory)
		{
			fprintf(stderr, "Failed to allocate memory for file: \"%s\"!\n", FileName);
			if (fclose(F) == EOF)
			{
				fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			}
			return false;
		}

		// We expect file size to be the same it was when we last queried it (ftell()).
		size_t BytesRead = fread(Memory, sizeof(u8), FileSize, F);
		if (BytesRead != FileSize)
		{
			fprintf(stderr, "Failed to read file: \"%s\"!\n", FileName);
			if (fclose(F) == EOF)
			{
				fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			}
			free(Memory);
			return false;
		}

		if (fclose(F) == EOF)
		{
			fprintf(stderr, "Failed to close file: \"%s\"!\n", FileName);
			free(Memory);
			return false;
		}

		*Contents = (char *) Memory;
		(*Contents)[FileSize] = 0;

		return true;
	}

	//void integer_as_binary(uint8_t Buffer[65], uint64_t Value)
	//{
	//	Buffer[64] = '\0';
	//	for (int i = 63; i >= 0; --i)
	//	{
	//		bool IsLastBitSet = Value % 2;
	//		Buffer[i] = IsLastBitSet ? '1' : '0';
	//		Value >>= 1;
	//	}
	//}

	uint8_t *i2b(uint64_t Value)
	{
		const int NUM_CHARS = 64 + 7 + 1;
		uint8_t *Text = (uint8_t *) malloc(NUM_CHARS);
		uint8_t *Ptr = &Text[NUM_CHARS-1];
		*Ptr-- = '\0';
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 8; ++j)
			{
				bool IsLastBitSet = Value % 2;
				*Ptr-- = IsLastBitSet ? '1' : '0';
				Value >>= 1;
			}
			if (i < 7) *Ptr-- = ' ';
		}
		return Text;
	}

	//@ double values larger than INT_MAX cause an overflow
	//double round(double D)
	//{
	//	int I = (int)D;
	//	double Fraction = D - I;
	//	if(D > 0.0) return (Fraction < 0.5 ? I : I+1);
	//	else return (Fraction > -0.5 ? I : I-1);
	//}

	//@ double values larger than INT_MAX cause an overflow
	//double round(double D)
	//{
	//	if(D >= 0.0)
	//		return (int)(D + 0.5);
	//	else
	//		return (int)(D - 0.5);
	//}

	//// including Min and Max
	////@ range
	//int get_random_int(int Min, int Max)
	//{
	//	assert(Min < Max);
	//
	//	static bool IsSrandCalled = false;
	//	if (!IsSrandCalled)
	//	{
	//		srand(time(NULL));
	//		IsSrandCalled = true;
	//	}
	//
	//	// rand() should return a value between 0 and RAND_MAX
	//	int NumValues = Max - Min + 1;
	//	return ((rand() % NumValues) + Min);
	//}

	double random_num()
	{
		static bool IsSrandCalled = false;
		if (!IsSrandCalled)
		{
			srand(time(NULL));
			IsSrandCalled = true;
		}
		return ((double)rand()) / RAND_MAX; // rand() -> 0 ... RAND_MAX
	}

	// RangeStart ... RangeEnd-1
	int random_num(int RangeStart, int RangeEnd)
	{
		//@ what if random_num() returns exactly 1.0?
		return RangeStart + (int)(random_num() * (RangeEnd - RangeStart));
	}

	//@ Its kind of ridiculous to read them 1 bit at a time. We should probably "bitwise and" with masks
	void unpack_double_value(double Value, bool *Sign, int *Exponent, double *Mantissa)
	{
		*Exponent = 0;
		*Mantissa = 0.0;
		unsigned long int IntValue = *((unsigned long int *) &Value);

		// Sign
		unsigned long int BitMask = ((unsigned long int) 1 << 63);
		*Sign = (IntValue & BitMask) ? true : false;

		// Exponent
		// 11-bit unsigned integer
		// 0 - 2047
		// In biased form: 1023 is zero: -1022 - +1023
		{
			int BitValue = 1;
			for (unsigned int Bit = 52; Bit < 63; ++Bit, BitValue *= 2)
			{
				int IsSet = (IntValue & ((unsigned long int) 1 << Bit)) ? 1 : 0;
				*Exponent += IsSet * BitValue;
			}
			*Exponent -= 1023;
		}

		// Mantissa
		// 52-bits, from most significant to least significant: 2^-1 + 2^-2 + ... + 2^-52 and then add 1
		{
			double BitValue = 1.0 / 2.0;
			for (int Bit = 51; Bit > -1; --Bit, BitValue /= 2.0)
			{
				int IsSet = (IntValue & ((unsigned long int) 1 << (unsigned int) Bit)) ? 1 : 0;
				*Mantissa += IsSet * BitValue;
			}
			*Mantissa += 1;
		}
	}

	// bool find_bytes(uint8_t const *needle, int32_t needle_len, uint8_t const *haystack, int32_t haystack_len, int32_t *p_offset)
	// {
	// 	for(int i = 0; i < haystack_len; ++i)
	// 	{
	// 		if(haystack[i] == needle[0])
	// 		{
	// 			for(int j = 1; j <= needle_len; ++j)
	// 			{
	// 				if(j == needle_len)
	// 				{
	// 					*p_offset = i;
	// 					// *ptr = &haystack[i];
	// 					return true;
	// 				}
	// 				if(needle[j] != haystack[i+j])
	// 				{
	// 					i += j;
	// 					break;
	// 				}
	// 			}
	// 		}
	// 	}

	// 	return false;
	// }

	int32_t c_str_len(const char *c_str) {
		assert(c_str != NULL);

		int32_t count = 0;

		while(c_str[count] != '\0') {
			count += 1;
		}

		return count;
	}

	int32_t forward_find(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len) {
		assert(needle_len > 0);

		for(int i = 0; i <= (haystack_len - needle_len); ++i) {
			for(int j = 0; needle[j] == haystack[i+j]; ++j) {
				if(j+1 == needle_len) {
					return i;
				}
			}
		}

		return -1;
	}

	int32_t backward_find(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len) {
		assert(needle_len > 0);

		for(int i = (haystack_len - needle_len); i > -1; --i) {
			for(int j = 0; needle[j] == haystack[i+j]; ++j) {
				if(j+1 == needle_len) {
					return i;
				}
			}
		}

		return -1;
	}

	// Boyer-Moore
	int32_t forward_find_bm(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len) {
		assert(needle_len > 0);

		int values_table[256];
		for(int i = 0; i < 256; ++i) {
			values_table[i] = needle_len;
		}
		values_table[needle[needle_len-1]] = needle_len; //overwrite if same value appears also before
		for(int i = 0; i < needle_len - 1; ++i) { //exclude last
			values_table[needle[i]] = needle_len - 1 - i;
		}
		// values_table[needle[needle_len-1]] = needle_len; //@ but is this really correct?
		// for(int i = 0; i < 256; ++i) {
		// 	printf("%d (%c): %d\n", i, i, values_table[i]);
		// }

		int i = -1; i += needle_len;
		while(i < haystack_len) {
			// printf("%c\n", haystack[i]);

			for(int j = 0; needle[needle_len-1-j] == haystack[i-j]; ++j) {
				if(j+1 == needle_len) {
					return i - (needle_len - 1);
				}
			}

			i += values_table[haystack[i]];
		}

		return -1;
	}

	// Boyer-Moore
	int32_t backward_find_bm(const uint8_t *haystack, int32_t haystack_len, const uint8_t *needle, int32_t needle_len) {
		assert(needle_len > 0);

		int values_table[256];
		for(int i = 0; i < 256; ++i) {
			values_table[i] = needle_len;
		}
		values_table[needle[0]] = needle_len; //overwrite if same value appears also after
		for(int i = needle_len - 1; i > 0; --i) { //exclude first
			values_table[needle[i]] = i;
		}
		// values_table[needle[needle_len-1]] = needle_len; //@ but is this really correct?
		// for(int i = 0; i < 256; ++i) {
		// 	printf("%d (%c): %d\n", i, i, values_table[i]);
		// }

		int i = haystack_len; i -= needle_len;
		while(i > -1) {
			// printf("%c\n", haystack[i]);

			for(int j = 0; needle[j] == haystack[i+j]; ++j) {
				if(j+1 == needle_len) {
					return i;
				}
			}

			i -= values_table[haystack[i]];
		}

		return -1;
	}

	void ZeroMemory(uint8_t * Memory, int64_t NumBytes)
	{
		for(int i = 0; i < NumBytes; ++i)
		{
			Memory[i] = 0;
		}
	}

	const char *filepath_get_basename(const char *filepath){
		assert(filepath);

		int length = strlen(filepath);
		assert(length > 0); // ?
		const char *p = filepath + length - 1;
		for(; p > filepath; --p){
			if(*p == '/'){
				p += 1;
				break;
			}
		}

		return p;
	}

	const char *basename_get_extension(const char *basename){
		assert(basename);

		int length = strlen(basename);
		assert(length > 0); // ?
		const char *p = basename + length - 1;
		for(; p > basename; --p){
			if(*p == '.'){
				p += 1;
				break;
			}
		}

		if(p == basename){ // ".file" and "file" do not have basenames
			return "";
		}

		return p;
	}

	// nanosecond accuracy
	double get_time_secs()
	{
		struct timespec t;
		clock_gettime(CLOCK_REALTIME, &t);
//		clock_gettime(CLOCK_THREAD_CPUTIME_ID, &t);
		return (t.tv_sec + (t.tv_nsec / 1000000000.0));
	}

	long get_time_us()
	{
		timeval time;
		gettimeofday(&time, NULL);
		return time.tv_sec * 1000000 + time.tv_usec;
	}

	void hash_table_init(HashTable *table)
	{
		for (int i = 0; i < HASH_TABLE_SIZE; ++i)
		{
			table->data[i] = 0;
		}
		table->num_occupied_slots = 0;
	}

	void hash_table_store(HashTable *table, const char *str)
	{
		unsigned int hash = 0;
		for (const char *p = str; *p; ++p)
		{
			hash += *p;
		}
		unsigned int index = hash % HASH_TABLE_SIZE;
	
		// we want to keep 1 empty slot, because otherwise when the table is full the lookup code will loop forever.
		assert(table->num_occupied_slots < HASH_TABLE_SIZE-1);
	
		while (table->data[index])
		{
			if (strcmp(table->data[index], str) == 0)
			{
				// string already there
				return;
			}
			index += 1;
			index %= HASH_TABLE_SIZE;
		}
		table->data[index] = str;
		table->num_occupied_slots += 1;
	}

	bool hash_table_has(HashTable *table, const char *str)
	{
		unsigned int hash = 0;
		for (const char *p = str; *p; ++p)
		{
			hash += *p;
		}
		unsigned int index = hash % HASH_TABLE_SIZE;
	
	//	int num_comparsions = 0;
		bool found = false;
		while (table->data[index])
		{
	//		num_comparsions += 1;
			if (strcmp(table->data[index], str) == 0)
			{
				found = true;
				break;
			}
			index += 1;
			index %= HASH_TABLE_SIZE;
		}
	//	printf("num_comparsions for \"%s\": %d\n", str, num_comparsions);
		return found;
	}

	void hash_table_print(HashTable *table)
	{
		for (int i = 0; i < HASH_TABLE_SIZE; ++i)
		{
			if (table->data[i])
			{
				unsigned int hash = 0;
				for (const char *p = table->data[i]; *p; ++p)
				{
					hash += *p;
				}
				unsigned int index = hash % HASH_TABLE_SIZE;
				printf("%d: %s (%d)\n", i, table->data[i], index);
			}
			else
			{
				printf("%d: -\n", i);
			}
		}
	}

//stringSlice make_stringSlice(const char *Original)
//{
//	stringSlice Slice;
//	Slice.Data = Original;
//	Slice.Length = strlen(Original);
//	return Slice;
//}
//
//void print_stringSlice(stringSlice Slice)
//{
//	const int MAX_C_STRING = 1024;
//	char c_string[MAX_C_STRING];
//
//	assert(Slice.Length < MAX_C_STRING-1);
//	for(int i = 0; i < Slice.Length; ++i)
//	{
//		c_string[i] = Slice.Data[i];
//	}
//	c_string[Slice.Length] = '\0';
//
//	printf("%s", c_string);
//}
}

#endif

#endif
