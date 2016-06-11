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

// Maybe good usage of C++ for abstracting this hassle.
// class JSVM {

// };


extern "C" {

  CAMLprim value exec_js(value ctx, value js_string)
  {
    const char *const result = caml_strdup(String_val(js_string));

    JSGlobalContextRef context = (JSGlobalContextRef)ctx;

    JSStringRef code = JSStringCreateWithUTF8CString(result);

    JSObjectRef globalObject = JSContextGetGlobalObject(context);
    JSValueRef exception;
    JSValueRef js_result = JSEvaluateScript(context, code, globalObject, NULL, 1, &exception);

    JSStringRef result_as_string = JSValueToStringCopy(context, js_result, &exception);
    size_t string_len = JSStringGetMaximumUTF8CStringSize(result_as_string);
    char *string_buffer = (char*)malloc(string_len);
    JSStringGetUTF8CString(result_as_string, string_buffer, string_len);
    JSStringRelease(code);
    return caml_copy_string(string_buffer);
  }

  CAMLprim value create_js_context_ml(__unused value unit)
  {
    return (value)JSGlobalContextCreateInGroup(NULL, NULL);
  }

}
