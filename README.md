rbtree
======

My rbtree code is really good. I developed it for chirp, a project I burned out
over. Chirp was good too, but I don't go back there anymore, too much damage was
done.

I had my quite egocentric code-formatting back then. So I run `rbtree.h` and
`qs.h` through clang-format. I also removed all mention of chirp and some other
crazy ideas I had back then.

* `rbtree.h` is a humble red-black tree implementation
* `qs.h` is a humble queue, stack and ring implementation
* performance is very well and it should be very readable and good for learning

Should run with C99 and there shouldn't be any bugs, it tested this like crazy.
If you're curios some of the testing is still in [old_dev](old_dev). The rest of
the testing went with chirp, chirp is buried on backup somewhere. RIP.

Installation
------------

Copy [rbtree.h](rbtree.h) and/or [qs.h](qs.h) to you source. The only
dependency is assert.h, but you can get rid of that easy if you want.

License is AGPL-3.0-or-later
----------------------------
