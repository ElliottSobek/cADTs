#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// var == NULL; EQ to: !var

#define NT_LEN 1
#define STR_MAX 2048

typedef struct node_s {
	char *datum;
	struct node_s *next;
} node_t;

typedef node_t *Node;

typedef struct s_ll_s {
	Node root;
} s_ll_t;

typedef s_ll_t *S_Ll;

static Node create_node(const char *const entry) {
	const Node const node = (Node) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	const size_t entry_len = strnlen(entry, STR_MAX);

	node->datum = (char*) calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->next = NULL;

	return node;
}

static Node find_prev_node(Node const list, const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);
	Node cur = list, prev = NULL;

	while (cur) {
		if (strncmp(entry, cur->datum, entry_len) == 0)
			return prev;
		prev = cur;
		cur = cur->next;
	}

	return NULL;
}

static bool node_exists(const Node const list, const char *const entry) {
	const size_t entry_len = strnlen(entry, STR_MAX);
	Node cur = list;

	while (cur) {
		if (strncmp(entry, cur->datum, entry_len) == 0)
			return true;
		cur = cur->next;
	}

	return false;
}

void s_ll_insert(const S_Ll const list, const char *const entry) {
	const Node const new_node = create_node(entry);
	Node node = list->root;

	if (!node) {
		list->root = new_node;
		return;
	}

	while (node->next)
		node = node->next;

	node->next = new_node;
}

void s_ll_insert_sorted(const S_Ll const list, const char *const entry) {
	const Node const new_node = create_node(entry);

	if (!list->root) {
		list->root = new_node;
		return;
	}

	const size_t entry_len = strnlen(entry, STR_MAX);

	if (strncmp(entry, list->root->datum, entry_len) < 0) {
		new_node->next = list->root;
		list->root = new_node;
		return;
	}

	Node cur = list->root, prev = NULL;

	while (cur) {
		if (strncmp(entry, cur->datum, entry_len) < 0) {
			new_node->next = cur;
			prev->next = new_node;
			return;
		}
		prev = cur;
		cur = cur->next;
	}

	prev->next = new_node;
}

void s_ll_remove(const S_Ll const list, const char *const entry) {
	if (!node_exists(list->root, entry))
		return;

	const Node const root = list->root, prev = find_prev_node(root, entry);
	Node delete_node;

	if (!prev) {
		delete_node = root;
		list->root = root->next;
	} else if (!prev->next)
		delete_node = prev;
	else {
		delete_node = prev->next;
		prev->next = delete_node->next;
	}

	free(delete_node->datum);
	delete_node->datum = NULL;
	free(delete_node);
	delete_node = NULL;
}

void s_ll_destroy(S_Ll list) {
	Node tmp, root = list->root;

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

void s_ll_print(const S_Ll const list) {
	for (Node node = list->root; node; node = node->next)
		printf("%s\n", node->datum);
}

S_Ll s_ll_create(void) {
	const S_Ll const list = (S_Ll) malloc(sizeof(S_Ll));
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
	const S_Ll const list = s_ll_create(); // Create

	s_ll_remove(list, "a"); // Remove non existent node

	s_ll_insert(list, "Alice"); // Insert middle

	s_ll_insert(list, "Sally"); // Insert middle

	s_ll_insert(list, "Zebra"); // Insert middle

	s_ll_insert(list, "Bob"); // Insert middle

	s_ll_insert(list, "Johnson"); // Insert middle

	s_ll_insert_sorted(list, "Jason"); // Insert sorted middle

	s_ll_insert_sorted(list, "Jzzz"); // Insert sorted middle

	s_ll_insert_sorted(list, "AAA"); // Insert sorted head

	s_ll_insert_sorted(list, "zzz"); // Insert sorted tail

	s_ll_print(list);

	printf("\n");

	s_ll_remove(list, "Sally"); // Delete middle

	s_ll_remove(list, "zzz"); // Delete tail

	s_ll_remove(list, "AAA"); // Delete head

	s_ll_print(list);

	s_ll_destroy(list);

	return EXIT_SUCCESS;
}
