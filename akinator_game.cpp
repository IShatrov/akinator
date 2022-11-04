#include "akinator.h"

#define CLEAR_BUFFER while(getchar() != '\n');

#define AKI_PRINT(...)                               \
{printf(__VA_ARGS__);                                \
if(should_speak)                                     \
{                                                    \
    char print_buffer[PRINT_BUFFER_SIZE + 1] = {0};  \
    sprintf(print_buffer, __VA_ARGS__);              \
                                                     \
    aki_speak(print_buffer);                         \
}}                                                   \

int should_speak = 0;

void start_game(const char *filename)
{
    assert(filename);

    my_tree tree;

    get_database(filename, &tree);

    printf("Would you like to enable text-to-speech? y/n\n");
    should_speak = (getchar() == 'y') ? 1:0;
    CLEAR_BUFFER

    while(GAME_IS_ON)
    {
        AKI_PRINT("What to do?\n"
                "%c. Try to guess your character\n"
                "%c. Give definition\n"
                "%c. Compare\n"
                "%c. Show database\n"
                "%c. Save database and exit\n"
                "%c. Exit without saving\n",
                GAME_GUESS, GAME_DEF, GAME_COMP, GAME_SHOW, GAME_SAVE_AND_EXIT, GAME_EXIT);

        int input = getchar();
        CLEAR_BUFFER

        switch(input)
        {
            case GAME_GUESS:
                guess(&tree);
                break;
            case GAME_DEF:
                def(&tree);
                break;
            case GAME_COMP:
                comp(&tree);
                break;
            case GAME_SHOW:
                tree_dump(&tree);
                break;
            case GAME_SAVE_AND_EXIT:
                save_database(&tree);
                return;
            case GAME_EXIT:
                return;
            default:
                AKI_PRINT("Please try again\n\n");
        }
    }

    return;
}

void guess(my_tree *tree)
{
    assert(tree);

    AKI_PRINT("Type y for yes and n for no\n");

    tree_node *node = tree->root;

    while(node->l_child && node->r_child)
    {
        AKI_PRINT("%s?\n", node->val);

        int input = getchar();
        CLEAR_BUFFER;

        switch(input)
        {
            case 'y':
                node = (tree_node*)node->l_child;
                break;
            case 'n':
                node = (tree_node*)node->r_child;
                break;
            default:
                AKI_PRINT("Please try again\n\n");
        }
    }

    AKI_PRINT("Is it %s?\n", node->val);

    int input = getchar();
    CLEAR_BUFFER;

    switch(input)
    {
        case 'y':
            AKI_PRINT("I won again\n");
            return;
        case 'n':
            update_database(tree, node);
            break;
        default:
            AKI_PRINT("Please try again\n\n");
    }

    return;
}

void read(char *buffer)
{
    assert(buffer);

    char chr = getchar();
    int chars_got = 0;

    while(chr != '\n')
    {
        buffer[chars_got++] = chr;

        chr = getchar();
    }

    buffer[chars_got] = '\0';

    return;
}

void update_database(my_tree *tree, tree_node *bad_node)
{
    assert(tree);

    AKI_PRINT("Who was it then?\n");

    char new_name[MAX_STR_LEN + 1] = {0};
    read(new_name);

    AKI_PRINT("How is %s different from %s?\n", new_name, bad_node -> val);

    char new_question[MAX_STR_LEN + 1] = {0};
    read(new_question);

    AKI_PRINT("So %s %s. You will not win next time!\n", new_name, new_question);

    tree_node *new_node = node_ctor(tree, bad_node->val);
    bad_node->r_child = new_node;

    new_node = node_ctor(tree, new_name);
    bad_node->l_child = new_node;

    strncpy(bad_node->val, new_question, MAX_STR_LEN);

    return;
}

char tree_find(tree_node *node, const char *target, my_stk *stk)
{
    assert(node);
    assert(target);
    assert(stk);

    if(!strcmp(target, node->val)) return 1;

    if(node->l_child && tree_find((tree_node*)node->l_child, target, stk))
    {
        stk_push(stk, 'y');

        return 1;
    }

    if(node->r_child && tree_find((tree_node*)node->r_child, target, stk))
    {
        stk_push(stk, 'n');

        return 1;
    }

    return 0;
}

void def(my_tree *tree)
{
    assert(tree);

    AKI_PRINT("Whose definition would you like to get?\n");

    char target[MAX_STR_LEN + 1] = {0};
    read(target);

    my_stk stk;
    STK_CTOR(&stk, MIN_REC_CAPACITY, stdout);

    if(tree_find(tree->root, target, &stk))
    {
        AKI_PRINT("%s ", target);

        tree_node *node = tree->root;

        while(stk.size)
        {
            char ans = stk_pop(&stk);

            if(ans == 'y')
            {
                AKI_PRINT("%s, ", node->val);

                node = (tree_node*)node->l_child;
            }
            else if(ans == 'n')
            {
                AKI_PRINT("not %s, ", node->val);

                node = (tree_node*)node->r_child;
            }
            else AKI_PRINT("ERROR: bad stack content\n");
        }
    }
    else AKI_PRINT("%s was not found in database\n", target);

    return;
}

void comp(my_tree *tree)
{
    assert(tree);

    AKI_PRINT("Enter two objects to compare\n");

    char target1[MAX_STR_LEN + 1] = {0};
    read(target1);

    char target2[MAX_STR_LEN + 1] = {0};
    read(target2);

    my_stk stk1, stk2;
    STK_CTOR(&stk1, MIN_REC_CAPACITY, stdout);
    STK_CTOR(&stk2, MIN_REC_CAPACITY, stdout);

    char found1 = 0, found2 = 0;

    if(!(found1 = tree_find(tree->root, target1, &stk1))) AKI_PRINT("%s not found in database\n", target1);
    if(!(found2 = tree_find(tree->root, target2, &stk2))) AKI_PRINT("%s not found in database\n", target2);


    if(found1 && found2)
    {
        char ans1 = 0, ans2 = 0;
        tree_node *node1 = tree->root, *node2 = tree->root;

        AKI_PRINT("They both ");
        while(stk1.size && stk2.size)
        {
            ans1 = stk_pop(&stk1);
            ans2 = stk_pop(&stk2);

            if(ans1 != ans2) break;

            if(ans1 == 'y')
            {
                AKI_PRINT("%s, ", node1->val);

                node1 = (tree_node*)node1->l_child;
                node2 = (tree_node*)node2->l_child;
            }
            else if(ans1 == 'n')
            {
                AKI_PRINT("not %s, ", node1->val);

                node1 = (tree_node*)node1->r_child;
                node2 = (tree_node*)node2->r_child;
            }
            else AKI_PRINT("ERROR: bad stack content\n");
        }

        putchar('\n');
        AKI_PRINT("but %s ", target1);

        do
        {
            if(node1->l_child)
            {
                if(ans1 == 'y')
                {
                    AKI_PRINT("%s, ", node1->val);

                    node1 = (tree_node*)node1->l_child;
                }
                else if(ans1 == 'n')
                {
                    AKI_PRINT("not %s, ", node1->val);

                    node1 = (tree_node*)node1->r_child;
                }
                else AKI_PRINT("ERROR: bad stack content\n");

                if(stk1.size) ans1 = stk_pop(&stk1);
            }
        }while(stk1.size);

        putchar('\n');
        AKI_PRINT("and %s ", target2);
        do
        {
            if(node2->l_child)
            {
                if(ans2 == 'y')
                {
                    AKI_PRINT("%s, ", node2->val);

                    node2 = (tree_node*)node2->l_child;
                }
                else if(ans2 == 'n')
                {
                    AKI_PRINT("not %s, ", node2->val);

                    node2 = (tree_node*)node2->r_child;
                }
                else AKI_PRINT("ERROR: bad stack content\n");

                if(stk2.size) ans1 = stk_pop(&stk2);
            }
        }while(stk2.size);
    }

    return;
}

void aki_speak(const char *text)
{
    assert(text);

    #ifdef SPEAK
    char command[PRINT_BUFFER_SIZE + 1] = "espeak.exe \"";

    strncat(command, text, MAX_STR_LEN*10);
    strncat(command, "\"", MAX_STR_LEN*10);

    int chr = 0;            //replace \n with space so that espeak could pronounce everything
    while(command[chr])
    {
        if(command[chr] == '\n') command[chr] = ' ';
        chr++;
    }

    system(command);
    #endif

    return;
}

#undef CLEAR_BUFFER
