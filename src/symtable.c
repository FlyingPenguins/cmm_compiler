#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "symtable.h"

#define MULTIPLIER (37)
#define OAT_HASH_LEN (20)

struct _list_t_ {
    char *id;
    char *type;
    char *value;
    struct _list_t_ *next;
};

struct _hash_table_t_ {
    int size;
    list_t **table;
};

hash_table_t
*create_hash_table(int size)
{
    hash_table_t *new_table;

    if (size < 1) return NULL;

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(hash_table_t))) == NULL) {
        return NULL;
    }

    /* Attempt to allocate memory for the table itself */
    if ((new_table->table = malloc(sizeof(list_t *) *size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    for(int i = 0; i < size; i++) new_table->table[i] = NULL;

    /* Set the table's size */
    new_table->size = size;

    return new_table;
}

/* Jenkins' one-at-a-time hash */
unsigned
oat_hash(hash_table_t *hashtable, void *key, int len)
{

        unsigned char *p = key;
        unsigned h = 0;
        int i;

        for (i = 0; i < len; i++) {
                h += p[i];
                h += ( h << 10 );
                h ^= ( h >> 6 );
        }

        h += ( h << 3 );
        h ^= (h >> 11);
        h += (h << 15);

        return h % hashtable->size;
}

list_t
*lookup(hash_table_t *hashtable, char *str)
{
        list_t *list;
        unsigned int h = oat_hash(hashtable, str, OAT_HASH_LEN);

        /* Go to the correct list based on the hash value and see if str is in
         * the list. If it is, return a pointer to the list element.
         * If it isn't, the item isn't in the table, so return NULL.
         */
        for(list = hashtable->table[h]; list != NULL; list = list->next) {
                if (strcmp(str, list->id) == 0) return list;
        }

        return NULL;
}

int
add_id(hash_table_t *hashtable, char *id, char *type, char *value)
{
        list_t *new_list;
        list_t *current_list;
        unsigned int h = oat_hash(hashtable, id, OAT_HASH_LEN);

        /* Attempt to allocate memory for list */
        if ((new_list = malloc(sizeof(list_t))) == NULL) return 1;

        /* Does item already exist? */
        current_list = lookup(hashtable, id);
        /* Item already exists, don't insert it again */
        if (current_list != NULL) return 2;
        /* Insert into list */
        new_list->id = strdup(id);
        new_list->type = strdup(type);
        new_list->value = strdup(value);
        new_list->next = hashtable->table[h];
        hashtable->table[h] = new_list;

        return 0;
}

void
free_table(hash_table_t *hashtable) {
        int i;
        list_t *list, *temp;

        if (hashtable == NULL) return;

        /* Free the memory for every item in the table, including the ids,
         * types, and values themselves.
         */
        for(int i = 0; i < hashtable->size; i++) {
                list = hashtable->table[i];
                while(list != NULL) {
                        temp = list;
                        list = list->next;
                        free(temp->id);
                        free(temp->type);
                        free(temp->value);
                        free(temp);
                }
        }

        /* Free the table itself */
        free(hashtable->table);
        free(hashtable);
}

int main()
{
        hash_table_t *symbol_table;
        int size_of_table = 1000;
        symbol_table = create_hash_table(size_of_table);

        add_id(symbol_table, "x", "int", "25");
        list_t *entry = lookup(symbol_table, "x");
        printf("id: %s, type: %s, value: %f\n", entry->id, entry->type,
                        atof(entry->value));
}
