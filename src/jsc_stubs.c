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

#include <unordered_map>
#include <mutex>
#include <string>
#include <vector>
#include <iostream>

#include <JavaScriptCore/JavaScript.h>

#include "jsc_ml_values.h"

extern "C" {

  static value js_exn;

  CAMLprim value
  set_js_exn(value __attribute__((unused)) unit)
  {
    CAMLparam0();
    js_exn = *caml_named_value("js-exn");
    CAMLreturn(Val_unit);
  }

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
      Store_field(global_context, 0, (value)JSGlobalContextCreate(nullptr));
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
    JSContextGroupRef ctx_group =
      maybe_context_group != Val_none ?
      JSContext_group_val(Some_val(maybe_context_group)) : nullptr;
    JSClassRef class_ref =
      maybe_class != Val_none ? JSClass_val(Some_val(maybe_class)) : nullptr;

    global_context = caml_alloc(sizeof(JSGlobalContextRef), Abstract_tag);
    Store_field(global_context, 0, (value)JSGlobalContextCreateInGroup(ctx_group, class_ref));
    CAMLreturn(global_context);
  }

  CAMLprim value
  jsc_ml_global_context_retain(value global_context)
  {
    CAMLparam1(global_context);
    DEBUG("Retain global context");
    JSGlobalContextRetain(JSContext_val(global_context));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_global_context_release(value global_context)
  {
    CAMLparam1(global_context);
    DEBUG("Release global context");
    JSGlobalContextRelease(JSContext_val(global_context));
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
  jsc_ml_value_of_ml_string(value ctx, value ml_string)
  {
    CAMLparam2(ctx, ml_string);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    auto as_value =
      JSValueMakeString(JSContext_val(ctx),
			JSStringCreateWithUTF8CString(String_val(ml_string)));
    Store_field(wrapper, 0, (value)as_value);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_string_create_with_utf8(value s)
  {
    CAMLparam1(s);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSStringRef), Abstract_tag);
    Store_field(wrapper,
		0,
		(value)JSStringCreateWithUTF8CString(caml_strdup(String_val(s))));
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

  CAMLprim value
  jsc_ml_value_of_js_string(value context, value js_string)
  {
    CAMLparam2(context, js_string);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    Store_field(wrapper, 0, (value)JSValueMakeString(JSContext_val(context),
						     JSString_val(js_string)));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_value_get_type(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    switch (JSValueGetType(JSContext_val(context), JSValue_val(js_value))) {
    case kJSTypeUndefined: CAMLreturn(Val_js_undefined);
    case kJSTypeNull:      CAMLreturn(Val_js_null);
    case kJSTypeBoolean:   CAMLreturn(Val_js_bool);
    case kJSTypeNumber:    CAMLreturn(Val_js_number);
    case kJSTypeString:    CAMLreturn(Val_js_string);
    case kJSTypeObject:    CAMLreturn(Val_js_object);
    }
  }

  CAMLprim value
  jsc_ml_value_is_undefined(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsUndefined(JSContext_val(context),
					   JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_null(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsNull(JSContext_val(context),
				      JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_bool(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsBoolean(JSContext_val(context),
					 JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_number(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsNumber(JSContext_val(context),
					JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_string(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsString(JSContext_val(context),
					JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_object(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsObject(JSContext_val(context),
					JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_array(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsArray(JSContext_val(context),
				       JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_value_is_date(value context, value js_value)
  {
    CAMLparam2(context, js_value);
    CAMLreturn(Val_bool(JSValueIsDate(JSContext_val(context),
				      JSValue_val(js_value))));
  }

  CAMLprim value
  jsc_ml_get_typed_array_type(value context, value js_typed_array)
  {
    CAMLparam2(context, js_typed_array);
    JSValueRef exn;
    switch (JSValueGetTypedArrayType(JSContext_val(context),
				     JSValue_val(js_typed_array),
				     &exn)) {
    case kJSTypedArrayTypeInt8Array:
      CAMLreturn(Val_js_typed_array_Int8Array);
    case kJSTypedArrayTypeInt16Array:
      CAMLreturn(Val_js_typed_array_Int16Array);
    case kJSTypedArrayTypeInt32Array:
      CAMLreturn(Val_js_typed_array_Int32Array);
    case kJSTypedArrayTypeUint8Array:
      CAMLreturn(Val_js_typed_array_Uint8Array);
    case kJSTypedArrayTypeUint8ClampedArray:
      CAMLreturn(Val_js_typed_array_Uint8ClampedArray);
    case kJSTypedArrayTypeUint16Array:
      CAMLreturn(Val_js_typed_array_Uint16Array);
    case kJSTypedArrayTypeUint32Array:
      CAMLreturn(Val_js_typed_array_Uint32Array);
    case kJSTypedArrayTypeFloat32Array:
      CAMLreturn(Val_js_typed_array_Float32Array);
    case kJSTypedArrayTypeFloat64Array:
      CAMLreturn(Val_js_typed_array_Float64Array);
    case kJSTypedArrayTypeArrayBuffer:
      CAMLreturn(Val_js_typed_array_ArrayBuffer);
    case kJSTypedArrayTypeNone:
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context),
						    exn));
    }
  }

  CAMLprim value
  jsc_ml_make_undefined(value context)
  {
    CAMLparam1(context);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    Store_field(wrapper, 0, (value)JSValueMakeUndefined(JSContext_val(context)));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_make_null(value context)
  {
    CAMLparam1(context);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    Store_field(wrapper, 0, (value)JSValueMakeNull(JSContext_val(context)));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_make_bool(value context, value boolean)
  {
    CAMLparam2(context, boolean);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    Store_field(wrapper,
		0,
		(value)JSValueMakeBoolean(JSContext_val(context),
					  Bool_val(boolean)));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_make_number(value context, value num)
  {
    CAMLparam2(context, num);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    Store_field(wrapper,
		0,
		(value)JSValueMakeNumber(JSContext_val(context),
					 Double_val(num)));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_make_from_json_string(value context, value js_string)
  {
    CAMLparam2(context, js_string);
    CAMLlocal1(wrapper);
    auto result = JSValueMakeFromJSONString(JSContext_val(context),
					    JSString_val(js_string));
    if (JSValueIsNull(JSContext_val(context), result))
      caml_raise_with_string(js_exn, "Input string wasn't JSON encoded");
    else {
      wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
      Store_field(wrapper, 0, (value)result);
      CAMLreturn(wrapper);
    }
  }

  CAMLprim value
  jsc_ml_make_json_string_from_value(value context, value js_value, value indent_count)
  {
    CAMLparam3(context, js_value, indent_count);
    CAMLlocal1(wrapper);
    unsigned indent = Int_val(indent_count);
    JSValueRef exn;
    auto result = JSValueCreateJSONString(JSContext_val(context),
					  JSValue_val(js_value),
					  indent,
					  &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    else {
      wrapper = caml_alloc(sizeof(JSStringRef), Abstract_tag);
      Store_field(wrapper, 0, (value)result);
      CAMLreturn(wrapper);
    }
  }

  static std::mutex class_definition_lock;
  static std::unordered_map<std::string, value> class_definition_lookup;
  // will I ever remove it? caml_remove_global_root(value *)

  auto cb_func = [](auto obj, auto spot){
    CAMLparam0();
    CAMLlocal1(ml_cb);
    auto *g =
    static_cast<std::unordered_map<std::string, value>*>(JSObjectGetPrivate(obj));
    {
      std::lock_guard<std::mutex> guard(class_definition_lock);
      const char *pulled = String_val((*g)["class_lookup_key"]);
      ml_cb = Some_val(Field(class_definition_lookup[pulled], spot));
    }
    CAMLreturn(ml_cb);
  };

  // Warning, MASSIVE
  CAMLprim value
  jsc_ml_object_class_create(value
			     /* The record definition of OCaml values */
			     class_def)
  {
    CAMLparam1(class_def);
    CAMLlocal2(wrapper, tuple_wrapper);

    {
      std::lock_guard<std::mutex> guard(class_definition_lock);
      caml_register_global_root(&class_def);
      const char *key_lookup = String_val(Field(class_def, 17));
      class_definition_lookup[key_lookup] = class_def;
    }

    JSClassDefinition starter = kJSClassDefinitionEmpty;
    starter.className = caml_strdup(String_val(Field(class_def, 2)));

    // if (Field(class_def, 1) != Val_emptylist) {
      // TODO Come back to this, class_attributes
    // }
    // if (Field(class_def, 3) != Val_none)
    //   starter.parentClass = JSClass_val(Some_val(Field(class_def, 3)));

    // if (Field(class_def, 4) != Val_emptylist) {
    //   // Come back to this, static_values
    // }
    // if (Field(class_def, 5) != Val_emptylist) {
    //   // Come back to this, static_functions
    // }


    if (Field(class_def, 6) != Val_none) {
      starter.initialize = [](auto ctx, auto object) {
    	CAMLparam0();
    	CAMLlocal3(ml_cb, ctx_w, object_w);
	ml_cb = cb_func(object, 6);
	ctx_w = caml_alloc(sizeof(JSContextRef), Abstract_tag);
	object_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
	Store_field(ctx_w, 0, (value)ctx);
	Store_field(object_w, 0, (value)object);
	caml_callback2(ml_cb, ctx_w, object_w);
    	CAMLreturn0;
      };
    }
    if (Field(class_def, 7) != Val_none) {
      starter.finalize = [](auto obj) {
    	CAMLparam0();
    	CAMLlocal2(ml_cb, obj_w);
    	obj_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    	Store_field(obj_w, 0, (value)obj_w);
    	caml_callback(cb_func(obj, 7), obj_w);
    	CAMLreturn0;
      };
    }
    if (Field(class_def, 8) != Val_none) {
      starter.hasProperty = [](auto ctx, auto obj, auto prop_name) {
    	CAMLparam0();
    	CAMLlocal4(ml_cb, ctx_w, obj_w, prop_name_w);
	ml_cb = cb_func(obj, 8);
    	ctx_w = caml_alloc(sizeof(JSContextRef), Abstract_tag);
    	obj_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    	prop_name_w = caml_alloc(sizeof(JSStringRef), Abstract_tag);
    	Store_field(ctx_w, 0, (value)ctx);
    	Store_field(obj_w, 0, (value)obj);
    	Store_field(prop_name_w, 0, (value)prop_name);
    	bool result =
	Val_bool(caml_callback3(ml_cb, ctx_w, obj_w, prop_name_w));
    	CAMLreturnT(bool, result);
      };
    }

    // if (Field(class_def, 9) != Val_none) {
    //   starter.getProperty = [](auto ctx, auto obj, auto prop_name, auto *exn) {
    // 	CAMLparam0();
    // 	CAMLlocal5(ml_cb, ctx_w, obj_w, prop_name_w, wrapper);
	
    // 	CAMLreturnT(JSValueRef, JSValue_val(wrapper));
    //   };
    // }

    // if (Field(class_def, 13) != Val_none) {
    //   starter.callAsFunction = [](auto ctx,
    // 				  auto fn,
    // 				  auto this_object,
    // 				  auto arg_count,
    // 				  auto args,
    // 				  auto *exn) {
    // 	CAMLparam0();
    // 	CAMLlocal1(t);
    // 	const char *identifier = nullptr;
    // 	{
    // 	  std::lock_guard<std::mutex> guard(class_definition_lock);
    // 	  identifier = static_cast<const char *>(JSObjectGetPrivate(fn));
    // 	}
    // 	printf("Called as function: Identifier: %s\n", identifier);
    // 	CAMLreturnT(JSValueRef, nullptr);
    //   };
    // }

    if (Field(class_def, 14) != Val_none) {
      starter.callAsConstructor = [](auto ctx,
				     auto ctor_object,
				     auto argumentCount,
				     auto arguments[],
				     auto *exn) -> JSObjectRef {
	CAMLparam0();
	CAMLlocal5(ctx_w, ctor_object_w, args_w, wrapper_result, ml_cb);
	// Need to store the exception in exn

	ml_cb = cb_func(ctor_object, 14);
	ctx_w = caml_alloc(sizeof(JSContextRef), Abstract_tag);
	ctor_object_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
	Store_field(ctx_w, 0, (value)ctx);
	Store_field(ctor_object_w, 0, (value)ctor_object);
	// std::vector<JSValueRef> v;
	// for (auto i = 0; i < argumentCount; i++)
	//   v.push_back(arguments[i]);
	// v.push_back(nullptr);
	// std::cout << v.size() << std::endl;
	// args_w = caml_alloc_array([](auto item){
	//     CAMLparam0();
	//     CAMLlocal1(wr);
	//     wr = caml_alloc(sizeof(JSValueRef), Abstract_tag);
	//     Store_field(wr, 0, (value)item);
	//     CAMLreturn(wr);
	//   },
	//   test
	  // (const char **)v.data()
	  // );
	args_w = Atom(0);
	wrapper_result = caml_callback3(ml_cb, ctx_w, ctor_object_w, args_w);
	CAMLreturnT(JSObjectRef, JSObject_val(wrapper_result));
      };
    }

    wrapper = caml_alloc(sizeof(JSClassRef), Abstract_tag);
    tuple_wrapper = caml_alloc_tuple(2);
    Store_field(wrapper, 0, (value)JSClassCreate(&starter));
    // First one is the identifier
    Store_field(tuple_wrapper, 0, Field(class_def, 17));
    // And then the class itself
    Store_field(tuple_wrapper, 1, wrapper);
    CAMLreturn(tuple_wrapper);
  }

  CAMLprim value
  jsc_ml_object_object_make(value maybe_class, value maybe_private_data , value context)
  {
    CAMLparam3(context, maybe_class, maybe_private_data);
    CAMLlocal1(wrapper);
    JSClassRef c =
      maybe_class == Val_none ? nullptr
      : JSClass_val(Field(Some_val(maybe_class), 1));

    // Need to set a unordered_map as the pointer on the object,
    // identifier will be in it it as well as a a private field with
    // pointer to whatever they hide
    auto private_data =
      maybe_class != Val_none ? new std::unordered_map<std::string, value> : nullptr;

    if (maybe_class != Val_none) {
      // Because private_data is on the heap, need to use
      // *private_data, otherwise get an error of error: array
      // subscript is not an integer.
      // caml_register_global_root(&Field(Some_val(maybe_class), 17));
      (*private_data)["class_lookup_key"] = Field(Some_val(maybe_class), 0);
      if (maybe_private_data != Val_none) {
	// caml_register_global_root(&Some_val(maybe_private_data));
	// (*private_data)["private_data"] = &Some_val(maybe_private_data);
      }
    }

    wrapper = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    Store_field(wrapper,
		0,
		(value)JSObjectMake(JSContext_val(context), c, private_data));
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_object_make_function(value create_function_record)
  {
    CAMLparam1(create_function_record);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    auto ctx = JSContext_val(Field(create_function_record, 0));
    auto name =
      Field(create_function_record, 1) != Val_none ?
      JSString_val(Some_val(Field(create_function_record, 1))) : nullptr;
    size_t parameter_count = Wosize_val(Field(create_function_record, 2));
    std::vector<JSStringRef> parameter_names;
    for (size_t i = 0; i < parameter_count; i++)
      parameter_names.push_back(JSString_val(Field(Field(create_function_record, 2), i)));
    auto body = JSString_val(Field(create_function_record, 3));
    auto source_url =
      Field(create_function_record, 4) != Val_none ?
      JSString_val(Some_val(Field(create_function_record, 4))) : nullptr;
    int starting_line = Int_val(Field(create_function_record, 5));

    JSValueRef exception = nullptr;

    auto function_object =
      JSObjectMakeFunction(ctx,
    			   name,
    			   parameter_count,
    			   parameter_count > 0 ? parameter_names.data() : nullptr,
    			   body,
    			   source_url,
    			   starting_line,
    			   &exception);

    if (exception)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(ctx, exception));

    Store_field(wrapper, 0, (value)function_object);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_object_call_as_function(value ctx,
				 value fn,
				 value maybe_this,
				 value args)
  {
    CAMLparam4(ctx, fn, maybe_this, args);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    DEBUG("Calling JS Function");
    JSValueRef exn;
    size_t argc = Wosize_val(args);
    auto this_obj =
      maybe_this != Val_none ? JSObject_val(Some_val(maybe_this)) : nullptr;
    std::vector<JSValueRef> _args_;
    _args_.reserve(argc);
    for (size_t i = 0; i < argc; i++) _args_.push_back(JSValue_val(Field(args, i)));
    auto call_result = JSObjectCallAsFunction(JSContext_val(ctx),
					      JSObject_val(fn),
    					      this_obj,
    					      argc,
    					      argc > 0 ? _args_.data() : nullptr,
    					      &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(ctx), exn));

    Store_field(wrapper, 0, (value)call_result);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_eval_script(value maybe_this,
		     value maybe_source_url,
		     value maybe_starting_line,
		     value context,
		     value script)
  {
    CAMLparam5(maybe_this, maybe_source_url, maybe_starting_line, context, script);
    CAMLlocal1(wrapper);
    auto _this_ =
      maybe_this != Val_none ? JSObject_val(maybe_this) : nullptr;
    auto _src_url_ =
      maybe_source_url != Val_none ? JSString_val(maybe_source_url) : nullptr;
    auto _starting_line_ =
      maybe_starting_line != Val_none ? Int_val(maybe_starting_line) : 1;
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    JSValueRef exn;
    auto result = JSEvaluateScript(JSContext_val(context),
				   JSString_val(script),
				   _this_,
				   _src_url_,
				   _starting_line_,
				   &exn);

    Store_field(wrapper, 0, (value)result);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_object_get_property(value context, value target, value prop_name)
  {
    CAMLparam3(context, target, prop_name);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
    JSValueRef exn;
    auto result =
      JSObjectGetProperty(JSContext_val(context),
			  JSObject_val(target),
			  JSString_val(prop_name),
			  &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    Store_field(wrapper, 0, (value)result);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_object_set_property(value maybe_properties_array,
			     value context,
			     value target,
			     value prop_name,
			     value prop_value)
  {
    CAMLparam5(maybe_properties_array, context, target, prop_name, prop_value);
    JSValueRef exn;
    // TODO need to handle the properties array
    JSObjectSetProperty(JSContext_val(context),
			JSObject_val(target),
			JSString_val(prop_name),
			JSValue_val(prop_value),
			kJSPropertyAttributeNone,
			&exn);
    // Not sure why these explode
    // if (exn)
    //   caml_raise_with_string(js_exn,
    // 			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_object_delete_property(value context, value js_object, value prop_name)
  {
    CAMLparam3(context, js_object, prop_name);
    JSValueRef exn;
    bool result = JSObjectDeleteProperty(JSContext_val(context),
					 JSObject_val(js_object),
					 JSString_val(prop_name),
					 &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    CAMLreturn(Bool_val(result));
  }

  CAMLprim value
  jsc_ml_object_get_property_at_index(value context, value js_object, value index)
  {
    CAMLparam3(context, js_object, index);
    JSValueRef exn;
    CAMLlocal1(wrapper);
    auto result = JSObjectGetPropertyAtIndex(JSContext_val(context),
					     JSObject_val(js_object),
					     Int_val(index),
					     &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    else {
      wrapper = caml_alloc(sizeof(JSValueRef), Abstract_tag);
      Store_field(wrapper, 0, (value)result);
      CAMLreturn(wrapper);
    }
  }

  CAMLprim value
  jsc_ml_object_set_property_at_index(value context,
				      value js_object,
				      value spot,
				      value spot_value)
  {
    CAMLparam4(context, js_object, spot, spot_value);
    CAMLlocal1(wrapper);
    JSValueRef exn;
    JSObjectSetPropertyAtIndex(JSContext_val(context),
			       JSObject_val(js_object),
			       Int_val(spot),
			       JSValue_val(spot_value),
			       &exn);
    if (exn)
      caml_raise_with_string(js_exn,
			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    else
      CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_object_get_private(value obj)
  {
    CAMLparam1(obj);
    auto datum = JSObjectGetPrivate(JSObject_val(obj));
    if (datum == nullptr) CAMLreturn(Val_none);
    else CAMLreturn(Val_some((value)datum));
  }

  CAMLprim value
  jsc_ml_object_set_private(value target, value datum)
  {
    CAMLparam2(target, datum);
    // Need to remember to unregister
    caml_register_global_root(&datum);
    CAMLreturn(Bool_val(JSObjectSetPrivate(JSObject_val(target), (void*)datum)));
  }

  CAMLprim value
  jsc_ml_object_is_function(value context, value obj)
  {
    CAMLparam2(context, obj);
    CAMLreturn(Bool_val(JSObjectIsFunction(JSContext_val(context),
					   JSObject_val(obj))));
  }

  CAMLprim value
  jsc_ml_object_is_constructor(value context, value obj)
  {
    CAMLparam2(context, obj);
    CAMLreturn(Bool_val(JSObjectIsConstructor(JSContext_val(context),
					      JSObject_val(obj))));
  }

  CAMLprim value
  jsc_ml_object_copy_property_names(value context, value obj)
  {
    CAMLparam2(context, obj);
    CAMLlocal1(wrapper);
    wrapper = caml_alloc(sizeof(JSPropertyNameArrayRef), Abstract_tag);
    auto result = JSObjectCopyPropertyNames(JSContext_val(context),
					    JSObject_val(obj));
    Store_field(wrapper, 0, (value)result);
    CAMLreturn(wrapper);
  }

  CAMLprim value
  jsc_ml_property_name_array_retain(value prop_array)
  {
    CAMLparam1(prop_array);
    JSPropertyNameArrayRetain(JSProperty_name_array_val(prop_array));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_property_name_array_release(value prop_array)
  {
    CAMLparam1(prop_array);
    JSPropertyNameArrayRelease(JSProperty_name_array_val(prop_array));
    CAMLreturn(Val_unit);
  }

  CAMLprim value
  jsc_ml_property_name_array_length(value prop_array)
  {
    CAMLparam1(prop_array);
    CAMLreturn(Int_val(JSPropertyNameArrayGetCount(JSProperty_name_array_val(prop_array))));
  }

  CAMLprim value
  jsc_ml_property_name_at_index(value prop_array, value spot)
  {
    CAMLparam2(prop_array, spot);
    CAMLlocal1(wrapper);
    auto name = JSPropertyNameArrayGetNameAtIndex(JSProperty_name_array_val(prop_array),
						  Int_val(spot));
    wrapper = caml_alloc(sizeof(JSStringRef), Abstract_tag);
    Store_field(wrapper, 0, (value)wrapper);
    CAMLreturn(wrapper);
  }
  // CAMLprim value
  // jsc_ml_test_idea(value unit)
  // {
  //   CAMLparam1(unit);
  //   CAMLlocal1(args);

  //   auto argumentCount = 2;

  //   std::vector<std::string> v;

  //   // std::string **arguments = {new std::string("hello"), new std::string("world")};

  //   char *arguments[] = {"hello", "world", nullptr};

  //   // for (auto i = 0; i < argumentCount; i++)
  //   //   v.push_back(arguments[i]);

  //   // v.push_back(nullptr);

  //   args = caml_alloc_array([](auto item){
  //   	CAMLparam0();
  //   	CAMLlocal1(wr);
  // 	// const char *i = ((std::string)item).c_str();
  // 	// char *item = item.c_str();
  // 	wr = caml_copy_string(item);
  //   	// wr = caml_alloc(sizeof(JSValueRef), Abstract_tag);
  //   	// Store_field(wr, 0, (value)item);
  //   	CAMLreturn(wr);
  //     },
  //     (const char**)arguments
  //     // (const char **)v.data()
  //     );

  //   CAMLreturn(args);
  // }

}
