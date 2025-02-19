/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HASHMAP__H
#define HASHMAP__H

#include <stdlib.h>
#include <stdbool.h>

 /**
  * Struct that represent an hashmap, mapping keys into values
  * 
  * keys are strings
  * values are of generic type
  */
typedef struct hashmap hashmap;

/**
 * Creates an hashmap that stores values that are as big as element_size
 * and can store at most max_size elements
 */
hashmap* hash_create(size_t max_size, size_t element_size);

/**
 * Deletes the given hashmap
 */
void hash_delete(hashmap** hash);

/**
 * Inserts a couple <key, value>
 */
void hash_put(hashmap* hash, const char* key, void* value);

/**
 * Removes the element mapped by key
 */
void hash_remove(hashmap* hash, const char* key);

/**
 * Returns the value mapped by key
 */
void* hash_get(hashmap* hash, const char* key);

/**
 * Copies the value mapped by key into buf
 */
void hash_get_2(hashmap* hash, const char* key, void* buf);

/**
 * Removes all the values in the hashmap
 * the struct itself is preserved
 */
void hash_clear(hashmap* hash);

/**
 * Returns the capacity of the hashmap
 */
size_t hash_get_capacity(hashmap* hash);

/**
 * Returns the size of the elements stored in the hash
 */
size_t hash_get_element_size(hashmap* hash);

/**
 * Sets a custom hash function
 */
void hash_set_function(hashmap* hash, size_t(*map)(const char*));

/**
 * Sets a custom hash function
 */
void hash_set_second_function(hashmap* hash, size_t(*map2)(const char*));

#endif