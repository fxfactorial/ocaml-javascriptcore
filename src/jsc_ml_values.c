/* -*- c++ -*- */
/*
 * jsc_ml_values.c
 * -----------
 * Copyright : (c) 2016, Edgar Aroutiounian <edgar.factorial@gmail.com>
 * Licence   : BSD3
 *
 * This file is a part of ocaml-javascriptcore.
 */

#include <iostream>

#include <JavaScriptCore/JavaScript.h>

#include "jsc_ml_values.h"

// #ifndef _DEBUG
// #else
// const std::string
// current_date_time()
// {
//   time_t     now = time(0);
//   struct tm  tstruct;
//   char       buf[80];
//   tstruct = *localtime(&now);
//   // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
//   // for more information about date/time format
//   strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
//   return buf;
// }
// #endif

extern "C" {

  /* caller must free the string */
  const char*
  jsvalue_to_utf8_string(JSGlobalContextRef ctx, JSValueRef v)
  {
    JSStringRef valueAsString = JSValueToStringCopy(ctx, v, NULL);
    size_t jsSize = JSStringGetMaximumUTF8CStringSize(valueAsString);
    char* jsBuffer = (char*)malloc(jsSize);
    JSStringGetUTF8CString(valueAsString, jsBuffer, jsSize);
    JSStringRelease(valueAsString);
    return jsBuffer;
  }

  CAMLprim value
  jsc_string_to_ml(JSStringRef str)
  {
    CAMLparam0();
    //DEBUG("Converting JSC string into OCaml string");
    size_t string_len = JSStringGetMaximumUTF8CStringSize(str);
    char string_buffer[string_len];
    JSStringGetUTF8CString(str, string_buffer, string_len);
    CAMLreturn(caml_copy_string(string_buffer));
  }

  JSStringRef
  ml_string_to_jsc_string(value ml_string)
  {
    //DEBUG("Converting OCaml string to JSC string");
    return JSStringCreateWithUTF8CString(caml_strdup(String_val(ml_string)));
  }

  CAMLprim value
  jsc_ml_print_js(value ctx, value jsvalue)
  {
    CAMLparam2(ctx, jsvalue);
    auto as_string =
      jsvalue_to_utf8_string(JSContext_val(ctx), JSValue_val(jsvalue));
    std::cout << as_string << std::endl;
    free((void*)as_string);
    CAMLreturn(Val_unit);
  }
}
