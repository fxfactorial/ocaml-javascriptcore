If you understand this image then you understand what this project is
about and what its capable of.

![poc](./static/poc.png)

# Installation (Tested on OS X and Ubuntu 14, 15)

`opam` makes this installation very easy, even taking care of system
dependencies for you.

In this directory do:

```shell
$ opam pin add javascriptcore . -y
```

If on Linux then this will probably crap out at first. That is fine,
there will be a `depext` message, follow that message and `opam` will
take care of all the required system dependencies. Then do the `opam
pin` operation again. On `OS X` it has debug logging enabled by
default.

# Backing 

This project is built with OCaml 4.03.0, C++11, libc++, blocks,
JavaScriptCore. It does not use objective-c and works on Linux.

# Help Wanted

This project has plenty of places to improve your JavaScript, OCaml,
C/C++ coding.
