/* -*- c++ -*- */
/*
 * jsc_ml_values.h
 * -----------
 * Copyright : (c) 2016, Edgar Aroutiounian <edgar.factorial@gmail.com>
 * Licence   : BSD3
 *
 * This file is a part of ocaml-javascriptcore.
 */

#ifndef JSC_ML_VALUES_H
#define JSC_ML_VALUES_H

#ifndef _DEBUG
#define DEBUG(s)
#else
#include <iostream>
#include <string>
#include <time.h>

const std::string current_date_time(void);
// http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// TrungTN's was easiest to use
#define DEBUG(s)				\
  std::cout << "\033[1;33m["			\
  << current_date_time ()			\
  << "]\036" << " \033[1;36m["			\
  <<  __PRETTY_FUNCTION__ << "]\033[0m: "	\
  << s << "\n"
#endif

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <JavaScriptCore/JavaScript.h>

#define Val_none Val_int(0)

//type t = Undefined | Null | Bool | Number | String | Object
#define Val_js_undefined Val_int(0)
#define Val_js_null      Val_int(1)
#define Val_js_bool      Val_int(2)
#define Val_js_number    Val_int(3)
#define Val_js_string    Val_int(4)
#define Val_js_object    Val_int(5)

#define Val_js_typed_array_Int8Array         Val_int(0)
#define Val_js_typed_array_Int16Array        Val_int(1)
#define Val_js_typed_array_Int32Array        Val_int(2)
#define Val_js_typed_array_Uint8Array        Val_int(3)
#define Val_js_typed_array_Uint8ClampedArray Val_int(4)
#define Val_js_typed_array_Uint16Array       Val_int(5)
#define Val_js_typed_array_Uint32Array       Val_int(6)
#define Val_js_typed_array_Float32Array      Val_int(7)
#define Val_js_typed_array_Float64Array      Val_int(8)
#define Val_js_typed_array_ArrayBuffer       Val_int(9)

#define Some_val(v) Field(v, 0)

CAMLprim value Val_some(value);

#define JSContext_group_val(v) ((JSContextGroupRef)Field(v, 0))
#define JSClass_val(v) ((JSClassRef)Field(v, 0))
#define JSContext_val(v) ((JSGlobalContextRef)Field(v, 0))
#define JSString_val(v) ((JSStringRef)Field(v, 0))
#define JSValue_val(v) ((JSValueRef)Field(v, 0))
#define JSObject_val(v) ((JSObjectRef)Field(v, 0))

extern "C" {

  const char *jsvalue_to_utf8_string(JSContextRef, JSValueRef);
  CAMLprim value jsc_string_to_ml(JSStringRef);
  JSStringRef ml_string_to_jsc_string(value);
  CAMLprim value make_ml_jsobject_ref(void);
  CAMLprim value jsc_ml_print_js(value, value);

}
#endif
