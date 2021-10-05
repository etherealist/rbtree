
# SPDX-FileCopyrightText: 2019 Jean-Louis Fuchs <ganwell@fangorn.ch>
#
# SPDX-License-Identifier: AGPL-3.0-or-later


"""Test if we can set traits (yes it is a stupid test)."""
from build._rbtree_tests import lib
from test_all import call_ffi


def test_traits():
    call_ffi(lib.test_traits)
