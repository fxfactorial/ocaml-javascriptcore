/* -*- c++ -*- */
#define CAML_NAME_SPACE
// OCaml declarations
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/fail.h>

#include <iostream>
#include <string>

#include <JavaScriptCore/JavaScript.h>

extern "C" {

  CAMLprim value jsc_ml_make_context(value __attribute__((unused)) unit)
  {
    return (value)JSGlobalContextCreateInGroup(NULL, NULL);
  }

  CAMLprim value jsc_ml_eval_script(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    const char *const result = caml_strdup(String_val(js_string));

    JSGlobalContextRef context = (JSGlobalContextRef)ctx;

    JSStringRef code = JSStringCreateWithUTF8CString(result);

    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    JSValueRef exception;
    JSValueRef js_result =
      JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

    JSStringRef result_as_string =
      JSValueToStringCopy(context, js_result, &exception);

    size_t string_len = JSStringGetMaximumUTF8CStringSize(result_as_string);
    char *string_buffer = (char*)malloc(string_len);
    JSStringGetUTF8CString(result_as_string, string_buffer, string_len);
    JSStringRelease(code);
    CAMLreturn(caml_copy_string(string_buffer));
  }

  CAMLprim value jsc_ml_check_syntax(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
    JSContextRef context = (JSContextRef)ctx;
    JSStringRef js_script =
      JSStringCreateWithUTF8CString(caml_strdup(String_val(js_string)));

    bool is_correct = JSCheckScriptSyntax(context, js_script, NULL, 1, NULL);
    CAMLreturn(Val_bool(is_correct));
  }

  CAMLprim value jsc_ml_gc(value ctx)
  {
    CAMLparam1(ctx);
    JSGarbageCollect((JSContextRef)ctx);
    CAMLreturn(Val_unit);
  }

  CAMLprim value jsc_ml_get_type(value ctx, value js_value)
  {
    return Val_int(JSValueGetType((JSContextRef)ctx, (JSValueRef)js_value));
  }

}
