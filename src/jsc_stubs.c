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

  static std::mutex class_definition_lock;
  static std::unordered_map<std::string, value> class_definition_lookup;

  // will I ever remove it? caml_remove_global_root(value *)

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
    // if (Field(class_def, 1) != Val_emptylist) {
    //   // TODO Come back to this, class_attributes
    // }
    starter.className = caml_strdup(String_val(Field(class_def, 2)));
    // if (Field(class_def, 3) != Val_none)
    //   starter.parentClass = JSClass_val(Some_val(Field(class_def, 3)));

    // if (Field(class_def, 4) != Val_emptylist) {
    //   // Come back to this, static_values
    // }
    // if (Field(class_def, 5) != Val_emptylist) {
    //   // Come back to this, static_functions
    // }
    // if (Field(class_def, 6) != Val_none) {
    //   starter.initialize = [](auto ctx, auto object) {
    // 	CAMLparam0();
    // 	CAMLlocal3(t, ctx_w, object_w);
    // 	const char *identifier = nullptr;
    // 	{
    // 	  std::lock_guard<std::mutex> guard(class_definition_lock);
    // 	  identifier = static_cast<const char *>(JSObjectGetPrivate(object));
    // 	}
    // 	if (identifier) {
    // 	  t = class_definition_lookup[identifier];
    // 	  ctx_w = caml_alloc(sizeof(JSContextRef), Abstract_tag);
    // 	  object_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    // 	  Store_field(ctx_w, 0, (value)ctx);
    // 	  Store_field(object_w, 0, (value)object);
    // 	  caml_callback2(Some_val(Field(t, 6)), ctx_w, object_w);
    // 	} else {
    // 	  printf("Should be impossible\n");
    // 	}
    // 	CAMLreturn0;
    //   };
    // }
    // if (Field(class_def, 7) != Val_none) {
    //   starter.finalize = [](auto obj) {
    // 	CAMLparam0();
    // 	CAMLlocal2(t, obj_w);
    // 	const char *identifier = nullptr;
    // 	{
    // 	  std::lock_guard<std::mutex> guard(class_definition_lock);
    // 	  identifier = static_cast<const char *>(JSObjectGetPrivate(obj));
    // 	}
    // 	t = class_definition_lookup[identifier];
    // 	obj_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    // 	Store_field(obj_w, 0, (value)obj_w);
    // 	caml_callback(Some_val(Field(t, 7)), obj_w);
    // 	CAMLreturn0;
    //   };
    // }
    // if (Field(class_def, 8) != Val_none) {
    //   starter.hasProperty = [](auto ctx, auto obj, auto prop_name) {
    // 	CAMLparam0();
    // 	CAMLlocal4(t, ctx_w, obj_w, prop_name_w);
    // 	const char *identifier = nullptr;
    // 	{
    // 	  std::lock_guard<std::mutex> guard(class_definition_lock);
    // 	  identifier = static_cast<const char *>(JSObjectGetPrivate(obj));
    // 	}
    // 	t = class_definition_lookup[identifier];
    // 	ctx_w = caml_alloc(sizeof(JSContextRef), Abstract_tag);
    // 	obj_w = caml_alloc(sizeof(JSObjectRef), Abstract_tag);
    // 	prop_name_w = caml_alloc(sizeof(JSStringRef), Abstract_tag);
    // 	Store_field(ctx_w, 0, (value)ctx);
    // 	Store_field(obj_w, 0, (value)obj);
    // 	Store_field(prop_name_w, 0, (value)prop_name);
    // 	bool result = Val_bool(caml_callback3(Some_val(Field(t, 8)),
    // 					      ctx_w,
    // 					      obj_w,
    // 					      prop_name_w));
    // 	CAMLreturnT(bool, result);
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
	auto *g =
	static_cast<std::unordered_map<std::string, value>*>(JSObjectGetPrivate(ctor_object));

	{
	  std::lock_guard<std::mutex> guard(class_definition_lock);
	  const char *pulled = String_val((*g)["class_lookup_key"]);
	  ml_cb = Some_val(Field(class_definition_lookup[pulled], 14));
	}

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

	// CAMLreturnT(JSObjectRef, JSObject_val(wrapper_result));
	// caml_callback2(ml_cb, ctx_w, ctor_object_w);
	// auto r = JSObjectMake(ctx, nullptr, nullptr);
	// CAMLreturnT(JSObjectRef, r);
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
    auto parameter_count = Wosize_val(Field(create_function_record, 2));
    std::vector<JSStringRef> parameter_names;
    for (auto i = 0; i < parameter_count; i++)
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
    auto argc = Wosize_val(args);
    auto this_obj =
      maybe_this != Val_none ? JSObject_val(Some_val(maybe_this)) : nullptr;
    std::vector<JSValueRef> _args_;
    _args_.reserve(argc);
    for (auto i = 0; i < argc; i++) _args_.push_back(JSValue_val(Field(args, i)));
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
    // if (exn) {
    //   auto g = JSContext_val(context);
      // int caml_page_table_lookup(void *);
      // std::cout << caml_page_table_lookup(context) << std::endl;
      // std::cout << g << std::endl;
      // std::cout << exn << std::endl;
      // auto t = JSValueGetType(JSContext_val(context), exn);
      // auto as_s = JSValueToStringCopy(JSContext_val(context), exn, nullptr);
      // caml_raise_with_string(js_exn,
      // 			     jsvalue_to_utf8_string(JSContext_val(context), exn));
    // }

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
