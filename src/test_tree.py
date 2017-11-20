"""Test if the tree stays consistent."""
import mpipe
import os
from os import path
from build._rbtree_tests import lib, ffi
from hypothesis import settings, Verbosity
from hypothesis.stateful import GenericStateMachine
from hypothesis.strategies import tuples, sampled_from, just, integers


class Node(object):
    """Represent a node in the tree."""

    def __init__(self, node, value):
        node.value = value
        self.node = node

    def __eq__(self, other):
        """Eq comparison."""
        return self.node.value == other.node.value

    def __lt__(self, other):
        """Lt comparison."""
        return self.node.value < other.node.value

    def __hash__(self):
        """Hash function."""
        return self.node.value


class GenTree(GenericStateMachine):
    """Test if the stays consistent."""

    def __init__(self):
        self.comparison = set()
        self.key = Node(ffi.new("node_t*"), 0)
        lib.test_init()

    def steps(self):
        add_node = tuples(
            just("add"),
            integers(
                min_value=-2**30,
                max_value=(2**30) - 1
            )
        )
        rnd_find = tuples(
            just("rnd_find"),
            integers(
                min_value=-2**30,
                max_value=(2**30) - 1
            )
        )
        delete_node = tuples(just(
            "delete_node"
        ), sampled_from(sorted(self.comparison)))
        delete = tuples(just(
            "delete"
        ), sampled_from(sorted(self.comparison)))
        replace_node = tuples(just(
            "replace_node"
        ), sampled_from(sorted(self.comparison)))
        replace = tuples(just(
            "replace"
        ), sampled_from(sorted(self.comparison)))
        find = tuples(just("find"), sampled_from(sorted(self.comparison)))
        if not self.comparison:
            return add_node | rnd_find
        else:
            return (
                add_node | rnd_find | delete_node | delete | find |
                replace | replace_node
            )

    def execute_step(self, step):
        action, value = step
        if action == 'delete_node':
            assert lib.test_remove(value.node) == 0
            self.comparison.remove(value)
            assert value not in self.comparison
            if not self.comparison:
                assert lib.test_tree_nil() == 1
            else:
                assert lib.test_remove(value.node) == 1
        elif action == 'delete':
            lib.test_remove_node(value.node)
            self.comparison.remove(value)
            assert value not in self.comparison
            if not self.comparison:
                assert lib.test_tree_nil() == 1
        elif action == 'replace_node':
            new = ffi.new("node_t*")
            new.value = value.node.value
            assert lib.test_replace_node(value.node, new) == 0
            value.node = new
            other = Node(ffi.new("node_t*"), value.node.value + 1)
            if other not in self.comparison:
                assert lib.test_replace_node(value.node, other.node) == 1
        elif action == 'replace':
            new = ffi.new("node_t*")
            new.value = value.node.value
            assert lib.test_replace(value.node, new) == 0
            value.node = new
            other = Node(ffi.new("node_t*"), value.node.value + 1)
            if other not in self.comparison:
                assert lib.test_replace(value.node, other.node) == 1
        elif action == 'add':
            node = Node(ffi.new("node_t*"), value)
            if node in self.comparison:
                assert lib.test_add(node.node) != 0
            else:
                assert lib.test_add(node.node) == 0
            self.comparison.add(node)
            assert node in self.comparison
        elif action == 'rnd_find':
            key = self.key
            key.node.value = value
            if key in self.comparison:
                assert lib.test_find(key.node) == 0
            else:
                assert lib.test_find(key.node) != 0
        elif action == 'find':
            key = self.key
            key.node.value = value.node.value
            assert lib.test_size() == len(self.comparison)
            assert lib.test_find(key.node) == 0
        else:
            assert False


class GenMpipeTree(GenericStateMachine):
    """Test if the stays consistent."""

    def __init__(self):
        self.comparison = set()
        build = os.environ.get("BUILD")
        self.proc = mpipe.open([path.join(
            build or "build",
            "test_tree"
        )])
        mpipe.write(self.proc, (lib.fn_init, 0))
        assert mpipe.read(self.proc) == [0]

    def teardown(self):
        mpipe.close(self.proc)
        self.proc = None  # Hypothesis seems to keep GSM objects

    def steps(self):
        add_node = tuples(
            just("add"),
            integers(
                min_value=-2**30,
                max_value=(2**30) - 1
            )
        )
        delete_node = tuples(just(
            "delete_node"
        ), sampled_from(sorted(self.comparison)))
        replace_node = tuples(just(
            "replace_node"
        ), sampled_from(sorted(self.comparison)))
        delete = tuples(just(
            "delete"
        ), sampled_from(sorted(self.comparison)))
        replace = tuples(just(
            "replace"
        ), sampled_from(sorted(self.comparison)))
        rnd_find = tuples(
            just("rnd_find"),
            integers(
                min_value=-2**30,
                max_value=(2**30) - 1
            )
        )
        find = tuples(just("find"), sampled_from(sorted(self.comparison)))
        if not self.comparison:
            return add_node | rnd_find
        else:
            return (
                add_node | delete_node | rnd_find | replace_node |
                find | delete | replace
            )

    def execute_step(self, step):
        proc = self.proc
        action, value = step
        if action == 'add':
            mpipe.write(proc, (lib.fn_add, value))
            if value in self.comparison:
                assert mpipe.read(proc) != [0]
            else:
                assert mpipe.read(proc) == [0]
            self.comparison.add(value)
            return
        elif action == "rnd_find":
            mpipe.write(proc, (lib.fn_find, value))
            if value in self.comparison:
                assert mpipe.read(proc) == [0]
            else:
                assert mpipe.read(proc) != [0]
            return
        elif action == "find":
            mpipe.write(proc, (lib.fn_find, value))
        elif action == "replace_node":
            mpipe.write(proc, (lib.fn_replace_node, value))
        elif action == "replace":
            mpipe.write(proc, (lib.fn_replace, value))
        elif action == "delete":
            assert action == "delete"
            mpipe.write(proc, (lib.fn_remove, value))
            self.comparison.remove(value)
        else:
            assert action == "delete_node"
            mpipe.write(proc, (lib.fn_remove_node, value))
            self.comparison.remove(value)
        assert mpipe.read(proc) == [0]


with settings(max_examples=2000, verbosity=Verbosity.verbose):
    TestTree = GenTree.TestCase
    TestMpipeTree = GenMpipeTree.TestCase
