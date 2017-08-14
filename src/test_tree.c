#include "test_tree.h"
#define test_include
#include "testing.h"
#undef test_include
#include "mpipe.h"

node_t* tree;

void
test_init(void)
{
    my_tree_init(&tree);
}

int
test_add(node_t* node)
{
    int ret;
    my_node_init(node);
    ret = my_insert(&tree, node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    if(ret != 0) {
        assert(!(
                rb_parent_m(node) != my_nil_ptr ||
                rb_left_m(node) != my_nil_ptr ||
                rb_right_m(node) != my_nil_ptr ||
                rb_color_m(node) != RB_BLACK
        ) && "Node should be black");
    }
    return ret;
}

int
test_remove(node_t* key)
{
    node_t* node;
    int ret = my_delete(&tree, key, &node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

int
test_remove2(int val)
{
    node_t key;
    node_t* node;
    key.value = val;
    int ret = my_delete(&tree, &key, &node);
    free(node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

void
test_remove_node(node_t* node)
{
    my_delete_node(&tree, node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
}

static
void
test_remove_node2(int val)
{
    node_t key;
    node_t* node;
    key.value = val;
    my_find(tree, &key, &node);
    my_delete_node(&tree, node);
    free(node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
}

int
test_find(node_t* node)
{
    node_t* out;
    int ret;
    ret = my_find(tree, node, &out);
    if(ret == 0) {
        TA(rb_value_m(node) == rb_value_m(out), "Did not find right node")
    }
    return ret;
}

int
test_replace_node(node_t* old, node_t* new)
{
    int ret = my_replace_node(&tree, old, new);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

int
test_replace_node2(int val)
{
    node_t key;
    node_t* old;
    node_t* new = malloc(sizeof(node_t));
    key.value = val;
    new->value = val;
    my_find(tree, &key, &old);
    int ret = my_replace_node(&tree, old, new);
    if(rb_value_m(old) != rb_value_m(new))
        ret = 1;
    free(old);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

int
test_replace(node_t* old, node_t* new)
{
    node_t* node;
    int ret = my_replace(&tree, old, new, &node);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

int
test_replace2(int val)
{
    node_t key;
    node_t* old;
    node_t* new = malloc(sizeof(node_t));
    rb_value_m(&key) = val;
    rb_value_m(new) = val;
    int ret = my_replace(&tree, &key, new, &old);
    if(rb_value_m(old) != rb_value_m(new))
        ret = 1;
    free(old);
#   ifndef RB_NO_CHECK
        my_check_tree(tree);
#   endif
    return ret;
}

int
test_size(void)
{
    return my_size(tree);
}

int
test_tree_nil(void)
{
    return tree == my_nil_ptr;
}

static
void
return_val(mpack_writer_t* writer, int val)
{
    mpack_start_array(writer, 1);
    mpack_write_int(writer, val);
    mpack_finish_array(writer);
}

static
void
test_handler(mpack_node_t data, mpack_writer_t* writer)
{
    int func = mpack_node_int(
        mpack_node_array_at(data, 0)
    );
    int val = mpack_node_int(
        mpack_node_array_at(data, 1)
    );
    node_t* node = NULL;
    node_t key;
    switch(func) {
        case fn_init:
            test_init();
            return_val(writer, 0);
            break;
        case fn_add:
            node = malloc(sizeof(node_t));
            rb_value_m(node) = val;
            return_val(writer, test_add(node));
            break;
        case fn_remove_node:
            test_remove_node2(val);
            return_val(writer, 0);
            break;
        case fn_remove:
            return_val(writer, test_remove2(val));
            break;
        case fn_replace_node:
            return_val(writer, test_replace_node2(val));
            break;
        case fn_replace:
            return_val(writer, test_replace2(val));
            break;
        case fn_find:
            key.value = val;
            return_val(writer, test_find(&key));
            break;
        case fn_size:
            return_val(writer, test_size() == val);
            break;
        case fn_test_tree_nil:
            return_val(writer, test_tree_nil());
            break;
        case fn_quit:
            exit(0);
            break;
        default:
            assert(0 && "Not implemented");
    }
}

int
main(void)
{
    return mpp_runner(test_handler);
}
