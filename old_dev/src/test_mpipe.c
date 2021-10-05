// SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
//
// SPDX-License-Identifier: AGPL-3.0-or-later

#include "mpipe.h"

#include <assert.h>

enum functions {
   fn_quit = 0,
   fn_inc = 1
};

static
void
test_handler(mpack_node_t data, mpack_writer_t* writer)
{
    int function = mpack_node_int(
        mpack_node_array_at(data, 0)
    );
    (void)(function);
    assert(function == fn_inc);
    int val = mpack_node_int(
        mpack_node_array_at(data, 1)
    ) + 1;
    mpack_start_array(writer, 1);
    mpack_write_int(writer, val);
    mpack_finish_array(writer);
}

int
main(void)
{
    return mpp_runner(test_handler);
}
