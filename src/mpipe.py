"""Send message-pack messages to subprocess."""

import ctypes
from subprocess import Popen, PIPE
import sys
import umsgpack


def open(args : list) -> Popen:
    """Open a subprocess for sending message-pack messages."""
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
