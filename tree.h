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
    VAL_POISON = 0,
    CAP_POSION = -1,
    SIZE_POSION = -1,
};

enum tree_visitor_modes
{
    PRE_ORDER,
    IN_ORDER,
    POST_ORDER,
};

typedef char *tree_elmt;

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

//! @param[in] dest Uninitialised tree.
//! @param[in] capacity Tree capacity.
//! @param[in] root_val Root value.
//! @param[out] dest Initialised tree.
//! @return Returns pointer to root.
//! @brief Initialises tree.
tree_node* tree_ctor(my_tree *dest, ssize_t capacity, const tree_elmt root_val);

//! @param[in] tree Pointer to tree.
//! @param[in] val Value to add.
//! @return Returns pointer to new node.
//! @brief Creates node.
tree_node* node_ctor(my_tree *tree, const tree_elmt val);

//! @param[in] tree Pointer to tree.
//! @brief Destroys tree.
void tree_dtor(my_tree *tree);

//! @param[in] tree Pointer to tree.
//! @brief Creates tree dump.
void tree_dump(my_tree *tree);

//! @param[in] node Pointer to tree node.
//! @param[in] gr_viz File to print graphviz code to.
//! @brief Prints graphviz code to file.
void print_branch(tree_node *node, FILE *gr_viz);

//void tree_visitor(tree_node *node, const char mode, );

#endif

