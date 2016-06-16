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

static void
jsc_ml_vm_finalize(value ctx)
{
  DEBUG("Calling release for JS Virtual Machine");
  return JSGlobalContextRelease(JSVirtual_machine_val(ctx));
}

struct custom_operations
jsc_context_ops = {
  (char*)"jsc.vm",
  jsc_ml_vm_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

value
jsc_string_to_ml(JSStringRef str)
{
  size_t string_len = JSStringGetMaximumUTF8CStringSize(str);
  char string_buffer[string_len];
  JSStringGetUTF8CString(str, string_buffer, string_len);
  return caml_copy_string(string_buffer);
 }

JSStringRef
ml_string_to_jsc_string(value ml_string)
{
  return JSStringCreateWithUTF8CString(caml_strdup(String_val(ml_string)));
}
