/* -*- objc -*- */
#include <iostream>
#include <string>
#include <Foundation/Foundation.h>

#include <caml/mlvalues.h>

extern "C" {

  CAMLprim value speak(__unused value unit)
  {
    std::cout << "Do speak!\n";
    return Val_unit;
  }
}
