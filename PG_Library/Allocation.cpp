#include "pch.h"
#include "Allocation.h"

void* Alloc(size_t size, unsigned char dimensions, ...)
{
	va_list valist;

	/* initialize valist for num number of arguments */
	va_start(valist, dimensions);

	int* values = (int*)malloc(sizeof(int) * dimensions);
	
	int numberOfElements = 0;
	for (int i = 0; i < dimensions; i++)
	{
		int value = va_arg(valist, int);
		numberOfElements += value;
		values[i] = value;
	}

	/* clean memory reserved for valist */
	va_end(valist);

	void* result = malloc(size * numberOfElements);

	return result;
}