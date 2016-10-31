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

CAMLprim value
jsc_ml_context_group_create(value __attribute__((unused)) unit)
{
  CAMLparam0();
  CAMLlocal1(wrapper);
  wrapper = caml_alloc(sizeof(JSContextGroupRef), Abstract_tag);
  Store_field(wrapper, 0, (value)JSContextGroupCreate());
  CAMLreturn(wrapper);
}

CAMLprim value
jsc_ml_context_group_retain(value context_group)
{
  CAMLparam1(context_group);
  JSContextGroupRetain(JSContext_group_val(context_group));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_context_group_release(value context_group)
{
  CAMLparam1(context_group);
  JSContextGroupRelease(JSContext_group_val(context_group));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_global_context_create(value maybe_global_object_class)
{
  CAMLparam1(maybe_global_object_class);
  CAMLlocal1(global_context);
  global_context = caml_alloc(sizeof(JSGlobalContextRef), Abstract_tag);
  if (maybe_global_object_class == Val_none) {
    Store_field(global_context, 0, (value)JSGlobalContextCreate(NULL));
  } else {
    JSClassRef object_class = (JSClassRef)Some_val(maybe_global_object_class);
    Store_field(global_context, 0, (value)JSGlobalContextCreate(object_class));
  }
  CAMLreturn(global_context);
}

CAMLprim value
jsc_ml_global_context_create_in_group(value maybe_context_group, value maybe_class)
{
  CAMLparam2(maybe_context_group, maybe_class);
  CAMLlocal1(global_context);
  JSContextGroupRef ctx_group = NULL;
  JSClassRef class_ref = NULL;

  if (maybe_context_group != Val_none)
    ctx_group = JSContext_group_val(maybe_context_group);
  if (maybe_class != Val_none)
    class_ref = JSClass_val(maybe_class);

  global_context = caml_alloc(sizeof(JSGlobalContextRef), Abstract_tag);
  Store_field(global_context, 0, (value)JSGlobalContextCreateInGroup(ctx_group, class_ref));
  CAMLreturn(global_context);
}

CAMLprim value
jsc_ml_global_context_retain(value global_context)
{
  CAMLparam1(global_context);
  JSGlobalContextRetain(JSGlobal_context_val(global_context));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_global_context_release(value global_context)
{
  CAMLparam1(global_context);
  JSGlobalContextRelease(JSGlobal_context_val(global_context));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_get_global_object(value context)
{
  CAMLparam1(context);
  CAMLlocal1(wrapper);
  wrapper = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
  Store_field(wrapper, 0, (value)JSContextGetGlobalObject(JSContext_val(context)));
  CAMLreturn(wrapper);
}

CAMLprim value
jsc_ml_get_group(value context)
{
  CAMLparam1(context);
  CAMLlocal1(wrapper);
  wrapper = caml_alloc(sizeof(JSContextGroupRef), Abstract_tag);
  Store_field(wrapper, 0, (value)JSContextGetGroup(JSContext_val(context)));
  CAMLreturn(wrapper);
}

CAMLprim value
jsc_ml_string_create_with_chars(value chars, value length)
{
  CAMLparam2(chars, length);
  CAMLlocal1(wrapper);
  wrapper = caml_alloc(sizeof(JSStringRef), Abstract_tag);
  Store_field(wrapper,
	      0,
	      (value)JSStringCreateWithCharacters((const unsigned short *)String_val(chars),
						  Int_val(length)));
  CAMLreturn(wrapper);
}

CAMLprim value
jsc_ml_string_create_with_utf8(value s)
{
  CAMLparam1(s);
  CAMLlocal1(wrapper);
  wrapper = caml_alloc(sizeof(JSStringRef), Abstract_tag);
  Store_field(wrapper, 0, (value)JSStringCreateWithUTF8CString(String_val(s)));
  CAMLreturn(wrapper);
}

CAMLprim value
jsc_ml_string_retain(value js_string)
{
  CAMLparam1(js_string);
  JSStringRetain(JSString_val(js_string));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_string_release(value js_string)
{
  CAMLparam1(js_string);
  JSStringRelease(JSString_val(js_string));
  CAMLreturn(Val_unit);
}

CAMLprim value
jsc_ml_string_length(value js_string)
{
  CAMLparam1(js_string);
  CAMLreturn(Val_int(JSStringGetLength(JSString_val(js_string))));
}

CAMLprim value
jsc_ml_string_max_size(value js_string)
{
  CAMLparam1(js_string);
  CAMLreturn(Val_int(JSStringGetMaximumUTF8CStringSize(JSString_val(js_string))));
}

CAMLprim value
jsc_ml_string_is_equal(value s1, value s2)
{
  CAMLparam2(s1, s2);
  CAMLreturn(Val_bool(JSStringIsEqual(JSString_val(s1), JSString_val(s2))));

}

CAMLprim value
jsc_ml_string_is_equal_utf8(value js_string, value ml_string)
{
  CAMLparam2(js_string, ml_string);
  CAMLreturn(Val_bool(JSStringIsEqualToUTF8CString(JSString_val(js_string),
						   String_val(ml_string))));
}
// CAMLprim value
// jsc_ml_make_vm(value __attribute__((unused)) unit)
// {
//   CAMLparam0();
//   CAMLlocal1(jsc_ml_ctx);
//   DEBUG("Creating a JSC Global Context, aka a Virtual Machine");
//   JSGlobalContextRef ctx = JSGlobalContextCreateInGroup(NULL, NULL);
//   jsc_ml_ctx =
//     caml_alloc_custom(&jsc_context_ops,
// 			sizeof(JSGlobalContextRef), 0, 1);
//   JSVirtual_machine_val(jsc_ml_ctx) = ctx;
//   CAMLreturn(jsc_ml_ctx);
// }

// CAMLprim value
// jsc_ml_eval_script(value ctx, value js_string)
// {
//   CAMLparam2(ctx, js_string);
//   CAMLlocal1(string_buf);
//   DEBUG("Evaling a script");
//   JSValueRef exception;
//   JSGlobalContextRef context = JSVirtual_machine_val(ctx);
//   JSStringRef code = ml_string_to_jsc_string(js_string);
//   JSObjectRef globalObject = JSContextGetGlobalObject(context);
//   JSValueRef js_result =
//     JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

//   JSStringRelease(code);

//   if (js_result) {
//     JSStringRef result_as_string =
// 	JSValueToStringCopy(context, js_result, &exception);
//     string_buf = jsc_string_to_ml(result_as_string);
//     JSStringRelease(result_as_string);
//     CAMLreturn(string_buf);
//   }
//   else {
//     JSStringRef exceptionIString = JSValueToStringCopy(context, exception, NULL);
//     size_t exceptionUTF8Size = JSStringGetMaximumUTF8CStringSize(exceptionIString);
//     char exceptionUTF8[exceptionUTF8Size];
//     JSStringGetUTF8CString(exceptionIString, exceptionUTF8, exceptionUTF8Size);
//     JSStringRelease(exceptionIString);
//     caml_raise_with_string(*caml_named_value("js-exn"), exceptionUTF8);
//   }
// }

// CAMLprim value
// jsc_ml_check_syntax(value ctx, value js_string)
// {
//   CAMLparam2(ctx, js_string);
//   DEBUG("Checking syntax of a script");
//   JSContextRef context = JSVirtual_machine_val(ctx);
//   JSStringRef js_script =
//     JSStringCreateWithUTF8CString(caml_strdup(String_val(js_string)));
//   bool is_correct =
//     JSCheckScriptSyntax(context, js_script, NULL, 1, NULL);
//   JSStringRelease(js_script);
//   CAMLreturn(Val_bool(is_correct));
// }

// CAMLprim value
// jsc_ml_garbage_collect(value ctx)
// {
//   CAMLparam1(ctx);
//   DEBUG("Garbage Collecting");
//   JSGarbageCollect(JSVirtual_machine_val(ctx));
//   CAMLreturn(Val_unit);
// }

// CAMLprim value
// jsc_ml_set_context_name(value context_value, value new_context_name)
// {
//   CAMLparam2(context_value, new_context_name);
//   DEBUG("Setting Context Name");
//   JSGlobalContextSetName(JSVirtual_machine_val(context_value),
// 			   ml_string_to_jsc_string(new_context_name));
//   CAMLreturn(Val_unit);
// }

// CAMLprim value
// jsc_ml_get_context_name(value context_value)
// {
//   CAMLparam1(context_value);
//   DEBUG("Getting Context Name");
//   CAMLreturn(jsc_string_to_ml(JSGlobalContextCopyName(JSVirtual_machine_val(context_value))));
// }
