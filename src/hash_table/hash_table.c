#include "hash_table.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#define NUM_CLASSES 11

// We use the hash table to store the reserved words
static char classes[NUM_CLASSES][MAX_STR_LENGTH] = {
    "CONST", "VAR", "PROCEDURE", "CALL", "BEGIN", "END",
    "IF", "THEN", "WHILE", "DO", "ODD"};

/**
 * @brief initlalizes node with specified string
 * @param str String
 * @return pointer to node
 */
Node *node_init(const char *str) {
    Node *node = (Node *)malloc(sizeof(Node));
    strcpy(node->keyword, str);
    node->next = NULL;
    return node;
}

/**
 * @brief insert string into node
 * @param str String
 */
void node_insert(Node *node, const char *str) {
    Node *tmp = node;
    if (!strcmp(tmp->keyword, PLACEHOLDER)) {
        strcpy(tmp->keyword, str);
    } else {
        while (tmp->next) {
            tmp = tmp->next;
        }
        tmp->next = node_init(str);
    }
}

/**
 * @brief free allocated node list memory
 * @param node Pointer to node
 */
void node_free(Node *node) {
    Node *current = node;
    Node *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

/**
 * @brief djb2 algorithm for hashing strings by Dan Bernstein
 * @param str String to be hashed
 * @return hash index
 */
unsigned long hash_djb2(const char *str) {
    char *lower_str = str_to_lower(str);
    unsigned long hash = 5381;
    int c;

    while ((c = *lower_str++))
        hash = ((hash << 5) + hash) + c;

    free(lower_str - strlen(str) - 1);  // Free the allocated memory for lower_str
    return hash % TABLE_SIZE;
}

/**
 * @brief convert string to lowercase
 * @param str String to be converted
 * @return pointer to the lowercase string
 */
char *str_to_lower(const char *str) {
    char *lower_str = (char *)malloc(strlen(str) + 1);
    for (int i = 0; str[i]; i++) {
        lower_str[i] = tolower(str[i]);
    }
    lower_str[strlen(str)] = '\0';
    return lower_str;
}

/**
 * @brief return table with initialized values
 * @return pointer to table
 */
KWTable *kwtable_init(void) {
    KWTable *table = (KWTable *)malloc(sizeof(KWTable));

    for (int i = 0; i < TABLE_SIZE; i++) {
        table->table[i] = node_init(PLACEHOLDER);
    }

    unsigned int index;
    for (int i = 0; i < NUM_CLASSES; i++) {
        index = hash_djb2(classes[i]);
        node_insert(table->table[index], classes[i]);
    }

    return table;
}

/**
 * @brief returns positive value if string is in the table and 0 otherwise
 * @return int value
 */
bool kwtable_query(KWTable *table, const char *str) {
    char *lower_str = str_to_lower(str);
    int index = hash_djb2(lower_str);
    Node *list = table->table[index];
    bool ret_val = 0;

    while (list != NULL) {
        ret_val = !strcmp(str_to_lower(list->keyword), lower_str);
        if (ret_val)
            break;
        list = list->next;
    }

    free(lower_str);  // Free the allocated memory for lower_str
    return ret_val;
}

/**
 * @brief free table's allocated memory
 * @param table Pointer to table
 */
void kwtable_free(KWTable *table) {
    for (int i = 0; i < TABLE_SIZE; i++)
        node_free(table->table[i]);
    free(table);
}