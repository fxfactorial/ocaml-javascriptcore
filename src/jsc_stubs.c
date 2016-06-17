/* -*- c++ -*- */

/*
 * jsc_stubs.c
 * -----------
 * Copyright : (c) 2016, Edgar Aroutiounian <edgar.factorial@gmail.com>
 * Licence   : BSD3
 *
 * This file is a part of ocaml-javascriptcore.
 */

#define CAML_NAME_SPACE
// OCaml declarations
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <iostream>
#include <string>

#include <JavaScriptCore/JavaScript.h>

#include "jsc_ml_values.h"

extern "C" {

  CAMLprim value
  jsc_ml_make_vm(value __attribute__((unused)) unit)
  {
    CAMLparam0();
    CAMLlocal1(jsc_ml_ctx);
    DEBUG("Creating a JSC Global Context, aka a Virtual Machine");

    JSGlobalContextRef ctx = JSGlobalContextCreateInGroup(NULL, NULL);

    jsc_ml_ctx =
      caml_alloc_custom(&jsc_context_ops,
			sizeof(JSGlobalContextRef), 0, 1);
    JSVirtual_machine_val(jsc_ml_ctx) = ctx;
    CAMLreturn(jsc_ml_ctx);
  }

  CAMLprim value
  jsc_ml_eval_script(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    CAMLlocal1(string_buf);
    DEBUG("Evaling a script");

    JSGlobalContextRef context = JSVirtual_machine_val(ctx);

    JSStringRef code = ml_string_to_jsc_string(js_string);

    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    JSValueRef exception;
    JSValueRef js_result =
      JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

    // Need to raise exception if there was an error of sorts
    JSStringRef result_as_string =
      JSValueToStringCopy(context, js_result, &exception);

    string_buf = jsc_string_to_ml(result_as_string);

    JSStringRelease(result_as_string);
    JSStringRelease(code);
    CAMLreturn(string_buf);
  }

  CAMLprim value
  jsc_ml_check_syntax(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    DEBUG("Checking syntax of a script");

    JSContextRef context = JSVirtual_machine_val(ctx);
    JSStringRef js_script =
      JSStringCreateWithUTF8CString(caml_strdup(String_val(js_string)));
    bool is_correct =
      JSCheckScriptSyntax(context, js_script, NULL, 1, NULL);
    JSStringRelease(js_script);
    CAMLreturn(Val_bool(is_correct));
  }

  CAMLprim value
  jsc_ml_garbage_collect(value ctx)
  {
    CAMLparam1(ctx);
    DEBUG("Garbage Collecting");

    JSGarbageCollect(JSVirtual_machine_val(ctx));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_make_string_with_ml_str(value ml_string)
  {
    CAMLparam1(ml_string);
    CAMLlocal1(ml_abstract);
    DEBUG("Creating JSStringRef from OCaml String");

    ml_abstract = caml_alloc(1, Abstract_tag);
    Store_field(ml_abstract, 0, (value)ml_string_to_jsc_string(ml_string));
    CAMLreturn(ml_abstract);
  }

  CAMLprim value
  jsc_ml_make_string(value __attribute__((unused)) unit)
  {
    return Val_unit;
  }

  CAMLprim value
  jsc_ml_retain_jsc_string(value jsc_string)
  {
    CAMLparam1(jsc_string);
    DEBUG("Calling Retain on JSC String");

    JSStringRetain(JSString_val(jsc_string));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_release_jsc_string(value jsc_string)
  {
    CAMLparam1(jsc_string);
    DEBUG("Calling Release on JSC String");

    JSStringRelease(JSString_val(jsc_string));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_jsc_length(value jsc_string)
  {
    CAMLparam1(jsc_string);
    DEBUG("Getting JSC String length");

    CAMLreturn(Val_int(JSStringGetLength(JSString_val(jsc_string))));
  }

}
