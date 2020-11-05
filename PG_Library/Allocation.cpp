#include "pch.h"
#include "Allocation.h"

void* Alloc(size_t size, unsigned char dimensions, ...)
{
	if (dimensions < 1)
	{
		return nullptr;
	}

	va_list valist;

	/* initialize valist for num number of arguments */
	va_start(valist, dimensions);

	int numberOfElements = 0;
	void* result = nullptr;

	if (dimensions == 1)
	{
		numberOfElements = va_arg(valist, int);
		result = malloc(size * numberOfElements);
	}
	else
	{
		int* values = (int*)malloc(sizeof(int) * dimensions);

		for (int i = 0; i < dimensions; i++)
		{
			int value = va_arg(valist, int);
			numberOfElements += value;
			values[i] = value;
		}

		result = malloc(size * numberOfElements);
		
		free(values);
	}

	/* clean memory reserved for valist */
	va_end(valist);

	return result;
}