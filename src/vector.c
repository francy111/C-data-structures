/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../include/vector.h"

/**
 * Struct that represent a generic type vector
 *
 * Elements can be of any type
 */
typedef struct vector {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 * 
	 * Mainly, so that users wouldn't be able to modify 
	 * the value of individual fields in a wrongful way, 
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Actual pointer to the memory location storing
	 * the elements of the vectorvectorSize
	 * 
	 * Void* is used to accept any type of data
	 */
	void* elements;

	/* Vectors are fixed in size. The size is used
	 * both for creating the actual array and for
	 * any array-related operation (mainly the ones
	 * that iterate it)
	 */
	size_t vector_size;

	/* Pointer arithmetics don't make sense when using void*
	 * since the compiler can't possibly know how much space to
	 * alloc for each element, nor how many positions to move when
	 * iterating it, the element size is used to overcome this
	 */
	size_t element_size;
} vector;

/**
 *  Creates a vector with the given size, ready to store elements that are 'element_size' long
 */
vector* vec_create(size_t vector_size, size_t element_size) {
	
	vector* v = NULL;

	// Check if the sizes are correct, that is, at least 1
	if (vector_size > 0 && element_size > 0) {

		/* And not too big(consequence of passing a negative value to a size_t parameter)
		 * The maximum length of the array is LONG_MAX, so vector_size * element_size should be at most, LONG_MAX
		 */
		if (vector_size <= SIZE_MAX / element_size) {

			// Sizes are correct, we can allocate memory for the struct
			v = (vector*)malloc(sizeof(vector));

			// If the allocation was successful
			if (v != NULL) {
				v->vector_size = vector_size;
				v->element_size = element_size;
				v->elements = calloc(v->vector_size, v->element_size);

				// Check if the array's memory allocation was successful
				if (v->elements == NULL) {

					// In case the allocation failed, revert the allocation for vector
					free(v);
					v = NULL;
				}
			}
		}
	}
	return v;
}

/**
 * Deletes the given vector, since memory is allocated dinamically
 * the following actions are performed:
 *   The memory allocated for storing the actual array of data is freed
 *   The memory allocated for the struct itself is freed
 *   The pointer to the struct (*v) is set to NULL
 *
 * N.B
 * If the added elements were passed as struct to pointers, each element of
 * the array would contain a copy of the struct, after deletion that memory would be free
 * However, if it was passed as a pointer to a pointer to the struct, then each
 * array element would store the pointer to the struct, after deletion, the memory for
 * said structs would still be allocated, SCEGLIERE SE USARE UN FLAG O LASCIARE RESPONSABILITA
 *  A UTENTE
 */
void vec_delete(vector** v) {

	// Access the array only if the pointer is valid
	if (v != NULL && *v != NULL) {

		// Zero the memory used for the array and free it
		memset((*v)->elements, 0, (*v)->vector_size * (*v)->element_size);
		free((*v)->elements);

		// Zero the memory used for the whole struct and free it
		memset((*v), 0, sizeof(vector));
		free(*v);

		// Set the pointer to the struct to NULL so it can't be used to access unallocated memory
		*v = NULL;
	}
}

/**
 * Performs the action v[i] = x
 *
 * To be more specific, it copies the value pointed to by x into the
 * i -th position of the vector. We assume the pointed value is of the size
 * given when creating the vector
 */
void vec_insert_at(vector* v, void* x, size_t i) {

	if (v != NULL && i < v->vector_size && x != NULL) {

		/* Get the i - th element's position
		 * Casting the array as char* lets us perform pointer operations
		 * As we know that every element is 1 byte, the i-th element will be
		 * i times the size of each element, starting from the base (v->elements)
		 */
		void* ithPtr = (char*)v->elements + i * v->element_size;

		// Write the value pointed to by x into the found position
		memcpy(ithPtr, x, v->element_size);
	}
}

/**
 * Removes the i -th element from the vector
 * In other words, Performs the action v[i] = del.value
 *
 * To be more specific, it sets the whole i -th element
 * (all the bytes of such element, based on element size) to be zero (0)
 */
void vec_remove_at(vector* v, size_t i) {

	if (v != NULL && i < v->vector_size) {
		/* Get the i - th element's position
		 * Casting the array as char* lets us perform pointer operations
		 * As we know that every element is 1 byte, the i-th element will be
		 * i times the size of each element, starting from the base (v->elements)
		 */
		void* ithPtr = (char*)v->elements + i * v->element_size;

		// Write all zeros on all bytes of the i -th element
		memset(ithPtr, 0, v->element_size);
	}
}

/**
 * Returns a pointer to the i -th element of v, so v(i)
 *
 * To be more specific, it returns the memory address
 * of the first byte of the i -th element. This is returned
 * as a void pointer, which should be casted by the caller
 */
void* vec_get_at(vector* v, size_t i) {

	void* res = NULL;

	if (v != NULL && i < v->vector_size) {

		/* Get the i - th element's position
		 * Casting the array as char* lets us perform pointer operations
		 * As we know that every element is 1 byte, the i-th element will be
		 * i times the size of each element, starting from the base (v->elements)
		 */
		res = (void*)((char*)v->elements + i * v->element_size);
	}
	return res;
}

/**
 * Copies the i -th element of the array, v(i) inside the buffer
 * pointed to by buf
 *
 */
void vec_get_2_at(vector* v, size_t i, void* buf) {

	if (v != NULL && i < v->vector_size && buf != NULL) {

		/* Get the i - th element's position
		 * Casting the array as char* lets us perform pointer operations
		 * As we know that every element is 1 byte, the i-th element will be
		 * i times the size of each element, starting from the base (v->elements)
		 */
		void* ithPtr = (void*)((char*)v->elements + i * v->element_size);
		memcpy(buf, ithPtr, v->element_size);
	}
}

/**
 * Returns the vector size of v
 */
size_t vec_get_size(vector* v) {
	return (v != NULL) ? v->vector_size : 0;
}

/**
 * Returns the element size of v
 */
size_t vec_get_element_size(vector* v) {
	return (v != NULL) ? v->element_size : 0;
}

/**
 * Checks if the element pointed to by x is present in the vector
 * 
 * The value returned is actually it's position in the array
 * from 1 to vector_size (not from 0 to vector_size-1)
 */
short vec_contains(vector* v, void* x) {

	int isPresent = 0, index = 0;

	// Iterate through the vector and compare with each element
	if (v != NULL && x != NULL) {

		for (index = 0; index < v->vector_size; index++) {

			// Compare the i -th element with the element pointed to by x
			isPresent = (memcmp((char*)v->elements + index * v->element_size, x, v->element_size) == 0);
			if (isPresent)
				break;
		}
	}

	return isPresent ? index + 1 : 0;
}

/**
 * Checks if the i -th element of the vector is empty or not
 *
 * With empty we mean that each byte is set to 0
 */
short vec_is_index_empty(vector* v, size_t i) {

	int empty = -1; // vector is null

	if (v != NULL && i < v->vector_size) {

		char* ithPtr = (char*)v->elements + i * v->element_size;
		char* toCmp = '\0';
		empty = 1; // Assume the position is empty and check if it's not

		for (int j = 0; j < v->element_size; j++) {
			if (memcmp(ithPtr + j * sizeof(char), &toCmp, sizeof(char))) {
				empty = 0;
				break;
			}
		}
	}

	return empty;
}

/** 
 * Empties every element, but
 * doesn't deallocate the memory
 */
void vec_clear(vector* v) {

	if (v != NULL) {

		// Consider the whole array as an array of chars (so we can write byte by byte)
		char* ptr = (char*)v->elements;

		memset(ptr, '\0', v->vector_size * v->element_size);
	}
}