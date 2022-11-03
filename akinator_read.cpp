#include "akinator.h"

#define SKIP_SPACES(text) while(isspace(*text)) text++
#define SKIP_SPACES_AND_CLOSE_BRACKETS(text) while(isspace(*text) || (*text) == '}') text++

void get_database(const char *filename, my_tree *tree)
{
    assert(filename);
    assert(tree);

    FILE *database = fopen(filename, "r");
    assert(database);

    char *text = read_text(database);

    parse_database(text, tree);

    free(text);
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

void parse_database(const char *text, my_tree *tree)
{
    assert(text);
    assert(tree);

    char buffer[MAX_STR_LEN + 1] = {0};

    fill_buffer(&text, buffer);

    tree_node *node = tree_ctor(tree, DEFAULT_TREE_CAP, buffer);

    parse_node(&text, tree, node, buffer);
}

char fill_buffer(const char **text, char *buffer)
{
    assert(text);
    assert(*text);
    assert(buffer);

    int buffer_len = 0;

    SKIP_SPACES((*text));

    if(**text == '{')
    {
        (*text)++;
        SKIP_SPACES((*text));

        if(**text == '"')
        {
            (*text)++;

            while(**text != '"' && buffer_len <= MAX_STR_LEN)
            {
                buffer[buffer_len++] = *((*text)++);
            }

            buffer[buffer_len] = '\0';

            (*text)++;
            SKIP_SPACES((*text));
        }
        else printf("ERROR: invalid database\n");
    }
    else if(**text == '}')
    {
        (*text)++;

        SKIP_SPACES((*text));

        return 0;
    }
    else printf("ERROR: invalid database\n");

    return 1;
}

void parse_node(const char **text, my_tree *tree, tree_node *node, char *buffer)
{
    assert(text);
    assert(*text);
    assert(tree);
    assert(node);
    assert(buffer);

    if(fill_buffer(text, buffer))
    {
        tree_node *new_node = node_ctor(tree, buffer);

        node->l_child = new_node;

        parse_node(text, tree, new_node, buffer);
    }
    else
    {
        SKIP_SPACES_AND_CLOSE_BRACKETS((*text));
        return;
    }

    if(fill_buffer(text, buffer))
    {
        tree_node *new_node = node_ctor(tree, buffer);

        node->r_child = new_node;

        parse_node(text, tree, new_node, buffer);
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
