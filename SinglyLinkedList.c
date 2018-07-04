#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	const size_t entry_len = strnlen(entry, STR_MAX);

	Node const node = (Node) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

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

	for (Node node = list; node->next; node = node->next) {
		if (strncmp(entry, node->datum, entry_len) == 0)
			return prev;
		prev = cur;
		cur = node->next;
	}

	return prev;
}

void s_ll_insert(S_Ll list, const char *const entry) {
	Node node = list->root, new_node = create_node(entry);

	if (!node) {
		list->root = new_node;
		return;
	}

	while (node->next)
		node = node->next;

	node->next = new_node;
}

void s_ll_insert_sorted(S_Ll list, const char *const entry) {
	Node *const root = &list->root;
	Node cur = *root, prev = NULL, new_node = create_node(entry);
	const size_t datum_len = strnlen(new_node->datum, STR_MAX);

	if (!cur) {
		*root = new_node;
		return;
	}

	if (strncmp(new_node->datum, cur->datum, datum_len) < 0) {
		new_node->next = cur;
		*root = new_node;
		return;
	}

	for (Node node = *root; node; node = node->next) {
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

void s_ll_remove(S_Ll list, const char *const entry) {
	Node *const root = &list->root;
	Node delete_node, prev = find_prev_node(*root, entry);

	if (!prev) {
		delete_node = *root;
		*root = (*root)->next;
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

void s_ll_print(S_Ll list) {
	for (Node node = list->root; node; node = node->next)
		printf("%s\n", node->datum);
}

S_Ll s_ll_create(void) {
	S_Ll list = (S_Ll) malloc(sizeof(S_Ll));
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
	S_Ll list = s_ll_create();

	s_ll_insert(list, "Alice");

	s_ll_insert(list, "Sally");

	s_ll_insert(list, "Zebra");

	s_ll_insert(list, "Bob");

	s_ll_insert(list, "Johnson");

	s_ll_insert_sorted(list, "Jason");

	s_ll_insert_sorted(list, "Jzzz");

	s_ll_insert_sorted(list, "AAA");

	s_ll_insert_sorted(list, "zzz");

	s_ll_remove(list, "Sally");

	s_ll_remove(list, "zzz");

	s_ll_remove(list, "aaa");

	s_ll_print(list);

	s_ll_destroy(list);

	return EXIT_SUCCESS;
}
