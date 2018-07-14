#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var == NULL; EQ to: !var

#define NT_LEN 1
#define STR_MAX 2048

typedef struct node_s {
	char *datum;
	struct node_s *left, *right;
} node_t;

typedef node_t *Node;

typedef struct bst_s {
	Node root;
} bst_t;

typedef bst_t *Bst;

static Node create_node(const char *const restrict entry) {
	const Node const restrict node = (Node) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	const size_t entry_len = strnlen(entry, STR_MAX);

	node->datum = (char*) calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->left = NULL;
	node->right = NULL;

	return node;
}

static Node min_value_node(const Node const node) {
    Node cur = node;

    while (cur->left)
        cur = cur->left;

    return cur;
}

static void destroy_tree(Node restrict root) {
	if (!root)
		return;
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root->datum);
	root->datum = NULL;

	free(root);
	root = NULL;
}

static void print_tree_in_order(const Node const restrict root) {
	if (!root)
		return;
	print_tree_in_order(root->left);
	printf("%s\n", root->datum);
	print_tree_in_order(root->right);
}

static void print_tree_pre_order(const Node const restrict root) {
	if (!root)
		return;
	printf("%s\n", root->datum);
	print_tree_pre_order(root->left);
	print_tree_pre_order(root->right);
}

void bst_insert(const Bst const restrict bst, const char *const restrict entry) {
	const Node const new_node = create_node(entry);

	if (!bst->root) {
		bst->root = new_node;
		return;
	}
	int cmp_res;
	Node restrict parent;
	Node cur = bst->root;

	while (cur) {
		cmp_res = strncmp(entry, cur->datum, strnlen(entry, STR_MAX));
		parent = cur;

		if (cmp_res < 0)
			cur = cur->left;
		else
			cur = cur->right;
	}

	if (cmp_res < 0)
		parent->left = new_node;
	else
		parent->right = new_node;
}

void bst_print_v1(const Bst const restrict bst) {
	print_tree_in_order(bst->root);
}

void bst_print_v2(const Bst const restrict bst) {
	print_tree_pre_order(bst->root);
}

Node remove_node(Node root, const char *const restrict entry) {
	int cmp_res = strncmp(entry, root->datum, strnlen(entry, STR_MAX));

	if (!root)
		return NULL;
	else if (cmp_res < 0)
		root->left = remove_node(root->left, entry);
	else if (cmp_res > 0)
		root->right = remove_node(root->right, entry);
	else {
		Node restrict del_node = root;

		if (!root->left && !root->right) { // Remove Leaf Node
			root = NULL;

			free(del_node->datum);
			del_node->datum = NULL;

			free(del_node);
			del_node = NULL;
		} else if (!root->left) { // Remove With One Left Child
			root = root->right;

			free(del_node->datum);
			del_node->datum = NULL;

			free(del_node);
			del_node = NULL;
		} else if (!root->right) { // Remove With One Right Child
			root = root->left;

			free(del_node->datum);
			del_node->datum = NULL;

			free(del_node);
			del_node = NULL;
		} else { // Remove With Two Children
			const size_t root_key_len = strnlen(root->datum, STR_MAX);
			del_node = min_value_node(root->right);
			root->datum = (char*) realloc(root->datum, sizeof(char) * (root_key_len + NT_LEN));

			strncpy(root->datum, del_node->datum, root_key_len);
			root->right = remove_node(root->right, del_node->datum);
		}
	}
	return root;
}

char *bst_find(const Bst const restrict bst, const char *const restrict entry) {
	if (!bst->root) {
		return "";
	}
	int cmp_res;
	Node cur = bst->root;

	while (cur) {
		cmp_res = strncmp(entry, cur->datum, strnlen(entry, STR_MAX));

		if (cmp_res == 0)
			return cur->datum;
		else if (cmp_res < 0)
			cur = cur->left;
		else
			cur = cur->right;
	}

	return "";
}

void bst_remove(const Bst const restrict bst, const char *const restrict entry) {
	remove_node(bst->root, entry);
}

void bst_destroy(Bst restrict bst) {
	destroy_tree(bst->root);
	free(bst);
	bst = NULL;
}

Bst bst_create(void) {
	Bst restrict bst = (Bst) malloc(sizeof(bst_t));

	if (!bst)
		exit(EXIT_FAILURE);
	bst->root = NULL;

	return bst;
}

int main(void) {
	Bst restrict bst = bst_create();

	bst_insert(bst, "g");

	bst_insert(bst, "d"); // Left node

	bst_insert(bst, "u"); // Right node

	bst_insert(bst, "a"); // Left node, leaf

	bst_insert(bst, "e"); // Right node, leaf

	bst_insert(bst, "j"); // Left node

	bst_insert(bst, "z"); // Right node, leaf

	bst_insert(bst, "h"); // Left node, leaf

	bst_insert(bst, "s"); // Right node

	bst_insert(bst, "m"); // Left node

	bst_insert(bst, "t"); // Right node, leaf

	bst_insert(bst, "q"); // Right node, leaf

	bst_print_v1(bst);

	printf("\n");

	bst_print_v2(bst);

	printf("\n");

	printf("This is bst_find and what it found: %s\n", bst_find(bst, "h"));

	printf("\n");

	bst_remove(bst, "t"); // Delete  leaf

	bst_remove(bst, "m"); // Delete with one node

	bst_remove(bst, "j"); // Delete two nodes

	bst_print_v2(bst);

	bst_destroy(bst);

	return EXIT_SUCCESS;
}
