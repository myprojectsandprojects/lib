#ifndef LIB_H
#define LIB_H

// Nothing in here should be specific to any platform
// But we do depend on C standard library (which should be cross platform?)

#include <stdio.h>
#include <stdlib.h>
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

#endif


