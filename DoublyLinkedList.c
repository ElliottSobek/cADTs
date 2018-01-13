#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var == NULL; EQ to: !var

#define NT_LEN 1
#define STR_MAX 2048

typedef struct node {
	char *datum;
	struct node *prev, *next;
} node_t;

node_t *create_node(const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);

	node_t *const node = (node_t*) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	node->datum = (char*) calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->prev = NULL;
	node->next = NULL;

	return node;
}

node_t *find_node(node_t *const list, const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);

	for (node_t *node = list; node; node = node->next)
		if (strncmp(entry, node->datum, entry_len) == 0)
			return node;

	return NULL;
}

void insert_node(node_t *list, const char *const entry) {
	node_t *node = list, *new_node = create_node(entry);

	if (!node) {
		list = new_node;
		return;
	}

	while (node->next)
		node = node->next;

	new_node->prev = node;
	node->next = new_node;
}

void insert_node_sorted(node_t **const list, const char *const entry) {
	node_t *cur = *list, *new_node = create_node(entry);
	const size_t datum_len = strnlen(new_node->datum, STR_MAX);

	if (!cur) {
		*list = new_node;
		return;
	}

	if (strncmp(new_node->datum, cur->datum, datum_len) < 0) {
		new_node->next = cur;
		cur->prev = new_node;
		*list = new_node;
		return;
	}

	for (node_t *node = *list; node; node = node->next) {
		if (strncmp(new_node->datum, node->datum, datum_len) < 0) {
			new_node->prev = cur;
			new_node->next = cur->next;
			cur->next->prev = new_node;
			cur->next = new_node;
			return;
		}
		cur = node;
	}

	new_node->prev = cur;
	cur->next = new_node;
}

void delete_node(node_t **const list, const char *const entry) {
	node_t *const node = find_node(*list, entry), *delete_node = node;

	if (!node->prev)
		*list = node->next;

	else if (!node->next)
		node->prev->next = NULL;

	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	free(delete_node->datum);
	delete_node->datum = NULL;
	free(delete_node);
	delete_node = NULL;
}

void destroy_list(node_t *list) {
	node_t *tmp;

	while (list) {
		tmp = list;
		list = list->next;

		free(tmp->datum);
		tmp->datum = NULL;
		free(tmp);
		tmp = NULL;
	}
}

void print_list(node_t *const list) {

	for (node_t *node = list; node; node = node->next)
		printf("%s\n", node->datum);
}

/*  ASSERT:
	Alice
	Jason
	Jzzz
	Zebra
	Bob
	Johnson
*/
int main(void) {
	node_t *list = create_node("Alice"); // Create

	insert_node(list, "Sally"); // Insert middle

	insert_node(list, "Zebra"); // Insert middle

	insert_node(list, "Bob"); // Insert middle

	insert_node(list, "Johnson"); // Insert middle

	insert_node_sorted(&list, "Jason"); // Insert sorted middle

	insert_node_sorted(&list, "Jzzz"); // Insert sorted middle

	insert_node_sorted(&list, "AAA"); // Insert sorted head

	insert_node_sorted(&list, "Zzzz"); // Insert sorted tail

	delete_node(&list, "Sally"); // Delete middle

	delete_node(&list, "Zzzz"); // Delete tail

	delete_node(&list, "AAA"); // Delete head

	print_list(list);

	destroy_list(list);

	return EXIT_SUCCESS;
}
