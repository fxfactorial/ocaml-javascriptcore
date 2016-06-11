# -*- makefile -*-

ml_headers := $(shell opam config var lib)/ocaml
cc := $(shell which clang++)
c_objects := jscore_stubs.o
# Stupid hack
ml_src_files := javaScriptCore.ml
ml_objects_link := javaScriptCore.o

ml_cc_opts := '-std=c++11 -stdlib=libstdc++'

ifeq ($(shell uname),Darwin)
	c_flags := -c -std=c++11 -Wall -I${ml_headers}
	ml_cc_libs := '-framework JavaScriptCore -lc++'
else
	c_flags := -c -std=c++11 -Wall \
	-I${ml_headers} -I/usr/include/webkitgtk-3.0

	ml_cc_libs := '-lJavaScriptCore -lc++'
endif

ml_cc := ocamlfind ocamlopt
ml_jsc_lib := javaScriptCore.cmxa
test_server_pkgs := cohttp.lwt,podge,lwt.ppx

exec := Test_program

%.o: %.cpp; ${cc} ${c_flags} $<
%.cmx: %.ml; ${ml_cc} -c $<

test_server:test_server.ml ml_lib ${c_objects}
	${ml_cc} ${ml_jsc_lib} -cc ${cc} \
	-ccopt ${ml_cc_opts} -package \
	${test_server_pkgs} -linkpkg \
	$< -o ${exec}

ml_lib:${c_objects} ${ml_src_files}
	${ml_cc} -cclib ${ml_cc_libs} \
	${c_objects} ${ml_src_files} -a -linkall \
	-o ${ml_jsc_lib}

.PHONY: clean

clean:; @rm -rf *.o *.cmx *.cmi *.cmt *.a *.cmxa ${exec}
