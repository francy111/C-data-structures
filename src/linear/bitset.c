/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/linear/bitset.h"
#include <stdlib.h>
#include <string.h>

 /**
  * Struct that represent a bitset
  */
typedef struct bitset {
	
	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Istead of storing the bits in a bool array, we use an array of unsigned integers, where each integer contains 32 bits */
	uint32_t* bits;

	/* Number of bits in the set */
	size_t set_size;
} bitset;

/**
 *  Creates a bitset of the given size
 */
bitset* bitset_create(size_t size) {

	bitset* b = NULL;

	if (0 < size && size < SIZE_MAX) {

		b = (bitset*)malloc(sizeof(bitset));

		if (b) {

			b->set_size = size;
			b->bits = (uint32_t*)calloc((b->set_size + 31) / 32, sizeof(uint32_t));

			// If the calloc failed cancel the creation
			if (!b->bits) {

				free(b);
				b = NULL;
			}
		}
	}

	return b;
}

/**
 *  Deletes the given bitset
 */
void bitset_delete(bitset** b) {

	if (b && *b) {

		free(&(*b)->bits);
		memset(*b, 0, sizeof(bitset));
		free(*b);
		*b = NULL;
	}
	return;
}

/**
 *  Sets the i -th bit of the set to 1
 */
void bitset_set(bitset* b, size_t i) {

	if (b && i < b->set_size) {

		b->bits[i / 32] |= (1U << (i % 32));
	}
}

/**
 *  Sets all the bits of the set to 1
 */
void bitset_set_full(bitset* b) {

	if (b) memset(b->bits, 0xFF, sizeof(b->bits));
}

/**
 *  Sets the i -th bit of the set to 0
 */
void bitset_unset(bitset* b, size_t i) {

	if (b && i < b->set_size) {

		b->bits[i / 32] &= ~(1U << (i % 32));
	}
}

/**
 *  Sets all the bits of the set to 1
 */
void bitset_unset_full(bitset* b) {

	if (b) memset(b->bits, 0x00, sizeof(b->bits));
}

/**
 *  Returns the value of the i -th bit of the set
 */
bool bitset_get(bitset* b, size_t i) {

	return (b && i < b->set_size) ? (b->bits[i / 32] & (1U << (i % 32))) : false;
}

/**
 *  Toggles the i -th bit of the set
 */
void bitset_toggle(bitset* b, size_t i) {

	if (b && i < b->set_size) {

		b->bits[i / 32] ^= ~(1U << (i % 32));
	}
}

/**
 *  Toggles all the bits in the set
 */
void bitset_toggle_full(bitset* b) {

	if (b) for (size_t i = 0; i < b->set_size; i++) b->bits[i] ^= ~0;
}

/**
 *  Returns the number of positive bits in the set
 */
size_t bitset_count(bitset* b) {

	size_t count = 0;
	if (b) {
		
		for (size_t i = 0; i < b->set_size; i++)
			if (bitset_get(b, i))
				count++;
	}
	return count;
}

/**
 *  Toggles the number of bits in the set
 */
size_t bitset_get_size(bitset* b) {

	return b ? b->set_size : 0;
}
