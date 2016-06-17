(** A JavaScript execution context. Holds the global object and
    other execution state. *)
type virtual_machine

module Raw_calls = struct
  type js_ptr
  external make_vm :
    unit -> virtual_machine = "jsc_ml_make_vm"
  external evaluate_script :
    virtual_machine -> string -> string = "jsc_ml_eval_script"
  external check_script_syntax :
    virtual_machine -> string -> bool = "jsc_ml_check_syntax"
  external garbage_collect :
    virtual_machine -> unit = "jsc_ml_garbage_collect"
  external make_js_string_with_ml_string :
    string -> js_ptr = "jsc_ml_make_string_with_ml_str"
  external make_js_string : unit -> js_ptr = "jsc_ml_make_string"
  external release_js_string : js_ptr -> unit = "jsc_ml_release_jsc_string"
  external retain_js_string : js_ptr -> unit = "jsc_ml_retain_jsc_string"
  external length_js_string : js_ptr -> int = "jsc_ml_jsc_length"
end

class vm = object
  val vm_ = Raw_calls.make_vm ()
  method evaluate_script src = Raw_calls.evaluate_script vm_ src
  method gc = Raw_calls.garbage_collect vm_
  method check_syntax src = Raw_calls.check_script_syntax vm_ src
end

class virtual js_value = object

  method virtual retain : unit
  method virtual release : unit

end

class js_string ?ml_string () = object

  inherit js_value

  val raw_ptr = match ml_string with
      None -> Raw_calls.make_js_string ()
    | Some s -> Raw_calls.make_js_string_with_ml_string s

  method retain = Raw_calls.retain_js_string raw_ptr

  method release = Raw_calls.release_js_string raw_ptr

  method length = Raw_calls.length_js_string raw_ptr

end
