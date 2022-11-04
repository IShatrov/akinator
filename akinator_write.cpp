#include "akinator.h"

#define ALLIGN(n_tabs) for(int i = 0; i < n_tabs; i++) fputc('\t', dest);

void save_database(my_tree *tree)
{
    assert(tree);

    FILE *new_database = fopen("new_database.txt", "w");
    assert(new_database);

    save_node(tree->root, new_database);

    fclose(new_database);
}

void save_node(tree_node *node, FILE *dest)
{
    assert(node);
    assert(dest);

    static int allignment = 0;

    ALLIGN(allignment);
    fputc('{', dest);
    fputc('\n', dest);

    ALLIGN(allignment);
    fprintf(dest, " \"%s\" \n", node->val);

    allignment++;

    if(node->l_child) save_node((tree_node*)node->l_child, dest);
    if(node->r_child) save_node((tree_node*)node->r_child, dest);

    allignment--;

    ALLIGN(allignment);
    fputc('}', dest);
    fputc('\n', dest);

    return;
}
