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

// #ifndef _DEBUG
// #define DEBUG(s)
// #else
// #include <iostream>
// #include <string>
// #include <time.h>

// const std::string current_date_time(void);
// // http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// // TrungTN's was easiest to use
// #define DEBUG(s)				\
//   std::cout << "\033[1;33m["			\
//   << current_date_time ()			\
//   << "]\036" << " \033[1;36m["			\
//   <<  __PRETTY_FUNCTION__ << "]\033[0m: "	\
//   << s << "\n"
// #endif

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <JavaScriptCore/JavaScript.h>

#define Val_none Val_int(0)
#define Some_val(v) Field(v, 0)

CAMLprim value Val_some(value);

#define JSVirtual_machine_val(v)		\
  (*((JSGlobalContextRef*)Data_custom_val(v)))

#define JSContext_group_val(v) ((JSContextGroupRef)Field(v, 0))
#define JSClass_val(v) ((JSClassRef)Field(v, 0))
#define JSGlobal_context_val(v) ((JSGlobalContextRef)Field(v, 0))
#define JSContext_val(v) ((JSContextRef)Field(v, 0))

extern struct custom_operations jsc_context_ops;

const char *jsvalue_to_utf8_string(JSContextRef, JSValueRef);
CAMLprim value jsc_string_to_ml(JSStringRef);
JSStringRef ml_string_to_jsc_string(value);
CAMLprim value make_ml_jsobject_ref(void);

#endif
