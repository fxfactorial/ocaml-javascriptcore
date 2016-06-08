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

  CAMLprim value exec_js(value ctx, value js_string)
  {
    const char *const result = caml_strdup(String_val(js_string));
    JSContext *_context = (JSContext*)ctx;
    JSValue *evaled =
      [_context evaluateScript:
		  [NSString stringWithUTF8String:result]];
    return caml_copy_string([[evaled toString] UTF8String]);
  }

  CAMLprim value create_js_context_ml(__unused value unit)
  {
    return (value)[[JSContext alloc]
		    initWithVirtualMachine:[[JSVirtualMachine alloc] init]];
  }

}
