#ifndef HASH_TABLE_H_
#define HASH_TABLE_H_

typedef struct hash_table_entry hash_table_entry_t;
typedef struct hash_table hash_table_t;

typedef struct hash_table_entry
{
	char *key;
	void *data;
	char is_deleted:1;
}
#if defined(MEMORY_EFFICIENT)
__attribute__((packed, aligned(1)))
#endif
	hash_table_entry_t;

typedef struct hash_table
{
	unsigned size,
			 count;
	hash_table_entry_t **entries;
} hash_table_t;

hash_table_entry_t *hash_table_entry_ctor(const char *key,
                                          const void* data);
hash_table_entry_t *hash_table_entry_default_ctor();
hash_table_t *hash_table_ctor();
void hash_table_dtor(hash_table_t *table);

#endif // HASH_TABLE_H_
