/* -*- objc -*- */
#define CAML_NAME_SPACE
// OCaml declarations
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/fail.h>

#include <iostream>
#include <string>

#include <Foundation/Foundation.h>
#include <JavaScriptCore/JavaScriptCore.h>

extern "C" {

  CAMLprim value speak(__unused value unit)
  {
    std::cout << "Do speak!\n";
    return Val_unit;
  }

  CAMLprim value exec_js(value js_string)
  {
    const char *const result = caml_strdup(String_val(js_string));
    JSContext *repl_context =
      [[JSContext alloc]
	initWithVirtualMachine:[[JSVirtualMachine alloc] init]];
    JSValue *evaled =
      [repl_context evaluateScript:
		      [NSString stringWithUTF8String:result]];
    // std::cout << [[evaled toString] UTF8String] << "\n";
    return caml_copy_string([[evaled toString] UTF8String]);
  }
}
