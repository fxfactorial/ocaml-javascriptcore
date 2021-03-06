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

extern "C" {

  CAMLprim value
  Val_some(value v)
  {
    CAMLparam1(v);
    CAMLlocal1(some);
    some = caml_alloc(1, 0);
    Store_field(some, 0, v);
    CAMLreturn(some);
  }

  /* caller must free the string */
  const char*
  jsvalue_to_utf8_string(JSContextRef ctx, JSValueRef v)
  {
    JSStringRef valueAsString = JSValueToStringCopy(ctx, v, NULL);
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(valueAsString);
    char* jsBuffer = (char*)malloc(jsSize);
    JSStringGetUTF8CString(valueAsString, jsBuffer, jsSize);
    JSStringRelease(valueAsString);
    return jsBuffer;
  }

  CAMLprim value
  jsc_ml_any_to_string(value ctx, value any)
  {
    CAMLparam2(ctx, any);
    // DEBUG("Converting some JSValueRef into an OCaml string");
    CAMLlocal1(as_string);
    const char *string_utf8 =
      jsvalue_to_utf8_string(JSContext_val(ctx), JSValue_val(any));
    as_string = caml_copy_string(string_utf8);
    free((void*)string_utf8);
    CAMLreturn(as_string);
  }

  CAMLprim value
  jsc_ml_js_string_to_string(value context, value js_string)
  {
    CAMLparam2(context, js_string);
    CAMLlocal1(result);
    size_t js_size = JSStringGetMaximumUTF8CStringSize(JSString_val(js_string));
    char *js_buffer = (char*)malloc(js_size);
    JSStringGetUTF8CString(JSString_val(js_string), js_buffer, js_size);
    free((void*)js_buffer);
    CAMLreturn(caml_copy_string(js_buffer));
  }
  // JSStringRef
  // ml_string_to_jsc_string(value ml_string)
  // {
  //   //DEBUG("Converting OCaml string to JSC string");
  //   return JSStringCreateWithUTF8CString(caml_strdup(String_val(ml_string)));
  // }

}
