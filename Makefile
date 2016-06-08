# -*- makefile -*-

ml_headers := $(shell opam config var lib)/ocaml
flags := -c -ObjC++ -std=c++11 -stdlib=libstdc++ -Wall -I${ml_headers}
cc := $(shell which clang++)
objects := jscore_stubs.o

ml_cc_opts := '-std=c++11 -stdlib=libstdc++'
ml_cc_libs := '-framework Foundation -framework JavaScriptCore -lc++'
ml_cc := ocamlopt
ml_jsc_lib := javaScriptCore.cmxa

exec := Test_program

all:ml_lib ${objects}
	ocamlopt ${ml_jsc_lib} -cc ${cc} -ccopt ${ml_cc_opts} test_file.ml -o ${exec}

ml_lib:javaScriptCore.ml
	${ml_cc} ${objects} \
	-cc ${cc} -ccopt ${ml_cc_opts} \
	-cclib ${ml_cc_libs} $< -a -o ${ml_jsc_lib}

%.o: %.cpp
	${cc} ${flags} $<

.PHONY: clean

clean:
	@rm -rf *.o *.cmx *.cmi *.cmt *.a *.cmxa ${exec}
