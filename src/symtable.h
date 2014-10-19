typedef struct _list_t_ list_t;

typedef struct _hash_table_t_ hash_table_t;

hash_table_t *create_hash_table(int size);
unsigned oat_hash(hash_table_t *hashtable, void *key, int len);
list_t *lookup_string(hash_table_t *hashtable, char *str);
int add_id(hash_table_t *hashtable, char *id, char *type, char *value);
void free_table(hash_table_t *hashtable);
