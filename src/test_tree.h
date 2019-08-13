// SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

struct node_s;
typedef struct node_s node_t;
struct node_s {
    int     value;
    char    color;
    node_t* parent;
    node_t* left;
    node_t* right;
};
void
test_init(void);
int
test_add(node_t* node);
void
test_remove_node(node_t* node);
int
test_remove(node_t* node);
int
test_replace_node(node_t* old, node_t* new);
int
test_replace(node_t* old, node_t* new);
int
test_find(node_t* node);
int
test_size(void);
int
test_tree_nil(void);
enum functions {
    fn_quit = 0,
    fn_init = 1,
    fn_add = 2,
    fn_remove_node = 3,
    fn_remove = 4,
    fn_replace_node = 5,
    fn_replace = 6,
    fn_find = 7,
    fn_size = 8,
    fn_test_tree_nil = 9
};
