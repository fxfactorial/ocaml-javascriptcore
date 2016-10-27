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
    JSValueRef exception;
    JSGlobalContextRef context = JSVirtual_machine_val(ctx);
    JSStringRef code = ml_string_to_jsc_string(js_string);
    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    JSValueRef js_result =
      JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

    JSStringRelease(code);

    if (js_result) {
      JSStringRef result_as_string =
	JSValueToStringCopy(context, js_result, &exception);
      string_buf = jsc_string_to_ml(result_as_string);
      JSStringRelease(result_as_string);
      CAMLreturn(string_buf);
    }
    else {
      JSStringRef exceptionIString = JSValueToStringCopy(context, exception, NULL);
      size_t exceptionUTF8Size = JSStringGetMaximumUTF8CStringSize(exceptionIString);
      char exceptionUTF8[exceptionUTF8Size];
      JSStringGetUTF8CString(exceptionIString, exceptionUTF8, exceptionUTF8Size);
      JSStringRelease(exceptionIString);
      caml_raise_with_string(*caml_named_value("js-exn"), exceptionUTF8);
    }
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
  jsc_ml_set_context_name(value context_value, value new_context_name)
  {
    CAMLparam2(context_value, new_context_name);
    DEBUG("Setting Context Name");
    JSGlobalContextSetName(JSVirtual_machine_val(context_value),
			   ml_string_to_jsc_string(new_context_name));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_get_context_name(value context_value)
  {
    CAMLparam1(context_value);
    DEBUG("Getting Context Name");
    CAMLreturn(jsc_string_to_ml(JSGlobalContextCopyName(JSVirtual_machine_val(context_value))));
  }
}
