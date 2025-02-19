/*
 * Copyright (c) 2024 Biribo' Francesco
 *
 * Permission to use, copy, modify, and distribute this software for any purpose with or without fee is hereby granted, provided that the above copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "../../include/non-linear/hashmap.h"
#include "../../include/linear/vector.h"
#include "../../include/linear/bitset.h"
#include <string.h>

/* Hash function that transforms the string key into an unsigned integer */
size_t hash_util_default_hash(const char* key);
size_t hash_util_default_second_hash(const char* key);
 /**
  * Struct that represent an hashmap, mapping keys into values
  *
  * keys are strings
  * values are of generic type
  */
typedef struct hashmap {

	/* The actual definition of the struct is placed
	 * here and not in the header file to try and achieve incapsulation
	 *
	 * Mainly, so that users wouldn't be able to modify
	 * the value of individual fields in a wrongful way,
	 * such as setting a bigger size/ element size to read/write
	 * in unallocated memory (or, still, memory that isn't 'ours')
	 */

	/* Array of generic type of values */
	vector* values;

	/* Bitset used to determine if an element in the vector is occupied or not (could be 0 but 0 is the actual element) */
	bitset* occupied;

	/* Hash function, can be specified */
	size_t(*hash_func)(const char*);

	/* second hash function, can be specified, used for collisions */
	size_t(*second_hash)(const char*);
} hashmap;

/**
 * Creates an hashmap that stores values that are as big as element_size
 * and can store at most capacity elements
 */
hashmap* hash_create(size_t capacity, size_t element_size) {

	hashmap* hash = NULL;

	if (0 < capacity && 0 < element_size && capacity <= SIZE_MAX/element_size) {
	
		hash = (hashmap*)malloc(sizeof(hashmap));
		if (hash) {

			hash->values = vec_create(capacity, sizeof(const char*) + element_size);
			if (hash->values) {

				hash->occupied = bitset_create(capacity);
				if (hash->occupied) {

					hash->hash_func = *hash_util_default_hash;
					hash->hash_func = *hash_util_default_second_hash;
				}
				else {

					vec_delete(&hash->values);
					free(hash);
					hash = NULL;
				}
			}
			else {

				free(hash);
				hash = NULL;
			}
		}
	}
	return hash;
}

/**
 * Deletes the given hashmap
 */
void hash_delete(hashmap** hash) {

	if (hash && *hash) {
	
		vec_delete(&(*hash)->values);
		bitset_delete(&(*hash)->occupied);
		memset(*hash, 0, sizeof(hash));
		free(*hash);
		*hash = NULL;
	}
	return;
}

/**
 * Inserts a couple <key, value>
 */
void hash_put(hashmap* hash, const char* key, void* value) {

	if (hash && key) {

		// Proceed if not full
		if (bitset_count(hash->occupied) < bitset_get_size(hash->occupied)) {

			size_t index = hash->hash_func(key) % hash_get_capacity(hash);
			size_t step = hash->second_hash(key);


			size_t max_iterations = hash_get_capacity(hash);

			for (size_t i = 0; i < max_iterations; i++) {

				// If the position is free
				if (!bitset_get(hash->occupied, index)) {

					// Insert the couple <key, value>
					void* to_insert = malloc(sizeof(const char*) + hash_get_element_size(hash));
					if (to_insert) {

						// The first part of the memory will be used to store the pointer to key
						memcpy((char*)to_insert, &key, sizeof(const char*));

						// The second part to store the actual value
						memcpy((char*)to_insert + sizeof(const char*), value, hash_get_element_size(hash));

						vec_insert_at(hash->values, to_insert, index);
						bitset_set(hash->occupied, i);
					}

					break;
				}

				index = (index + step) % hash_get_capacity(hash);
			}
		}
	}
	return;
}

/**
 * Removes the element mapped by key
 */
void hash_remove(hashmap* hash, const char* key) {

	if (hash && key) {

		// Proceed if the map is not empty
		if (bitset_count(hash->occupied) > 0) {

			size_t index = hash->hash_func(key) % hash_get_capacity(hash);
			size_t step = hash->second_hash(key);


			size_t max_iterations = hash_get_capacity(hash);

			for (size_t i = 0; i < max_iterations; i++) {

				// If the position is ocuupied
				if (bitset_get(hash->occupied, index)) {

					// Geyt the couple <key, value>
					void* read = vec_get_at(hash->values, index);

					// If the key is the one we were looking for
					char* read_key = *(char**)read;
					if (strncmp(read_key, key, strlen(key)) == 0) {

						// Remove the couple
						vec_remove_at(hash->values, index);
						free(read_key);
						bitset_unset(hash->occupied, index);
						break;
					}
				}
				index = (index + step) % hash_get_capacity(hash);
			}
		}
	}
	return;
}

/**
 * Returns the value mapped by key
 */
void* hash_get(hashmap* hash, const char* key) {

	void* val = NULL;

	if (hash && key) {

		// Proceed if the map is not empty
		if (bitset_count(hash->occupied) > 0) {

			size_t index = hash->hash_func(key) % hash_get_capacity(hash);
			size_t step = hash->second_hash(key);

			size_t max_iterations = hash_get_capacity(hash);

			for (size_t i = 0; i < max_iterations; i++) {

				// If the position is ocuupied
				if (bitset_get(hash->occupied, index)) {

					// Geyt the couple <key, value>
					void* read = vec_get_at(hash->values, index);

					// If the key is the one we were looking for
					char* read_key = *(char**)read;
					if (strncmp(read_key, key, strlen(key)) == 0) {

						val = (void*)((char*)read + sizeof(const char*));
						break;
					}
				}
				index = (index + step) % hash_get_capacity(hash);
			}
		}
	}
	return val;
}

/**
 * Copies the value mapped by key into buf
 */
void hash_get_2(hashmap* hash, const char* key, void* buf) {

	if (hash && key) {

		size_t index = hash->hash_func(key) % hash_get_capacity(hash);
		size_t step = hash->second_hash(key);


		size_t max_iterations = hash_get_capacity(hash);

		for (size_t i = 0; i < max_iterations; i++) {

			// If the position is ocuupied
			if (bitset_get(hash->occupied, index)) {

				// Geyt the couple <key, value>
				void* read = vec_get_at(hash->values, index);

				// If the key is the one we were looking for
				char* read_key = *(char**)read;
				if (strncmp(read_key, key, strlen(key)) == 0) {

					memcpy(buf, (char*)read + sizeof(const char*), hash_get_element_size(hash));
					break;
				}
			}
			index = (index + step) % hash_get_capacity(hash);
		}
	}
	return;
}

/**
 * Removes all the values in the hashmap
 * the struct itself is preserved
 */
void hash_clear(hashmap* hash) {

	if (hash) vec_clear(hash->values);
	return;
}

/**
 * Returns the capacity of the hashmap
 */
size_t hash_get_capacity(hashmap* hash) {

	return hash ? vec_get_size(hash->values) : 0;
}

/**
 * Returns the size of the elements stored in the hash
 */
size_t hash_get_element_size(hashmap* hash) {

	return hash ? vec_get_element_size(hash->values) : 0;
}

/* Hash function that transforms the string key into an unsigned integer */
size_t hash_util_default_hash(const char* key) {

	size_t hash = 5381;
	int c;
	while (c = *key++) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}
size_t hash_util_default_second_hash(const char* key) {

	size_t hash = 1431655765;

	while (*key) {
		hash ^= *key++;
		hash = (hash << 5) + hash;
	}

	return 0;
}

/**
 * Sets a custom hash function
 */
void hash_set_function(hashmap* hash, size_t (*map)(const char*)) {

	if (hash && map) hash->hash_func = map;
	return;
}

/**
 * Sets a custom hash function
 */
void hash_set_second_function(hashmap* hash, size_t(*map2)(const char*)) {

	if (hash && map2) hash->hash_func = map2;
	return;
}