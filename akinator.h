#ifndef AKINATOR
#define AKINATOR

#include "tree.h"
#include "simple_stack.h"
#include <stdarg.h>

#define SPEAK

const int DEFAULT_TREE_CAP = 100;
const int PRINT_BUFFER_SIZE = 1000;

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

//! @param[in] filename Database filename.
//! @brief Starts game.
void start_game(const char *filename);

//! @param[in] tree Pointer to tree.
//! @param[in] should_speak 0 if should not speak, !0 if should.
//! @brief Starts guess mode.
void guess(my_tree *tree, int should_speak);

//! @param[in] buffer Chars array.
//! @brief Reads chars from stdin until \n is met and stores them in buffer.
void read(char *buffer);

//! @param[in] tree Pointer to tree.
//! @param[in] bad_node Node which will be updated.
//! @param[in] should_speak 0 if should not speak, !0 if should.
//! @brief Updates database.
void update_database(my_tree *tree, tree_node *bad_node, int should_speak);

//! @param[in] node Node to start searching from.
//! @param[in] target Target.
//! @param[in] stk Stack to save route from node to target if found.
//! @brief Finds target in tree.
char tree_find(tree_node *node, const char *target, my_stk *stk);

//! @param[in] tree Pointer to tree.
//! @param[in] should_speak 0 if should not speak, !0 if should.
//! @brief Starts definition mode.
void def(my_tree *tree, int should_speak);

//! @param[in] tree Pointer to tree.
//! @param[in] should_speak 0 if should not speak, !0 if should.
//! @brief Starts comparison mode.
void comp(my_tree *tree, int should_speak);

//! @param[in] should_speak 0 if should not speak, !0 if should.
//! @param[in] fmt Format.
//! @brief Says text using espeak.
void aki_speak(int should_speak, const char *fmt, ...);

//akinator_read.cpp

//! @param[in] filename Database filename.
//! @param[in] tree Tree to save database at.
//! @brief Reads database.
void get_database(const char *filename, my_tree *tree);

//! @param[in] stream File to examine.
//! @return Returns amount of characters in stream.
//! @brief Gets amount of characters in stream.
size_t get_file_size(FILE *stream);

//! @param[in] stream File to read from.
//! @return Returns pointer to char array.
//! @brief Reads text from stream and stores it as char array.
char* read_text(FILE *stream);

//! @param[in] text Text to parse.
//! @param[in] tree Tree to save database at.
//! @brief Parses database.
void parse_database(const char *text, my_tree *tree);

//! @param[in] text Text to read from.
//! @param[in] buffer Buffer to save text to.
//! @brief Reads values for nodes.
char fill_node(const char **text, char *buffer);

//! @param[in] text Text.
//! @param[in] tree Pointer to tree.
//! @param[in] node Node to parse.
//! @param[in] buffer Buffer containing value of node.
//! @brief Parses node.
void parse_node(const char **text, my_tree *tree, tree_node *node, char *buffer);

//akinator_write.cpp

//! @param[in] tree Pointer to tree.
//! @brief Saves database.
void save_database(my_tree *tree);

//! @param[in] node Node to save.
//! @param[in] dest File to save to.
void save_node(tree_node *node, FILE *dest);

#endif
