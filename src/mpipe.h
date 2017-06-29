// =====
// mpipe
// =====
//
// Send message-packs over a pipe. Meant as a very simple RPC. It is compatible
// with async coding and therefore used in rbtree and chirp to run hypothesis
// tests on C code. (Segfaults will be detected instead of killing the pytest
// run)
//
// Installation
// ============
//
// Copy mpipe.h and mpipe.c into your source. And add mpack_ to your project.
// Use src/mpipe.py to send messages.
//
// mpipe.py
// ========
//
// .. code-block:: python
//
//    proc = mpipe.open(["./build/test_mpipe"])
//    mpipe.write(proc, (1, 0))
//    res = mpipe.read(proc)
//    print(res)
//    mpipe.write(proc, (0,))
//
// If you set the environment variable MPP_GDB to True. mpipe.open will attach
// a gdb to the process. You need
//
// .. code-block:: bash
//
//    sudo sh -c "echo 0 > /proc/sys/kernel/yama/ptrace_scope"
//
// for that.
//
// If you set the environment variable MPP_RR to True. mpipe.open will record
// an rr session. You need
//
// .. code-block:: bash
//
//    sudo sh -c "echo 1 > /proc/sys/kernel/perf_event_paranoid"
//
// for that.
//
// Development
// ===========
//
// See `README.rst`_
//
// .. _`README.rst`: https://github.com/ganwell/rbtree
//
// API
// ===
//
// mpack_node_t* mpp_read_message(mpp_read_t* context)
//   Read a message and return the root node of a message. *tree* has to be
//   released using mpp_*read_message_fin*. If your program accesses data of
//   multiple messages you can use `context garbage-collection`_. Returns NULL
//   on error.
//
//   Also: mpp_fdread_message(int fd, mpp_read_t* context) use specified fd
//   instead of stdin.
//
// int mpp_read_message_fin(mpp_read_t* context)
//   Release all resources associated with the message. Returns 0 on success
//   (see mpack_error_t).
//
// mpack_writer_t* mpp_write_message(mpp_write_t* context)
//   Start writing a message. Use *mpack_writer_t* to created the message. See
//   mpack_. After the message is finished call *mpp_write_message_fin*.
//
//   Also: mpp_fdwrite_message(int fd, mpp_write_t* context) use specified fd
//   instead of stdout.
//
// int mpp_write_message_fin(mpp_write_t* context)
//   Release all resources associated with the message. Returns 0 on success
//   (see mpack_error_t)
//
// .. _mpack: https://github.com/ludocode/mpack
//
// Context garbage-collection
// ==========================
//
// .. _`context garbage-collection`:
//
// It can be quite painful to release memory immediately after it is used last.
// For example if you convert a document, you have temporary strings that are
// generated by a sub-function and is used by other sub-functions. Who owns
// that memory, where should it be freed? Simple, you know that after
// converting the document, all the temporary strings can be freed, so you add
// them to a stack_ and after the document is converted you free them all.
//
// .. _stack: https://github.com/ganwell/rbtree/blob/master/qs.rst
//
// Declarations
// ============
//
// Includes
// --------
//
// .. code-block:: cpp
//
#ifndef mpp_mpipe_h
#define mpp_mpipe_h
#include <unistd.h>

#include <mpack.h>

// Functions
// ---------
//
// .. code-block:: cpp
//

struct mpp_read_s;
typedef struct mpp_read_s mpp_read_t;
struct mpp_read_s {
    mpack_tree_t tree;
    mpack_node_t node;
    char* data;
};

struct mpp_write_s;
typedef struct mpp_write_s mpp_write_t;
struct mpp_write_s {
    int fd;
    mpack_writer_t writer;
    char* data;
    size_t size;
};

mpack_node_t*
mpp_fdread_message(int fd, mpp_read_t* context);
int
mpp_read_message_fin(mpp_read_t* context);

mpack_writer_t*
mpp_fdwrite_message(int fd, mpp_write_t* context);
int
mpp_write_message_fin(mpp_write_t* context);

#ifdef _WIN32
#   if defined(_MSC_VER) && _MSC_VER < 1600
#       define mpp_inline __inline
#   else // _MSC_VER
#       define mpp_inline inline
#   endif // _MSC_VER
#else
#   define mpp_inline inline
#endif

static
mpp_inline
mpack_node_t*
mpp_read_message(mpp_read_t* context)
{
    return mpp_fdread_message(STDIN_FILENO, context);
}
static
mpp_inline
mpack_writer_t*
mpp_write_message(mpp_write_t* context)
{
    return mpp_fdwrite_message(STDOUT_FILENO, context);
}
#endif //mpp_mpipe_h
