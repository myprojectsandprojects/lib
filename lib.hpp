#include <stdint.h>

//void integer_as_binary(uint8_t Buffer[65], uint64_t Value);
uint8_t *i2b(uint64_t Value); // integer as binary
void unpack_double_value(double Value, bool *Sign, int *Exponent, double *Mantissa);
double round(double D);
//int get_random_int(int Min, int Max);
double random_num();
int random_num(int RangeStart, int RangeEnd);


