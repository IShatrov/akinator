#ifndef TREE
#define TREE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const int MAX_STR_LEN = 100;
const int ROOT = 0;

const char TREE_BYPASS = 1;

enum poison
{
    //VAL_POISON = -1,
    CAP_POSION = -1,
    SIZE_POSION = -1,
};

typedef char tree_elmt[MAX_STR_LEN + 1];

typedef struct
{
    tree_elmt val;

    void *l_child;
    void *r_child;
} tree_node;

typedef struct
{
    tree_node *root;

    ssize_t size;
    ssize_t capacity;

    tree_node *data;
} my_tree;

tree_node* tree_ctor(my_tree *dest, ssize_t capacity, const tree_elmt root_val);

tree_node* node_ctor(my_tree *tree, const tree_elmt val);

void tree_dtor(my_tree *tree);

void tree_dump(my_tree *tree);

void print_branch(tree_node *node, FILE *gr_viz);

#endif

