#include "mpipe.h"

int
writeall(int fd, void *buf, size_t n)
{
	char *buffer = buf;
	ssize_t r;
	while (n) {
		r = write(fd, buffer, n);
		if (r < 0)
			return -1;
		buffer += (size_t)r;
		n -= (size_t)r;
	}
	return 0;
}

ssize_t
readall(int fd, void *buf, size_t n)
{
	char *buffer = buf;
	size_t ptr = 0;
	ssize_t r;
	for (;;) {
		r = read(fd, buffer + ptr, n - ptr);
		if (r < 0)
			return -1;
		if (r == 0)
			break;
		ptr += (size_t)r;
	}
	return ptr;
}

mpack_node_t*
mpp_fdread_message(int fd, mpp_read_t* context)
{
    ssize_t read_size;
    size_t msg_size;
    read_size = readall(fd, &msg_size, sizeof(size_t));
    if(read_size != sizeof(size_t))
        return NULL;
    context->data = malloc(msg_size);
    read_size = readall(fd, context->data, msg_size);
    if(read_size != (ssize_t) msg_size)
        return NULL;
    mpack_tree_init(&context->tree, context->data, msg_size);
    context->root = mpack_tree_root(&context->tree);
    return &context->root;
}

int
mpp_read_message_fin(mpp_read_t* context)
{
    mpack_error_t err = mpack_tree_destroy(&context->tree);
    free(context->data);
    return err;
}

mpack_writer_t*
mpp_fdwrite_message(int fd, mpp_write_t* context)
{
    context->fd = fd;
    mpack_writer_init_growable(
        &context->writer,
        &context->data,
        &context->size
    );
    return &context->writer;
}

int
mpp_write_message_fin(mpp_write_t* context)
{
    size_t write_size;
    mpack_error_t err = mpack_writer_destroy(&context->writer);
    if(err != mpack_ok)
        return err;
    write_size = writeall(
        context->fd,
        &context->size,
        sizeof(size_t)
    );
    if(write_size != sizeof(size_t))
        return -1;
    writeall(
        context->fd,
        context->data,
        context->size
    );
    if(write_size != context->size)
        return -1;
    free(context->data);
    return 0;
}
