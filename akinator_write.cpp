#include "akinator.h"

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

    for(int i = 0; i < allignment; i++) fputc('\t', dest);
    fputc('{', dest);
    fputc('\n', dest);

    for(int i = 0; i < allignment; i++) fputc('\t', dest);
    fprintf(dest, " \"%s\" \n", node->val);

    allignment++;

    if(node->l_child) save_node((tree_node*)node->l_child, dest);
    if(node->r_child) save_node((tree_node*)node->r_child, dest);

    allignment--;

    for(int i = 0; i < allignment; i++) fputc('\t', dest);
    fputc('}', dest);
    fputc('\n', dest);

    return;
}
