#include "hash.h"
#include "hash_table.h"
#include "debug.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define HASH_LENGTH 32
#define HASH_TABLE_SIZE 3

#ifndef INTS
int is_equal(void *first, void *second)
{
	return strcmp(first, second);
}
#else
int is_equal(void *first, void *second)
{
	return (int)first == (int)second;
}
#endif // INTS

#ifndef DOUBLE_HASHING
unsigned int hash1(const char* str) {
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % HASH_TABLE_SIZE; // reduce to a fixed range
}
#else
unsigned int hash1(const char* str) {
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
		hash = c + (hash << 6) + (hash << 16) - hash;

	return hash % HASH_TABLE_SIZE;
}

unsigned int hash2(const char* str) {
	unsigned int hash = 0;
	int c;

	while ((c = *str++))
		hash = c + (hash << 3) + (hash << 11) - hash;

	return hash % (HASH_TABLE_SIZE - 1) + 1;
}

#endif // DOUBLE_HASHING

hash_table_t *hash_table_ctor()
{
	hash_table_t *buff = malloc(sizeof(hash_table_t));
	buff->entries = malloc(sizeof(hash_table_entry_t) * HASH_TABLE_SIZE);

	for (int i = 0; i < HASH_TABLE_SIZE; i++)
		buff->entries[i] = hash_table_entry_default_ctor();


	buff->size = HASH_TABLE_SIZE;
	buff->count = 0;
	return buff;
}

hash_table_entry_t *hash_table_entry_default_ctor()
{
	hash_table_entry_t *buff = malloc(sizeof(hash_table_entry_t));
	buff->key = 0;
	buff->data = NULL;
	buff->is_deleted = 1;
	return buff;
}
hash_table_entry_t *hash_table_entry_ctor(const char *key,
                                          const void* data)
{
	hash_table_entry_t *buff = malloc(sizeof(hash_table_entry_t));
	buff->key = hash1(key);
#ifndef INTS
	buff->data = malloc(strlen(data) + 1);
	memcpy(buff->data, data, strlen(data));
#else
	buff->data = data;
#endif // INTS
	buff->is_deleted = 0;
	return buff;
}


void hash_table_dtor(hash_table_t *table)
{
	for (int i = 0; i < table->size; i++)
	{
#ifndef INTS
		free(table->entries[i]->data);
#endif // INTS
		free(table->entries[i]);
	}
	free(table->entries);
	free(table);
}

void print_entry(hash_table_entry_t *entry)
{
#ifndef INTS
	fprintf(stdout, " | %16u | %16s\n", entry->key, entry->data);
#else
	fprintf(stdout, " | %16u | %16d\n", entry->key, entry->data);
#endif // INTS
}

#ifndef DOUBLE_HASHING
void insert(hash_table_t **table, char *key, void *data)
{
	hash_table_t *buff = *table;
	if ((buff->count == HASH_TABLE_SIZE))
	{
		fprintf(stdout, "ERROR! Hash table is full");
		return;
	}
	buff->count++;
	int idx = hash1(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
		idx = (idx + 1) % HASH_TABLE_SIZE; // linear probing

	buff->entries[idx % HASH_TABLE_SIZE] = hash_table_entry_ctor(key, data);
}

void *search(hash_table_t *table, char *key, void *data)
{
	hash_table_t *buff = table;

	unsigned int idx = hash1(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
	{
		if (buff->entries[idx % HASH_TABLE_SIZE]->key == hash1(key) &&
		    is_equal(buff->entries[idx % HASH_TABLE_SIZE]->data, data))
			return buff->entries[idx % HASH_TABLE_SIZE]->data;

		idx = (idx + 1) % HASH_TABLE_SIZE;
	}

	return NULL;
}
void delete(hash_table_t *table, char *key, void *data)
{
	hash_table_t *buff = table;

	unsigned int idx = hash1(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
	{
		if (buff->entries[idx % HASH_TABLE_SIZE]->key == hash1(key) &&
		    is_equal(buff->entries[idx % HASH_TABLE_SIZE]->data, data))
			buff->entries[idx % HASH_TABLE_SIZE] = hash_table_entry_default_ctor();

		idx = (idx + 1) % HASH_TABLE_SIZE;
	}

	return NULL;
}

#else
void insert(hash_table_t **table, char *key, void *data)
{
	hash_table_t *buff = *table;

	if (buff->count == HASH_TABLE_SIZE)
	{
		fprintf(stdout, "ERROR! Hash table is full");
		return;
	}
	buff->count++;
	printf("%d\n", buff->count);

	int idx = hash1(key);
	int step = hash2(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
		idx = (idx + step + 1) % HASH_TABLE_SIZE;

	buff->entries[idx % HASH_TABLE_SIZE] = hash_table_entry_ctor(key, data);
}

void *search(hash_table_t *table, char *key, void *data)
{
	hash_table_t *buff = table;

	int idx = hash1(key);
	int step = hash2(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
	{
		if (buff->entries[idx % HASH_TABLE_SIZE]->key == hash1(key) &&
		    is_equal(buff->entries[idx % HASH_TABLE_SIZE]->data, data))
			return buff->entries[idx % HASH_TABLE_SIZE]->data;
		idx = (idx + step + 1) % HASH_TABLE_SIZE;
	}

	return NULL;
}
void *delete(hash_table_t *table, char *key, void *data)
{
	hash_table_t *buff = table;

	int idx = hash1(key);
	int step = hash2(key);

	while (buff->entries[idx % HASH_TABLE_SIZE]->is_deleted == 0)
	{
		if (buff->entries[idx % HASH_TABLE_SIZE]->key == hash1(key) &&
		    is_equal(buff->entries[idx % HASH_TABLE_SIZE]->data, data))
			buff->entries[idx % HASH_TABLE_SIZE] = hash_table_entry_default_ctor();
		idx = (idx + step + 1) % HASH_TABLE_SIZE;
	}

	return NULL;
}
#endif // DOUBLE_HASHING


void print(hash_table_t *table)
{
	hash_table_t *temp = table;
	int count = 0;
	fprintf(stdout, "-----------------------------------------\n");
	fprintf(stdout, "%3s | %16s | %16s\n", "Num", "Key", "Data");
	fprintf(stdout, "-----------------------------------------\n");
	for (int i = 0; i < temp->size; i++)
	{
		fprintf(stdout, "%03d", i);
		print_entry(temp->entries[i]);
	}
}

int main(int argc, char **argv)
{
	hash_table_t *table = hash_table_ctor();

#ifndef INTS
	insert(&table, "a", "qwerty123");
	insert(&table, "a", "qwerty012");
	insert(&table, "a", "qwerty456");
	insert(&table, "g", "qwerty789");
	printf("Data=%d\n", search(table, "a", "qwerty123"));
#else
	insert(&table, "a", 123);
	insert(&table, "a", 4567);
	insert(&table, "a", 245362);
	insert(&table, "g", 212);
	print(table);
	printf("Data=%d\n", search(table, "a", 123));
	delete(table, "a", 123);
	print(table);
#endif // INTS

	hash_table_dtor(table);

}
