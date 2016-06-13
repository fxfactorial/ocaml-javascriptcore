/* -*- c++ -*- */

/*
 * jscore_stubs.cpp
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

#define JSContext_val(v) (*((JSGlobalContextRef*)Data_custom_val(v)))

static void
jsc_ml_context_finalize(value ctx)
{
  return JSGlobalContextRelease(JSContext_val(ctx));
}

static struct
custom_operations jsc_context_ops = {
  (char*)"jsc.context",
  jsc_ml_context_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

extern "C" {

  CAMLprim value
  jsc_ml_make_context(value __attribute__((unused)) unit)
  {
    CAMLparam0();
    JSGlobalContextRef ctx = JSGlobalContextCreateInGroup(NULL, NULL);

    value jsc_ml_ctx =
      caml_alloc_custom(&jsc_context_ops,
			sizeof(JSGlobalContextRef), 0, 1);
    JSContext_val(jsc_ml_ctx) = ctx;
    CAMLreturn(jsc_ml_ctx);
  }

  CAMLprim value
  jsc_ml_eval_script(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    const char *const result = caml_strdup(String_val(js_string));

    // JSGlobalContextRef context = (JSGlobalContextRef)ctx;
    JSGlobalContextRef context = JSContext_val(ctx);

    JSStringRef code = JSStringCreateWithUTF8CString(result);

    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    JSValueRef exception;
    JSValueRef js_result =
      JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

    JSStringRef result_as_string =
      JSValueToStringCopy(context, js_result, &exception);

    size_t string_len =
      JSStringGetMaximumUTF8CStringSize(result_as_string);
    char *const string_buffer = (char*)malloc(string_len);
    JSStringGetUTF8CString(result_as_string, string_buffer, string_len);
    JSStringRelease(code);
    CAMLreturn(caml_copy_string(string_buffer));
  }

  CAMLprim value
  jsc_ml_check_syntax(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    JSContextRef context = JSContext_val(ctx);
    JSStringRef js_script =
      JSStringCreateWithUTF8CString(caml_strdup(String_val(js_string)));

    bool is_correct =
      JSCheckScriptSyntax(context, js_script, NULL, 1, NULL);
    CAMLreturn(Val_bool(is_correct));
  }

  CAMLprim value
  jsc_ml_gc(value ctx)
  {
    CAMLparam1(ctx);
    JSGarbageCollect(JSContext_val(ctx));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_get_type(value ctx, value js_value)
  {
    return Val_int(JSValueGetType(JSContext_val(ctx),
				  (JSValueRef)js_value));
  }
}
