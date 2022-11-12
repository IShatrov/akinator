#include "akinator.h"

#define SKIP_SPACES(text) while(isspace(*text)) text++
#define SKIP_SPACES_AND_CLOSE_BRACKETS(text) while(isspace(*text) || (*text) == '}') text++

void get_database(const char *filename, my_tree *tree, char **text)
{
    assert(filename);
    assert(tree);

    FILE *database = fopen(filename, "r");
    assert(database);

    *text = read_text(database);

    parse_database(*text, tree);

    fclose(database);

    return;
}

size_t get_file_size(FILE *stream)
{
    assert(stream != NULL);

    size_t ans = 0;

    assert(!fseek(stream, 0, SEEK_END));   //fseek returns zero upon success and non-zero otherwise
    ans = ftell(stream);
    assert(!ferror(stream));

    assert(!fseek(stream, 0, SEEK_SET));

    return ans;
}

char* read_text(FILE *stream)
{
    assert(stream != NULL);

    size_t len = get_file_size(stream);
    assert(len != 0);

    char* text = (char*)calloc(len + 1, sizeof(char)); //+1 for null terminator
    assert(text != NULL);

    fread(text, sizeof(char), len, stream);
    assert(!ferror(stream));

    return text;
}

void parse_database(char *text, my_tree *tree)
{
    assert(text);
    assert(tree);

    //char buffer[MAX_STR_LEN + 1] = {0};

    char *root_val = fill_node(&text);

    tree_node *node = tree_ctor(tree, DEFAULT_TREE_CAP, root_val);
    node->is_new = 0;

    parse_node(&text, tree, node);
}

char* fill_node(char **text)
{
    assert(text);
    assert(*text);

    SKIP_SPACES((*text));

    char *ptr = NULL;

    if(**text == '{')
    {
        (*text)++;
        SKIP_SPACES((*text));

        if(**text == '"')
        {
            (*text)++;

            ptr = *text;

            while(**text != '"')
            {
                (*text)++;
            }

            **text = '\0';

            (*text)++;
            SKIP_SPACES((*text));
        }
        else printf("ERROR: invalid database\n");
    }
    else if(**text == '}')
    {
        (*text)++;

        SKIP_SPACES((*text));

        return NULL;
    }
    else printf("ERROR: invalid database\n");

    return ptr;
}

void parse_node(char **text, my_tree *tree, tree_node *node)
{
    assert(text);
    assert(*text);
    assert(tree);
    assert(node);

    char *ptr = NULL;

    if((ptr = fill_node(text)))
    {
        tree_node *new_node = node_ctor(tree, ptr);
        new_node->is_new = 0;

        node->l_child = new_node;

        parse_node(text, tree, new_node);
    }
    else
    {
        SKIP_SPACES_AND_CLOSE_BRACKETS((*text));
        return;
    }

    if((ptr = fill_node(text)))
    {
        tree_node *new_node = node_ctor(tree, ptr);
        new_node->is_new = 0;

        node->r_child = new_node;

        parse_node(text, tree, new_node);
    }
    else
    {
        SKIP_SPACES_AND_CLOSE_BRACKETS((*text));
        return;
    }

    return;
}

#undef SKIP_SPACES
#undef SKIP_SPACES_AND_CLOSE_BRACKETS
