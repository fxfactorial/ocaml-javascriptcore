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

#ifndef _DEBUG
#define DEBUG(s)
#else
#include <time.h>
// http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// TrungTN's was easiest to use
const std::string current_date_time()
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
#define DEBUG(s)						\
  std::cout << "\033[1;33m[" \
  << current_date_time () \
  << "]\036" << " \033[1;36m[" \
  <<  __PRETTY_FUNCTION__ << "]\033[0m : " \
  << s << "\n"
#endif

#define JSContext_group_val(v) \
  (*((JSContextGroupRef*)Data_custom_val(v)))

#define JSVirtual_machine_val(v) (*((JSGlobalContextRef*)Data_custom_val(v)))

#define JSString_val(v) (*((JSStringRef*)Data_custom_val(v)))

#define JSClass_val(v) (*((JSClassRef*)Data_custom_val(v)))

#define JSProperty_name_array_val(v) \
  (*((JSPropertyNameArrayRef*)Data_custom_val(v)))

#define JSProperty_name_accumulator_val(v) \
  (*((JSPropertyNameAccumulatorRef*)Data_custom_val(v)))

#define JSValue_val(v) (*((JSValueRef*)Data_custom_val(v)))

#define JSObject_val(v) (*((JSObjectRef*)Data_custom_val(v)))

static void
jsc_ml_vm_finalize(value ctx)
{
  return JSGlobalContextRelease(JSVirtual_machine_val(ctx));
}

static struct
custom_operations jsc_context_ops = {
  (char*)"jsc.vm",
  jsc_ml_vm_finalize,
  custom_compare_default,
  custom_hash_default,
  custom_serialize_default,
  custom_deserialize_default,
  custom_compare_ext_default
};

// static void
// jsc_ml_context_group_finalize(value ctx)
// {
//   return JSContextGroupRelease(JSContext_group_val(ctx));
// }

static value jsc_string_to_ml(JSStringRef str)
{
  size_t string_len = JSStringGetMaximumUTF8CStringSize(str);
  char string_buffer[string_len];
  JSStringGetUTF8CString(str, string_buffer, string_len);
  return caml_copy_string(string_buffer);
 }

extern "C" {

  CAMLprim value
  jsc_ml_eval_script(value ctx, value js_string)
  {
    DEBUG("Evaling a script");
    CAMLparam2(ctx, js_string);
    CAMLlocal1(string_buf);
    char *const result = caml_strdup(String_val(js_string));

    JSGlobalContextRef context = JSVirtual_machine_val(ctx);

    JSStringRef code = JSStringCreateWithUTF8CString(result);

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
    free(result);
    CAMLreturn(string_buf);
  }

  CAMLprim value
  jsc_ml_check_syntax(value ctx, value js_string)
  {
    CAMLparam2(ctx, js_string);
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
    JSGarbageCollect(JSVirtual_machine_val(ctx));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_create_with_chars(value big_array_uint8_t)
  {
    CAMLparam1(big_array_uint8_t);

    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_create_with_null_string(value ml_string)
  {
    CAMLparam1(ml_string);
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_string_length(value js_string)
  {
    CAMLparam1(js_string);
    CAMLreturn(Val_long(JSString_val(js_string)));
  }

  CAMLprim value
  jsc_ml_string_is_equal(value a, value b)
  {
    CAMLparam2(a, b);
    CAMLreturn(Val_bool(JSStringIsEqual(JSString_val(a),
					JSString_val(b))));
  }

  CAMLprim value
  jsc_ml_to_ml_string(value js_string)
  {
    CAMLparam1(js_string);
    CAMLreturn(jsc_string_to_ml(JSString_val(js_string)));
  }

  // CAMLprim value
  // jsc_ml_create_context_group(value __attribute__((unused)) unit)
  // {
  //   CAMLparam0();
  //   CAMLlocal1(jsc_ml_ctx_grp);
  //   JSContextGroupRef ctx_grp = JSContextGroupCreate();

  //   jsc_ml_ctx_grp =
  //     caml_alloc_custom(&jsc_context_ops,
  // 			sizeof(JSGlobalContextRef), 0, 1);
  //   JSContext_group_val(jsc_ml_ctx_grp) = ctx_grp;
  //   CAMLreturn(jsc_ml_ctx_grp);
  // }

  CAMLprim value
  jsc_ml_make_vm(value __attribute__((unused)) unit)
  {
    CAMLparam0();
    CAMLlocal1(jsc_ml_ctx);
    JSGlobalContextRef ctx = JSGlobalContextCreateInGroup(NULL, NULL);

    jsc_ml_ctx =
      caml_alloc_custom(&jsc_context_ops,
			sizeof(JSGlobalContextRef), 0, 1);
    JSVirtual_machine_val(jsc_ml_ctx) = ctx;
    CAMLreturn(jsc_ml_ctx);
  }

  // CAMLprim value
  // jsc_ml_create_context_in_group(value ctx_grp, value js_class,
  // 				 value __attribute__((unused)) unit)
  // {
  //   CAMLparam2(ctx_grp, js_class);
  //   CAMLreturn(Val_unit);
  // }

  // CAMLprim value
  // jsc_ml_get_global(value ctx)
  // {
  //   CAMLparam1(ctx);
  //   CAMLreturn(Val_unit);
  // }

  // CAMLprim value
  // jsc_ml_get_context_group(value ctx)
  // {
  //   CAMLparam1(ctx);
  //   CAMLreturn(Val_unit);
  // }

  // CAMLprim value
  // jsc_ml_get_context_name(value ctx)
  // {
  //   CAMLparam1(ctx);
  //   CAMLreturn(Val_unit);
  // }

  // CAMLprim value
  // jsc_ml_set_context_name(value ctx, value new_name)
  // {
  //   CAMLparam2(ctx, new_name);
  //   CAMLreturn(Val_unit);
  // }

  // CAMLprim value
  // jsc_ml_get_type(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_int(JSValueGetType(JSContext_val(ctx),
  // 				      JSValue_val(js_value))));
  // }

  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSContext_val(ctx),
  // 					   JSValue_val(js_value))));
  // }

  // CAMLprim value
  // jsc_ml_is_null(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsNull(JSContext_val(ctx),
  // 				      JSValue_val(js_value))));
  // }

  // CAMLprim value
  // jsc_ml_is_boolean(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsBoolean(JSVirtual_machine_val(ctx),
  // 					 JSValue_val(js_value))));
  // }

  // CAMLprim value
  // jsc_ml_is_number(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsNumber(JSVirtual_machine_val(ctx),
  // 					JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_string(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsString(JSVirtual_machine_val(ctx),
  // 					JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_object(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsObject(JSVirtual_machine_val(ctx),
  // 					JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_object_of_class(value ctx, value js_value, value js_class)
  // {
  //   CAMLparam3(ctx, js_value, js_class);
  //   CAMLreturn(Val_bool(JSValueIsObjectOfClass(JSVirtual_machine_val(ctx),
  // 					       JSValue_val(js_value),
  // 					       JSClass_val(js_class))));
  // }

  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }
  // CAMLprim value
  // jsc_ml_is_undefined(value ctx, value js_value)
  // {
  //   CAMLparam2(ctx, js_value);
  //   CAMLreturn(Val_bool(JSValueIsUndefined(JSVirtual_machine_val(ctx),
  // 					   JSValue_val(js_value))));
  // }

}
