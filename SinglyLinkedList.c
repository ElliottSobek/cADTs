#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var == NULL; EQ to: !var

#define NT_LEN 1

typedef struct node {
	char *datum;
	struct node *next;
} node_t;

node_t *create_node(const char *const entry) {
	const size_t entry_len = strlen(entry);

	node_t *const node = malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	node->datum = calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->next = NULL;

	return node;
}

node_t *find_prev_node(node_t *const list, const char *const entry) {
	const size_t entry_len = strlen(entry);
	node_t *cur = list, *prev = NULL;

	for (node_t *node = list; node->next; node = node->next) {
		if (strncmp(entry, node->datum, entry_len) == 0)
			return prev;
		prev = cur;
		cur = node->next;
	}

	return prev;
}

void insert_node(node_t *list, const char *const entry) {
	node_t *node = list, *new_node = create_node(entry);

	if (!node) {
		list = new_node;
		return;
	}

	while (node->next)
		node = node->next;

	node->next = new_node;
}

void insert_node_sorted(node_t **const list, const char *const entry) {
	node_t *cur = *list, *prev = NULL, *new_node = create_node(entry);
	const size_t datum_len = strlen(new_node->datum);

	if (!cur) {
		*list = new_node;
		return;
	}

	if (strncmp(new_node->datum, cur->datum, datum_len) < 0) {
		new_node->next = cur;
		*list = new_node;
		return;
	}

	for (node_t *node = *list; node; node = node->next) {
		if (strncmp(new_node->datum, node->datum, datum_len) < 0) {
			new_node->next = cur;
			prev->next = new_node;
			return;
		}
		prev = cur;
		cur = cur->next;
	}

	prev->next = new_node;
}

void delete_node(node_t **const list, const char *const entry) {
	node_t *delete_node, *prev = find_prev_node(*list, entry);

	if (!prev) {
		delete_node = *list;
		*list = (*list)->next;
	} else if (!prev->next)
		delete_node = prev;
	else {
		delete_node = prev->next;
		prev->next = delete_node->next;
	}

	free(delete_node->datum);
	free(delete_node);
}

void destroy_list(node_t *list) {
	node_t *tmp;

	while (list) {
		tmp = list;
		list = list->next;

		free(tmp->datum);
		free(tmp);
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

	insert_node_sorted(&list, "zzz"); // Insert sorted tail

	delete_node(&list, "Sally"); // Delete middle

	delete_node(&list, "zzz"); // Delete tail

	delete_node(&list, "aaa"); // Delete head

	print_list(list);

	destroy_list(list);

	return 0;
}
