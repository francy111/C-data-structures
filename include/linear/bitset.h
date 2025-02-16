/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef BITSET__H
#define BITSED__H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

 /**
  * Struct that represent a bitset
  */
typedef struct bitset bitset;

/**
 *  Creates a bitset of the given size
 */
bitset* bitset_create(size_t size);

/**
 *  Deletes the given bitset
 */
void bitset_delete(bitset** b);

/**
 *  Sets the i -th bit of the set to 1
 */
void bitset_set(bitset* b, size_t i);

/**
 *  Sets all the bits of the set to 1
 */
void bitset_set_full(bitset* b);

/**
 *  Sets the i -th bit of the set to 0
 */
void bitset_unset(bitset* b, size_t i);

/**
 *  Sets all the bits of the set to 1
 */
void bitset_unset_full(bitset* b);

/**
 *  Returns the value of the i -th bit of the set
 */
bool bitset_get(bitset* b, size_t i);

/**
 *  Toggles the i -th bit of the set
 */
void bitset_toggle(bitset* b, size_t i);

/**
 *  Toggles all the bits in the set
 */
void bitset_toggle_full(bitset* b);

/**
 *  Returns the number of positive bits in the set
 */
size_t bitset_count(bitset* b);

/**
 *  Toggles the number of bits in the set
 */
size_t bitset_get_size(bitset* b);

#endif
