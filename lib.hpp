#ifndef LIB_H
#define LIB_H

// Nothing in here should be specific to any platform
// But we do depend on C standard library (which should be cross platform?)

#include <stdio.h>
#include <stdlib.h>
#include <time.h> //@ is this linux specific?
//#include <assert.h>

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

bool ReadFile(const char *FileName, u8 **Contents, size_t *NumBytes);
bool WriteFile(const char *FileName, u8 *Contents, size_t NumBytes);
bool ReadTextFile(const char *FileName, char **Contents); // returns 0-terminated contents
//bool WriteTextFile(const char *FileName, char **Contents); // expects 0-terminated contents

// Determines the length of an array at compile time.
#define COUNT(a) (sizeof(a) / sizeof(a[0]))

#define MIN(a, b) (a) < (b) ? (a) : (b)
#define MAX(a, b) (a) > (b) ? (a) : (b)

// Using this on a float or a double before casting it to an integer yields a more accurate result.
#define CAST_ROUND(n) ((n) + (((n) > 0) ? 0.5 : -0.5))

//void integer_as_binary(uint8_t Buffer[65], uint64_t Value);
uint8_t *i2b(uint64_t Value); // integer as binary
void unpack_double_value(double Value, bool *Sign, int *Exponent, double *Mantissa);
//double round(double D);
//int get_random_int(int Min, int Max);
double random_num();
int random_num(int RangeStart, int RangeEnd);
bool find_bytes(uint8_t const *needle, int32_t needle_len, uint8_t const *haystack, int32_t haystack_len, int32_t *p_offset);


#ifdef LIB_INCLUDE_IMPLEMENTATION

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

bool find_bytes(uint8_t const *needle, int32_t needle_len, uint8_t const *haystack, int32_t haystack_len, int32_t *p_offset)
{
	for(int i = 0; i < haystack_len; ++i)
	{
		if(haystack[i] == needle[0])
		{
			for(int j = 1; j <= needle_len; ++j)
			{
				if(j == needle_len)
				{
					*p_offset = i;
					// *ptr = &haystack[i];
					return true;
				}
				if(needle[j] != haystack[i+j])
				{
					i += j;
					break;
				}
			}
		}
	}

	return false;
}

#endif

#endif
