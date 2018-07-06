#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct hashtable_s {
	Node *data;
	unsigned int max_size, cur_size;
} hashtable_t;

typedef hashtable_t *HashTable;

void insert_set(HashTable*, const char *const, const char *const);

static Node create_node(const char *const key, const char *const value) {
	const Node const node = (Node) malloc(sizeof(node_t));
	if (!node)
		return NULL;

	const size_t key_len = strnlen(key, STR_MAX);

	node->key = (char*) calloc(key_len + NT_LEN, sizeof(char));
	if (!node->key)
		return NULL;
	strncpy(node->key, key, key_len);

	const size_t value_len = strnlen(value, STR_MAX);

	node->value = (char*) calloc(value_len + NT_LEN, sizeof(char));
	if (!node->value)
		return NULL;
	strncpy(node->value, value, value_len);

	node->next = NULL;

	return node;
}

// D. J. Bernstein Hash, Modified
static unsigned int get_hash(const HashTable const ht, const char *value) {
	unsigned int result = 5381;

	while (*value)
		result = (33 * result) ^ (unsigned char) *value++;

	return result % ht->max_size;
}

static void destroy_list(Node list) {
	Node tmp;

	while (list) {
		tmp = list;
		list = list->next;

		free(tmp->key);
		tmp->key = NULL;
		free(tmp->value);
		tmp->value = NULL;
		free(tmp);
		tmp = NULL;
	}
}

static Node find_prev_node(const Node const list, const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);
	Node cur = list, prev = NULL;

	for (Node node = list; node->next; node = node->next) {
		if (strncmp(entry, node->key, entry_len) == 0)
			return prev;
		prev = cur;
		cur = node->next;
	}

	return prev;
}

static void remove_collision_node(Node *const list, Node const prev) {
	Node delete_node;

	if (!prev) {
		delete_node = *list;
		*list = (*list)->next;
	} else if (!prev->next)
		delete_node = prev;
	else {
		delete_node = prev->next;
		prev->next = prev->next->next;
	}

	free(delete_node->key);
	delete_node->key = NULL;
	free(delete_node->value);
	delete_node->value = NULL;
	free(delete_node);
	delete_node = NULL;
}

static void add_collision_node(const Node const list, const Node const new_node) {
	Node node = list;

	while (node->next)
		node = node->next;

	node->next = new_node;
}

static void deep_copy(HashTable new_table, const HashTable const ht) {

	for (unsigned int i = 0; i < ht->max_size; i++) {
		if (!ht->data[i])
			continue;
		for (Node node = ht->data[i]; node; node = node->next)
			insert_set(&new_table, node->key, node->value);
	}
}

static void print_list(const Node const list) {
	for (Node node = list->next; node; node = node->next)
		printf("->%s:%s", node->key, node->value);
	printf("\n");
}

void remove_set(const HashTable const ht, const char *const key) {
	const unsigned int bin = get_hash(ht, key);

	if (ht->data[bin]->next)
		remove_collision_node(&ht->data[bin], find_prev_node(ht->data[bin], key));
	else {
		free(ht->data[bin]->key);
		ht->data[bin]->key = NULL;
		free(ht->data[bin]->value);
		ht->data[bin]->value = NULL;
		free(ht->data[bin]);
		ht->data[bin] = NULL;
	}
	ht->cur_size--;
}

char *get_value(const HashTable const ht, const char *const key) {
	const unsigned int bin = get_hash(ht, key);
	int cmp_res;

	for (Node node = ht->data[bin]; node; node = node->next) {
		cmp_res = strncmp(key, ht->data[bin]->key, strnlen(key, STR_MAX));

		if (cmp_res == 0)
			return node->value;
	}

	return NULL;
}

void print_table(const HashTable const ht) {
	if (!ht->data[0])
		printf("nil\n");
	else {
		printf("%s:%s", ht->data[0]->key, ht->data[0]->value);
		print_list(ht->data[0]);
	}

	for (unsigned int i = 1; i < ht->max_size; i++) {

		if (!ht->data[i])
			printf("|\nv\nnil\n");
		else {
			printf("|\nv\n%s:%s", ht->data[i]->key, ht->data[i]->value);
			print_list(ht->data[i]);
		}
	}
}

void ht_destroy(HashTable ht) {
	for (unsigned int i = 0; i < ht->max_size; i++) {
		if (!ht->data[i])
			continue;
		else if (ht->data[i]->next)
			destroy_list(ht->data[i]);
		else {
			free(ht->data[i]->key);
			ht->data[i]->key = NULL;
			free(ht->data[i]->value);
			ht->data[i]->value = NULL;
			free(ht->data[i]);
			ht->data[i] = NULL;
		}
	}
	free(ht->data);
	ht->data = NULL;
	free(ht);
	ht = NULL;
}

HashTable ht_create(const unsigned int max_size) {
	if (max_size < 1)
		return NULL;

	const HashTable const ht = (HashTable) malloc(sizeof(hashtable_t));
	if (!ht)
		exit(EXIT_FAILURE);

	ht->data = (Node*) malloc(sizeof(Node) * max_size);
	if (!ht->data)
		exit(EXIT_FAILURE);

	for (unsigned int i = 0; i < max_size; i++)
		ht->data[i] = NULL;

	ht->max_size = max_size;
	ht->cur_size = 0;

	return ht;
}

void insert_set(HashTable *ht_head, const char *const key, const char *const value) {
	const HashTable const ht = *ht_head;

	if (ht->cur_size >= ((ht->max_size * PERCENT_CUTOFF) / 100)) {
		HashTable new_table = ht_create(ht->max_size * HT_DELTA);

		deep_copy(new_table, ht);
		insert_set(&new_table, key, value);
		ht_destroy(ht);
		*ht_head = new_table;
		return;
	}

	Node const node = create_node(key, value);
	const unsigned int bin = get_hash(ht, key);

	if (!ht->data[bin])
		ht->data[bin] = node;
	else
		add_collision_node(ht->data[bin], node);

	ht->cur_size++;
}

int main(void) {
	HashTable ht1 = ht_create(DEFAULT_SIZE);

	insert_set(&ht1, "One", "1");

	insert_set(&ht1, "Two", "2");

	insert_set(&ht1, "Three", "3");

	insert_set(&ht1, "Four", "4");

	insert_set(&ht1, "Five", "5");

	insert_set(&ht1, "Nine", "9");

	remove_set(ht1, "One");

	remove_set(ht1, "Nine");

	remove_set(ht1, "Three");

	remove_set(ht1, "Two");

	print_table(ht1);

	ht_destroy(ht1);

	printf("===============================================================\n");

	HashTable ht2 = ht_create(DEFAULT_SIZE);

	insert_set(&ht2, "One", "1");

	insert_set(&ht2, "Two", "2");

	insert_set(&ht2, "Three", "3");

	insert_set(&ht2, "Four", "4");

	insert_set(&ht2, "Five", "5");

	insert_set(&ht2, "Six", "6");

	insert_set(&ht2, "Seven", "7");

	insert_set(&ht2, "Eight", "8");

	insert_set(&ht2, "Nine", "9");

	insert_set(&ht2, "Ten", "10");

	print_table(ht2);

	printf("This is get value function, get Eight: %s\n", get_value(ht2, "Eight"));

	ht_destroy(ht2);

	return EXIT_SUCCESS;
}
