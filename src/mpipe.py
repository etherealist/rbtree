"""Send message-pack messages to subprocess."""

import ctypes
from subprocess import Popen, PIPE
import sys
import os
import time
import umsgpack
from contextlib import contextmanager


@contextmanager
def open_and_close(args : list):
    """Open a subprocess for sending message-pack messages in a context.

    After the context it will send a close message: (0,).
    """
    proc = open(args)
    yield proc
    write(proc, (0,))


def open(args : list):
    """Open a subprocess for sending message-pack messages."""
    if os.environ.get("MPP_GDB") == "True":
        proc = Popen(args, stdin=PIPE, stdout=PIPE)
        argv = ["gdb", "-p", str(proc.pid)]
        if os.fork():
            os.execlp(argv[0], *argv)
        time.sleep(2)
        return proc
    elif os.environ.get("MPP_RR") == "True":
        return Popen(["rr"] + args, stdin=PIPE, stdout=PIPE)
    else:
        return Popen(args, stdin=PIPE, stdout=PIPE)


def write(proc : Popen, data) -> bool:
    """Write message to the process, returns True on success."""
    pack = umsgpack.dumps(data)
    size = bytes(ctypes.c_size_t(len(pack)))
    proc.stdin.write(size)
    proc.stdin.write(pack)
    proc.stdin.flush()


def read(proc : Popen):
    """Read message from the process, returns None on failure."""
    size = proc.stdout.read(ctypes.sizeof(ctypes.c_size_t))
    size = int.from_bytes(size,  sys.byteorder)
    pack = proc.stdout.read(size)
    return umsgpack.loads(pack)
