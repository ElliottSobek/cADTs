#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var == NULL; EQ to: !var

#define NT_LEN 1
#define STR_MAX 2048

typedef struct node_s {
	char *datum;
	struct node_s *prev, *next;
} node_t;

typedef node_t *Node;

typedef struct d_ll_s {
	Node root;
} d_ll_t;

typedef d_ll_t *D_Ll;

static Node create_node(const char *const entry) {
	const Node const node = (Node) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	const size_t entry_len = strnlen(entry, STR_MAX);

	node->datum = (char*) calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->prev = NULL;
	node->next = NULL;

	return node;
}

static Node find_node(const Node const list, const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);
	Node cur = list;

	while (cur) {
		if (strncmp(entry, cur->datum, entry_len) == 0)
			return cur;
		cur = cur->next;
	}

	return NULL;
}

void d_ll_insert(const D_Ll const list, const char *const entry) {
	const Node const new_node = create_node(entry);

	if (!list->root) {
		list->root = new_node;
		return;
	}

	Node root = list->root;

	while (root->next)
		root = root->next;

	new_node->prev = root;
	root->next = new_node;
}

void d_ll_insert_sorted(const D_Ll const list, const char *const entry) {
	const Node const new_node = create_node(entry);

	if (!list->root) {
		list->root = new_node;
		return;
	}

	const size_t entry_len = strnlen(entry, STR_MAX);

	if (strncmp(entry, list->root->datum, entry_len) < 0) {
		new_node->next = list->root;
		list->root->prev = new_node;
		list->root = new_node;
		return;
	}

	Node cur = list->root;

	while (cur->next) {
		if (strncmp(entry, cur->datum, entry_len) < 0) {
			new_node->prev = cur->prev;
			new_node->next = cur;
			cur->prev->next = new_node;
			cur->prev = new_node;
			return;
		}
		cur = cur->next;
	}

	new_node->prev = cur;
	cur->next = new_node;
}

void d_ll_remove(const D_Ll const list, const char *const entry) {
	if (!list->root)
		return;

	const Node const node = find_node(list->root, entry);

	if (!node)
		return;

	if (!node->prev)
		list->root = node->next;
	else if (!node->next)
		node->prev->next = NULL;
	else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}

	Node delete_node = node;

	free(delete_node->datum);
	delete_node->datum = NULL;
	free(delete_node);
	delete_node = NULL;
}

void d_ll_destroy(D_Ll list) {
	Node tmp, root = list->root;;

	while (root) {
		tmp = root;
		root = root->next;

		free(tmp->datum);
		tmp->datum = NULL;
		free(tmp);
		tmp = NULL;
	}
	free(list);
	list = NULL;
}

void d_ll_print(const D_Ll const list) {
	for (Node node = list->root; node; node = node->next)
		printf("%s\n", node->datum);
}

D_Ll d_ll_create(void) {
	const D_Ll const list = (D_Ll) malloc(sizeof(D_Ll));
	list->root = NULL;

	return list;
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
	const D_Ll const list = d_ll_create(); // Create

	d_ll_remove(list, "a"); // Remove non existent node

	d_ll_insert(list, "Alice"); // Insert middle

	d_ll_insert(list, "Sally"); // Insert middle

	d_ll_insert(list, "Zebra"); // Insert middle

	d_ll_insert(list, "Bob"); // Insert middle

	d_ll_insert(list, "Johnson"); // Insert middle

	d_ll_insert_sorted(list, "Jason"); // Insert sorted middle

	d_ll_insert_sorted(list, "Jzzz"); // Insert sorted middle

	d_ll_insert_sorted(list, "AAA"); // Insert sorted head

	d_ll_insert_sorted(list, "zzz"); // Insert sorted tail

	d_ll_print(list);

	printf("\n");

	d_ll_remove(list, "Sally"); // Delete middle

	d_ll_remove(list, "zzz"); // Delete tail

	d_ll_remove(list, "AAA"); // Delete head

	d_ll_print(list);

	d_ll_destroy(list);

	return EXIT_SUCCESS;
}
