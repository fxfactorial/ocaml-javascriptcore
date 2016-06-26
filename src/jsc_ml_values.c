/* -*- c++ -*- */

/*
 * jsc_ml_values.c
 * -----------
 * Copyright : (c) 2016, Edgar Aroutiounian <edgar.factorial@gmail.com>
 * Licence   : BSD3
 *
 * This file is a part of ocaml-javascriptcore.
 */

#include <JavaScriptCore/JavaScript.h>

#include "jsc_ml_values.h"

#ifndef _DEBUG
#else
const std::string
current_date_time()
{
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
  // for more information about date/time format
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  return buf;
}
#endif

CAMLprim value
Val_some(value v)
{
  CAMLparam1(v);
  CAMLlocal1(some);
  some = caml_alloc(1, 0);
  Store_field(some, 0, v);
  CAMLreturn(some);
}

value
jsc_string_to_ml(JSStringRef str)
{
  DEBUG("Converting JSC string into OCaml string");

  size_t string_len = JSStringGetMaximumUTF8CStringSize(str);
  char string_buffer[string_len];
  JSStringGetUTF8CString(str, string_buffer, string_len);
  return caml_copy_string(string_buffer);
}

JSStringRef
ml_string_to_jsc_string(value ml_string)
{
  DEBUG("Converting OCaml string to JSC string");

  return JSStringCreateWithUTF8CString(caml_strdup(String_val(ml_string)));
}

CAMLprim value string_list_of_prop_array(JSPropertyNameArrayRef arr)
{
  CAMLlocal3(as_ml_list, cons, as_ml_string);

  as_ml_list = Val_emptylist;

  size_t len = JSPropertyNameArrayGetCount(arr);
  printf("Total num is: %lu\n", len);
  for (size_t i = 0; i < len; i++) {
    cons = caml_alloc(2, 0);
    as_ml_string =
      jsc_string_to_ml(JSPropertyNameArrayGetNameAtIndex(arr, i));
    printf("Property Name: %s\n", String_val(as_ml_string));
    Store_field(cons,
		0,
		as_ml_string);
    Store_field(cons, 1, as_ml_list);
    as_ml_list = cons;
  }

  return as_ml_list;
}
