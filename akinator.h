#ifndef AKINATOR
#define AKINATOR

#include "tree.h"
#include "simple_stack.h"

const int DEFAULT_TREE_CAP = 100;

enum game_modes
{
    GAME_GUESS = '1',
    GAME_DEF = '2',
    GAME_COMP = '3',
    GAME_SHOW = '4',
    GAME_SAVE_AND_EXIT = '5',
    GAME_EXIT = '6',
};

const char GAME_IS_ON = 1;

//akinator_game.cpp

void start_game(const char *filename);

void guess(my_tree *tree);

void read(char *buffer);

void update_database(my_tree *tree, tree_node *bad_node);

char tree_find(tree_node *node, const char *target, my_stk *stk);

void def(my_tree *tree);

void comp(my_tree *tree);

//akinator_read.cpp

void get_database(const char *filename, my_tree *tree);

//! @param[in] stream File to examine.
//! @return Returns amount of characters in stream.
//! @brief Gets amount of characters in stream.
size_t get_file_size(FILE *stream);

//! @param[in] stream File to read from.
//! @return Returns pointer to char array.
//! @brief Reads text from stream and stores it as char array.
char* read_text(FILE *stream);

void parse_database(const char *text, my_tree *tree);

char fill_buffer(const char **text, char *buffer);

void parse_node(const char **text, my_tree *tree, tree_node *node, char *buffer);

//akinator_write.cpp

void save_database(my_tree *tree);

void save_node(tree_node *node, FILE *dest);

#endif
