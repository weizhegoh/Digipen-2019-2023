/******************************************************************************
 * @Author:     Swavek Wlodkowski
 * @Course:     CS 225
 *
 * This file contains helper code for detecting memory leaks. Do not change it.
 *
 * Including this header file and compiling the program with a pre-processor
 * macro MEMORY_DEBUG defined lets a program output the information about
 * memory blocks that are allocated with new/new[], and not released with
 * a matching call to delete/delete[].
 *
 * Operators implemented within this file are beyond the scope of CS 170, and
 * are covered in detail late in CS 225.
 *****************************************************************************/

#ifndef MEMORY_ALLOC_H
#define MEMORY_ALLOC_H

#ifdef MEMORY_DEBUG

#include <cstdio>
#include <cstring>
#include <cstdlib>

namespace debug
{
	std::size_t leak_index = 0;

	struct AllocationRecord
	{
		void* address;
		std::size_t size;
		bool is_array;
		bool is_used;
	}* allocation_records = nullptr;

	bool is_attached = false;
	bool issues_found = false;

	void check_memory()
	{
		if (allocation_records)
		{
			std::size_t count = 0;
			std::size_t size = 0;
			for (std::size_t i = 0; i < leak_index; ++i)
			{
				AllocationRecord& leak = allocation_records[i];
				if (leak.is_used)
				{
					++count;
					size += leak.size;
					if (leak.is_array)
					{
						std::printf(
							"\n* LEAK: %zu     (array %p)",
							leak.size,
							leak.address);
					}
					else
					{
						std::printf(
							"\n* LEAK: %zu     (%p)",
							leak.size,
							leak.address);
					}
				}
			}
			if (count)
			{
				std::printf("\nLEAKS: %zu (%zu bytes)\n", count, size);
			}
			else if (issues_found)
			{
				std::printf("\nSOME FREE STORE ISSUES WERE FOUND\n");
			}
			else
			{
				std::printf("\nNO FREE STORE LEAKS WERE FOUND\n");
			}
			std::free(allocation_records);
			allocation_records = nullptr;
		}
		else if (issues_found)
		{
			std::printf("\nSOME FREE STORE ISSUES WERE FOUND\n");
		}
		else
		{
			std::printf("\nNO FREE STORE OPERATIONS WERE FOUND\n");
		}
	}

	void* allocate_memory(std::size_t size, bool is_array)
	{
		if (!is_attached)
		{
			is_attached = true;
			std::atexit(check_memory);
		}
		const std::size_t ar_size = sizeof(AllocationRecord);
		AllocationRecord* temp = static_cast<AllocationRecord*>(
			std::malloc(ar_size * (leak_index + 1))
		);
		if (allocation_records)
		{
			std::memcpy(temp, allocation_records, ar_size * leak_index);
			std::free(allocation_records);
		}
		allocation_records = temp;
		void* result = std::malloc(size);
		allocation_records[leak_index++] = {result, size, is_array, true};
		return result;
	}

	void deallocate_memory(void* address, bool is_array)
	{
		if (!is_attached)
		{
			is_attached = true;
			std::atexit(check_memory);
		}
		if (!address)
		{
			return;
		}
		if (allocation_records)
		{
			bool found_array = false;
			bool found_non_array = false;
			for (std::size_t i = 0; i < leak_index; ++i)
			{
				AllocationRecord& leak = allocation_records[i];
				if (leak.address == address)
				{
					if (leak.is_used)
					{
						if (leak.is_array)
						{
							if (!is_array)
							{
								issues_found = true;
								std::printf(
									"Calling delete on an array.\n");
							}
						}
						else
						{
							if (is_array)
							{
								issues_found = true;
								std::printf(
									"Calling delete[] on a non-array.\n");
							}
						}
						leak.is_used = false;
						std::free(address);
						return;
					}
					else
					{
						if (leak.is_array)
						{
							found_array = true;
						}
						else
						{
							found_non_array = true;
						}
					}
				}
			}
			issues_found = true;
			if (is_array)
			{
				if (found_array)
				{
					std::printf("Potential double-delete[] of an array.\n");
				}
				else if (found_non_array)
				{
					std::printf("Potential double-delete[] of a non-array.\n");
				}
				else
				{
					std::printf(
						"Calling delete[] on a non-allocated memory.\n");
				}
			}
			else
			{
				if (found_non_array)
				{
					std::printf("Potential double-delete of a non-array.\n");
				}
				else if (found_array)
				{
					std::printf("Potential double-delete of an array.\n");
				}
				else
				{
					std::printf(
						"Calling delete on a non-allocated memory.\n");
				}
			}
		}
		else
		{
			std::printf("No memory has been allocated yet.\n");
		}
	}

	namespace
	{
		struct EnableDebug
		{
			EnableDebug()
			{
				if (!is_attached)
				{
					is_attached = true;
					std::atexit(check_memory);
				}
			}
		};

		static EnableDebug _enableDebug;
	}
}

void* operator new(std::size_t size)
{
	return debug::allocate_memory(size, false);
}

void* operator new[](std::size_t size)
{
	return debug::allocate_memory(size, true);
}

void operator delete(void* address, std::size_t) noexcept
{
	debug::deallocate_memory(address, false);
}

void operator delete(void* address) noexcept
{
	debug::deallocate_memory(address, false);
}

void operator delete[](void* address, std::size_t) noexcept
{
	debug::deallocate_memory(address, true);
}

void operator delete[](void* address) noexcept
{
	debug::deallocate_memory(address, true);
}

#endif // MEMORY_DEBUG

#endif // MEMORY_ALLOC_H
