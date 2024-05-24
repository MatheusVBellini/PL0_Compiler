#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdio.h>
#include <stdbool.h>

#define TABLE_SIZE 100
#define MAX_STR_LENGTH 20
#define PLACEHOLDER "NOT_A_KW"

typedef struct node {
  char keyword[MAX_STR_LENGTH];
  struct node *next;
} Node;

typedef struct {
  Node *table[TABLE_SIZE];
} KWTable;

/**
 * @brief initlalizes node with specified string
 * @param str String
 * @return pointer to node
 */
Node *node_init(const char *str);

/**
 * @brief insert string into node
 * @param node Current node list
 * @param str String
 */
void node_insert(Node *node, const char *str);

/**
 * @brief free allocated node list memory
 * @param node Pointer to node
 */
void node_free(Node *node);

/**
 * @brief djb2 algorithm for hashing strings by Dan Bernstein
 * @param str String to be hashed
 * @return hash index
 */
unsigned long hash_djb2(const char* str);

/**
 * @brief return table with initialized values
 * @return pointer to table
 */
KWTable* kwtable_init(void);

/**
 * @brief returns positive value if string is in the table and 0 otherwise
 * @param table Table to query into
 * @param str String to search in the table
 * @return bool value
 */
bool kwtable_query(KWTable *table, const char *str);

/**
 * @brief free table's allocated memory
 * @param table Pointer to table
 */
void kwtable_free(KWTable *table);

/**
 * @brief convert string to lowercase
 * @param str String to be converted
 * @return pointer to the lowercase string
 */
char *str_to_lower(const char *str);

#endif
