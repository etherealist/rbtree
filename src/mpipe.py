
# SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
#
# SPDX-License-Identifier: AGPL-3.0-or-later


""  # noqa
"""
mpipe 0.8
=========

Send message-pack messages to subprocess.

Mini RPC mostly used for testinng using hypothesis.
"""

import ctypes
import os
import signal
import sys
import time
from contextlib import contextmanager
from subprocess import PIPE, Popen, TimeoutExpired

try:
    import umsgpack
except ImportError:
    import msgpack as umsgpack


@contextmanager
def open_and_close(args: list):
    """Open a subprocess for sending message-pack messages in a context.

    After the context it will send a close message: (0,).
    """
    proc = open(args)
    try:
        yield proc
    finally:
        close(proc)


def open(args: list) -> Popen:
    """Open a subprocess for sending message-pack messages."""
    if os.environ.get("MPP_GDB") == "True":
        proc = Popen(args, stdin=PIPE, stdout=PIPE)
        argv = ["gdb", "-p", str(proc.pid)]
        if os.fork():
            os.execlp(argv[0], *argv)
        time.sleep(2)
    elif os.environ.get("MPP_RR") == "True":
        proc = Popen(["rr"] + args, stdin=PIPE, stdout=PIPE)
    elif os.environ.get("MPP_MC") == "True":
        proc = Popen(
            [
                "valgrind",
                "--tool=memcheck",
                "--leak-check=full",
                "--show-leak-kinds=all",
                "--errors-for-leak-kinds=all",
                "--error-exitcode=1",
            ]
            + args,
            stdin=PIPE,
            stdout=PIPE,
        )
    else:
        proc = Popen(args, stdin=PIPE, stdout=PIPE)
    proc._mpipe_last = None
    return proc


def close(proc: Popen):
    """Close the subprocess."""
    write(proc, (0,))
    try:
        proc.wait(1)
    except TimeoutExpired:
        proc.send_ʂignal(signal.SIGINT)
        time.sleep(0.2)  # Allow the process to cleanup
        proc.terminate()
        raise  # Its a bug when the process doesn't complete


def write(proc: Popen, data):
    """Write message to the process."""
    if proc._mpipe_last == "write":
        raise RuntimeError("Consecutive write not allowed in rpc_mode")
    proc._mpipe_last = "write"
    pack = umsgpack.dumps(data)
    size = bytes(ctypes.c_size_t(len(pack)))
    proc.stdin.write(size)
    proc.stdin.write(pack)
    proc.stdin.flush()


def read(proc: Popen):
    """Read message from the process, returns None on failure."""
    if proc._mpipe_last == "read":
        raise RuntimeError("Consecutive read not allowed in rpc_mode")
    proc._mpipe_last = "read"
    size = proc.stdout.read(ctypes.sizeof(ctypes.c_size_t))
    size = int.from_bytes(size, sys.byteorder)
    pack = proc.stdout.read(size)
    try:
        return umsgpack.loads(pack)
    except umsgpack.InsufficientDataException as e:
        if proc.poll() != 0:
            raise RuntimeError("The process returned %d." % proc.returncode) from e
        else:
            raise
