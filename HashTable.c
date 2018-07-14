#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// var == NULL; EQ to: !var

#define NT_LEN 1
#define HT_DELTA 2
#define STR_MAX 2048
#define DEFAULT_SIZE 10
#define PERCENT_CUTOFF 80

typedef struct node_s {
	char *key, *value;
	struct node_s *next;
} node_t;

typedef node_t *Node;

typedef struct s_ll_s {
	Node root;
} s_ll_t;

typedef s_ll_t *S_Ll;

static Node create_node(const char *const restrict key, const char *const restrict value) {
	const Node const restrict node = (Node) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	const size_t key_len = strnlen(key, STR_MAX);

	node->key = (char*) calloc((key_len + NT_LEN), sizeof(char));
	if (!node->key)
		exit(EXIT_FAILURE);

	strncpy(node->key, key, key_len);

	const size_t value_len = strnlen(value, STR_MAX);

	node->value = (char*) calloc((value_len + NT_LEN), sizeof(char));
	if (!node->value)
		exit(EXIT_FAILURE);

	strncpy(node->value, value, value_len);

	node->next = NULL;

	return node;
}

static Node find_prev_node(Node const list, const char *const restrict key) {
	const size_t key_len = strnlen(key, STR_MAX);
	Node restrict prev = NULL;
	Node cur = list;

	while (cur) {
		if (strncmp(key, cur->key, key_len) == 0)
			return prev;
		prev = cur;
		cur = cur->next;
	}

	return NULL;
}

static Node s_ll_find(const S_Ll const restrict list, const char *const restrict key) {
	const size_t key_len = strnlen(key, STR_MAX);
	Node cur = list->root;

	while (cur) {
		if (strncmp(key, cur->key, key_len) == 0)
			return cur;
		cur = cur->next;
	}

	return NULL;
}

static void s_ll_insert(const S_Ll const restrict list, const char *const restrict key, const char *const restrict value) {
	const Node const new_node = create_node(key, value);
	Node node = list->root;

	if (!node) {
		list->root = new_node;
		return;
	}

	while (node->next)
		node = node->next;

	node->next = new_node;
}

// THIS IS A VALID FUNCTION, ONLY COMMENTED OUT DUE TO COMPLIER ERROR BEING UNUSED IN THIS INSTANCE

// static void s_ll_insert_sorted(const S_Ll const restrict list, const char *const restrict key,
//                                const char *const restrict value) {
// 	const Node const new_node = create_node(key, value);

// 	if (!list->root) {
// 		list->root = new_node;
// 		return;
// 	}

// 	const size_t key_len = strnlen(key, STR_MAX);

// 	if (strncmp(key, list->root->key, key_len) < 0) {
// 		new_node->next = list->root;
// 		list->root = new_node;
// 		return;
// 	}

// 	Node restrict prev = NULL;
// 	Node cur = list->root;

// 	while (cur) {
// 		if (strncmp(key, cur->key, key_len) < 0) {
// 			new_node->next = cur;
// 			prev->next = new_node;
// 			return;
// 		}
// 		prev = cur;
// 		cur = cur->next;
// 	}

// 	prev->next = new_node;
// }

static int s_ll_remove(const S_Ll const restrict list, const char *const restrict key) {
	if (!s_ll_find(list, key))
		return -1;

	const Node const root = list->root, prev = find_prev_node(root, key);
	Node restrict delete_node;

	if (!prev) {
		delete_node = root;
		list->root = root->next;
	} else if (!prev->next)
		delete_node = prev;
	else {
		delete_node = prev->next;
		prev->next = delete_node->next;
	}

	free(delete_node->key);
	delete_node->key = NULL;

	free(delete_node->value);
	delete_node->value = NULL;

	free(delete_node);
	delete_node = NULL;

	return 0;
}

static void s_ll_destroy(S_Ll restrict list) {
	Node tmp, root = list->root;

	while (root) {
		tmp = root;
		root = root->next;

		free(tmp->key);
		tmp->key = NULL;

		free(tmp->value);
		tmp->value = NULL;

		free(tmp);
		tmp = NULL;
	}
	free(list);
	list = NULL;
}

static void s_ll_print(const S_Ll const restrict list) {
	for (Node node = list->root; node; node = node->next)
		printf("%s:%s\n", node->key, node->value);
}

static S_Ll s_ll_create(void) {
	const S_Ll const restrict list = (S_Ll) malloc(sizeof(S_Ll));
	list->root = NULL;

	return list;
}

// HASHTABLE IMPLEMENTATION

typedef struct hashtable_s {
	S_Ll *bins;
	unsigned int max_size, cur_size;
} hashtable_t;

typedef hashtable_t *HashTable;

// D. J. Bernstein Hash, Modified
static unsigned int get_hash(const HashTable const ht, const char *value) {
	unsigned int result = 5381;

	while (*value)
		result = (33 * result) ^ (unsigned char) *value++;

	return result % ht->max_size;
}

static void deep_copy(const HashTable const new_table, const HashTable const ht) {
	unsigned int bin;

	for (unsigned int i = 0; i < ht->max_size; i++) {
		if (!ht->bins[i])
			continue;
		for (Node node = ht->bins[i]->root; node; node = node->next) {
			bin = get_hash(new_table, node->key);

			s_ll_insert(new_table->bins[bin], node->key, node->value);
			new_table->cur_size++;
		}
	}
}

void ht_remove(const HashTable const ht, const char *const key) {
	const unsigned int bin = get_hash(ht, key);

	if (s_ll_remove(ht->bins[bin], key) == 0)
		ht->cur_size--;
}

char *ht_get_value(const HashTable const ht, const char *const key) {
	const unsigned int bin = get_hash(ht, key);

	for (Node node = ht->bins[bin]->root; node; node = node->next)
		if (strncmp(key, node->key, strnlen(key, STR_MAX)) == 0)
			return node->value;

	return NULL;
}

void ht_print(const HashTable const ht) {
	for (unsigned int bin = 0; bin < ht->max_size; bin++)
		s_ll_print(ht->bins[bin]);
}

void ht_destroy(HashTable ht) {
	for (unsigned int bin = 0; bin < ht->max_size; bin++)
		s_ll_destroy(ht->bins[bin]);

	free(ht->bins);
	ht->bins = NULL;

	free(ht);
	ht = NULL;
}

HashTable ht_create(const unsigned int max_size) {
	if (max_size < 1)
		return NULL;

	const HashTable const ht = (HashTable) malloc(sizeof(hashtable_t));
	if (!ht)
		exit(EXIT_FAILURE);

	ht->bins = (S_Ll*) malloc(sizeof(S_Ll) * max_size);
	if (!ht->bins)
		exit(EXIT_FAILURE);

	for (unsigned int i = 0; i < max_size; i++)
		ht->bins[i] = s_ll_create();

	ht->max_size = max_size;
	ht->cur_size = 0;

	return ht;
}

void ht_insert(HashTable *ht_head, const char *const key, const char *const value) {
	const HashTable const ht = *ht_head;
	const unsigned int bin = get_hash(ht, key);

	s_ll_insert(ht->bins[bin], key, value);
	ht->cur_size++;

	if (ht->cur_size >= ((ht->max_size * PERCENT_CUTOFF) / 100)) {
		HashTable new_table = ht_create(ht->max_size * HT_DELTA);

		deep_copy(new_table, ht);
		ht_destroy(ht);
		*ht_head = new_table;
	}
}

int main(void) {
	HashTable ht1 = ht_create(DEFAULT_SIZE);

	ht_remove(ht1, "One");

	ht_insert(&ht1, "One", "1");

	ht_insert(&ht1, "Two", "2");

	ht_insert(&ht1, "Three", "3");

	ht_insert(&ht1, "Four", "4");

	ht_insert(&ht1, "Five", "5");

	ht_insert(&ht1, "Nine", "9");

	ht_remove(ht1, "One");

	ht_remove(ht1, "Nine");

	ht_remove(ht1, "Three");

	ht_remove(ht1, "Two");

	ht_remove(ht1, "Six");

	ht_print(ht1);

	ht_destroy(ht1);

	printf("===============================================================\n");

	HashTable ht2 = ht_create(DEFAULT_SIZE);

	ht_insert(&ht2, "One", "1");

	ht_insert(&ht2, "Two", "2");

	ht_insert(&ht2, "Three", "3");

	ht_insert(&ht2, "Four", "4");

	ht_insert(&ht2, "Five", "5");

	ht_insert(&ht2, "Six", "6");

	ht_insert(&ht2, "Seven", "7");

	ht_insert(&ht2, "Eight", "8");

	ht_insert(&ht2, "Nine", "9");

	ht_insert(&ht2, "Ten", "10");

	ht_insert(&ht2, "Eleven", "11");

	ht_print(ht2);

	printf("This is ht2 cur_size: %d\n", ht2->cur_size);
	printf("This is ht2 max_size: %d\n", ht2->max_size);
	printf("This is get value function, get Eight: %s\n", ht_get_value(ht2, "Eight"));

	ht_destroy(ht2);

	return EXIT_SUCCESS;
}
