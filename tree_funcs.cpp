#include "tree.h"

tree_node* tree_ctor(my_tree *dest, ssize_t capacity, const tree_elmt root_val)
{
    assert(dest);
    assert(capacity > 0);

    dest->data = (tree_node*)calloc(capacity, sizeof(tree_node));
    assert(dest->data);

    dest->size = 1;
    dest->capacity = capacity;

    dest->data[ROOT].l_child = NULL;
    dest->data[ROOT].r_child = NULL;

    dest->data[ROOT].val = root_val;
    //strncpy(dest->data[ROOT].val, root_val, MAX_STR_LEN);

    dest->root = &(dest->data[ROOT]);

    return dest->root;
}

tree_node* node_ctor(my_tree *tree, const tree_elmt val)
{
    assert(tree);
    assert(tree->data);

    tree->data[tree->size].l_child = NULL;
    tree->data[tree->size].r_child = NULL;

    tree->data[tree->size].val = val;
    //strncpy(tree->data[tree->size].val, val, MAX_STR_LEN);

    (tree->size)++;

    return &(tree->data[tree->size - 1]);
}

void tree_dtor(my_tree *tree)
{
    assert(tree);

    for(int nodes_killed = 0; nodes_killed < tree->size; nodes_killed++)
    {
        tree->data[nodes_killed].val = NULL;

        tree->data[nodes_killed].l_child = NULL;
        tree->data[nodes_killed].r_child = NULL;
    }

    tree->root = NULL;

    tree->size = SIZE_POSION;
    tree->capacity = CAP_POSION;

    free(tree->data);
    tree->data = NULL;

    return;
}

void tree_dump(my_tree *tree)
{
    assert(tree);

    FILE *gr_viz = fopen("graphviz_code.txt", "w");
    assert(gr_viz);

    fprintf(gr_viz, "digraph dump\n{\n\t"
                        "node[shape = \"record\", style = \"rounded\"];\n\n");

    print_branch(tree->root, gr_viz);

    putc('}', gr_viz);

    fclose(gr_viz);

    system("dot.exe -T png -o dump_pic.png graphviz_code.txt");
    system("dump_pic.png");

    return;
}

void print_branch(tree_node *node, FILE *gr_viz)
{
    assert(node);
    assert(gr_viz);

    if(node->l_child)
    {
        fprintf(gr_viz, "\t" "\"%p\"->\"%p\" [label = \"y\"] ;\n",
            node, node->l_child);

        print_branch((tree_node*)node->l_child, gr_viz);
    }

    if(node->r_child)
    {
        fprintf(gr_viz, "\t" "\"%p\"->\"%p\" [label = \"n\"] ;\n", node, node->r_child);

        print_branch((tree_node*)node->r_child, gr_viz);
    }

    fprintf(gr_viz, "\t" "\"%p\"[label = \"{%p | val: %s | {l: %p | r: %p}}\"];\n\n",
        node, node, node->val, node->l_child, node->r_child);

    return;
}
