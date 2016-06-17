/*
 * jsc_ml_values.h
 * -----------
 * Copyright : (c) 2016, Edgar Aroutiounian <edgar.factorial@gmail.com>
 * Licence   : BSD3
 *
 * This file is a part of ocaml-javascriptcore.
 */

#ifndef JSC_ML_VALUES_H
#define JSC_ML_VALUES_H

#ifndef _DEBUG
#define DEBUG(s)
#else
#include <iostream>
#include <string>
#include <time.h>

const std::string current_date_time();
// http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
// TrungTN's was easiest to use
#define DEBUG(s)				\
  std::cout << "\033[1;33m["			\
  << current_date_time ()			\
  << "]\036" << " \033[1;36m["			\
  <<  __PRETTY_FUNCTION__ << "]\033[0m: "	\
  << s << "\n"
#endif

#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/memory.h>
#include <caml/custom.h>
#include <caml/fail.h>

#include <JavaScriptCore/JavaScript.h>

#define JSVirtual_machine_val(v)		\
  (*((JSGlobalContextRef*)Data_custom_val(v)))

#define JSString_val(v) (JSStringRef)Field(v, 0)

extern struct custom_operations jsc_context_ops;
value jsc_string_to_ml(JSStringRef);
JSStringRef ml_string_to_jsc_string(value);

#endif
