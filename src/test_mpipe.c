#include "mpipe.h"

enum functions {
   fn_quit = 0,
   fn_inc = 1
};

int
main(void)
{
    mpp_read_t mpp_read;
    mpp_write_t mpp_write;
    mpack_node_t* node;
    mpack_writer_t* writer;
    for(;;) {
        node = mpp_read_message(&mpp_read);
        if(node == NULL)
            return 9;
        int function = mpack_node_int(
            mpack_node_array_at(*node, 0)
        );
        if(function == fn_quit)
            return 0;
        else if(function == fn_inc) {
            int val = mpack_node_int(
                mpack_node_array_at(*node, 1)
            ) + 1;
            writer = mpp_write_message(&mpp_write);
            mpack_start_array(writer, 1);
            mpack_write_int(writer, val);
            mpack_finish_array(writer);
            if(mpp_write_message_fin(&mpp_write) != 0)
                return 9;
        }
        if(mpp_read_message_fin(&mpp_read) != 0)
            return 9;
        return 0; //TODO remove
    }
    return 0;
}
