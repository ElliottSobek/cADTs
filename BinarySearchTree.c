#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// var == NULL; EQ to: !var

#define NT_LEN 1

typedef struct node {
	char *datum;
	struct node *left, *right;
} node_t;

node_t *create_node(const char *const entry) {
	const size_t entry_len = strlen(entry);

	node_t *const node = (node_t*) malloc(sizeof(node_t));
	if (!node)
		exit(EXIT_FAILURE);

	node->datum = (char*) calloc((entry_len + NT_LEN), sizeof(char));
	if (!node->datum)
		exit(EXIT_FAILURE);

	strncpy(node->datum, entry, entry_len);

	node->left = NULL;
	node->right = NULL;

	return node;
}

void insert_node(node_t **const root, const char *const entry) {
	node_t *const new_node = create_node(entry);

	if (!*root) {
		*root = new_node;
		return;
	}
	int cmp_res;
	node_t *cur = *root, *parent;

	while (cur) {
		cmp_res = strncmp(new_node->datum, cur->datum, strlen(new_node->datum));
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

node_t *minValueNode(node_t *const node) {
    node_t *cur = node;

    while (cur->left)
        cur = cur->left;

    return cur;
}

node_t *delete_node(node_t *root, const char *const key) {
	const size_t key_len = strlen(key);
	int cmp_res = strncmp(key, root->datum, key_len);

	if (!root)
		return NULL;
	else if (cmp_res < 0)
		root->left = delete_node(root->left, key);
	else if (cmp_res > 0)
		root->right = delete_node(root->right, key);
	else {
		node_t *del_node = root;

		if (!root->left && !root->right) {
			root = NULL;
			free(del_node->datum);
			del_node->datum = NULL;
			free(del_node);
			del_node = NULL;
		} else if (!root->left) {
			root = root->right;
			free(del_node->datum);
			del_node->datum = NULL;
			free(del_node);
			del_node = NULL;
		} else if (!root->right) {
			root = root->left;
			free(del_node->datum);
			del_node->datum = NULL;
			free(del_node);
			del_node = NULL;
		} else {
			const size_t del_key_len = strlen(del_node->datum);
			del_node = minValueNode(root->right);
			root->datum = realloc(root->datum, sizeof(char) * (del_key_len + NT_LEN));

			strncpy(root->datum, del_node->datum, del_key_len);
			root->right = delete_node(root->right, del_node->datum);
		}
	}
	return root;
}

void destroy_tree(node_t *root) {
	if (!root)
		return;
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root->datum);
	root->datum = NULL;
	free(root);
	root = NULL;
}

void print_tree_in_order(node_t *const root) {
	if (!root)
		return;
	print_tree_in_order(root->left);
	printf("%s\n", root->datum);
	print_tree_in_order(root->right);
}

void print_tree_pre_order(node_t *const root) {
	if (!root)
		return;
	printf("%s\n", root->datum);
	print_tree_pre_order(root->left);
	print_tree_pre_order(root->right);
}

int main(void) {
	node_t *root = create_node("g"); // Create, root

	insert_node(&root, "d"); // Left node

	insert_node(&root, "u"); // Right node

	insert_node(&root, "a"); // Left node, leaf

	insert_node(&root, "e"); // Right node, leaf

	insert_node(&root, "j"); // Left node

	insert_node(&root, "z"); // Right node, leaf

	insert_node(&root, "h"); // Left node, leaf

	insert_node(&root, "s"); // Right node

	insert_node(&root, "m"); // Left node

	insert_node(&root, "t"); // Right node, leaf

	insert_node(&root, "q"); // Right node, leaf

	print_tree_in_order(root);

	printf("\n");

	delete_node(root, "t"); // Delete  leaf

	delete_node(root, "m"); // Delete with one node

	delete_node(root, "j"); // Delete two nodes

	print_tree_in_order(root);

	destroy_tree(root);

	return 0;
}
