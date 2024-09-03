rbtree
======

My rbtree code is really good. I developed it for chirp, a project I burned out
over. Chirp was good too, but I don't go back there anymore, too much damage was
done.

**Yes, rbtree is finished.**

I had my quite egocentric code-formatting back then. So I run `rbtree.h` and
`qs.h` through clang-format. I also removed all mention of chirp and some other
crazy ideas I had back then.

* [rbtree.h](rbtree.rst) is a humble red-black tree implementation
* [qs.h](qs.rst) is a humble queue, stack and ring implementation
* performance is very well and it should be very readable and good for learning

It runs with C99 and there shouldn't be any bugs, it was tested like crazy.
If you're curious - some of the testing is still in [old_dev](old_dev). The rest of
the testing went with chirp, chirp is buried in backup somewhere. RIP.

Installation
------------

Copy [rbtree.h](rbtree.h) and/or [qs.h](qs.h) to you source. The only
dependency is `assert.h`, but you can get rid of that easily if you want.

#### License is AGPL-3.0-or-later

Repository Status: Completed
============================

This repository hosts a project that is considered complete in terms of its
initial development goals. It remains publicly available for transparency,
sharing methodologies, and as a resource for others who might find the
implementations useful. Please note the following:

- **Project Completion**: The project has reached completion and fulfills the
  goals it was set to achieve. While I fully support this project, I do not
  anticipate significant changes or additions in the future.
- **Supported but Stable**: I am committed to supporting this project by
  addressing bugs and critical issues. However, the codebase is stable and
  intended to remain largely as it is.
- **Active Support Available**: I am available to provide support and respond to
  issues. Feel free to contact me with questions or if you need help understanding
  the project.
- **Viewing and Forking Encouraged**: You are welcome to view, fork, or use the
  code in your own projects. This project is provided as-is but is well-documented
  and maintained for reliability.

