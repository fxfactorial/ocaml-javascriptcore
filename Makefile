# -*- makefile -*-

ml_headers := $(shell opam config var lib)/ocaml
cc := $(shell which clang++)
objects := jscore_stubs.o

ml_cc_opts := '-std=c++11 -stdlib=libstdc++'

ifeq ($(shell uname),Darwin)
	flags := -c -std=c++11 -stdlib=libc++ -Wall -I${ml_headers}
	ml_cc_libs := '-framework JavaScriptCore -lc++'
else
	flags := -c -std=c++11 -stdlib=libc++ \
	-Wall -I${ml_headers} -I/usr/include/webkitgtk-3.0

	ml_cc_lib := '-lJavaScriptCore -lc++'
endif

ml_cc := ocamlfind ocamlopt
ml_jsc_lib := javaScriptCore.cmxa
test_server_pkgs := cohttp.lwt,podge,lwt.ppx

exec := Test_program

test_server:ml_lib ${objects}
	${ml_cc} ${ml_jsc_lib} \
	-cc ${cc} -ccopt ${ml_cc_opts} \
	-package ${test_server_pkgs} -linkpkg \
	test_server.ml -o ${exec}

ml_lib:javaScriptCore.ml
	${ml_cc} ${objects} \
	-cc ${cc} -ccopt ${ml_cc_opts} \
	-cclib ${ml_cc_libs} $< -a -o ${ml_jsc_lib}

%.o: %.cpp; ${cc} ${flags} $<

.PHONY: clean

clean:; @rm -rf *.o *.cmx *.cmi *.cmt *.a *.cmxa ${exec}
