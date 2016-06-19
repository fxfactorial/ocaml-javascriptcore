(** These are high level bindings to JavaScriptcore. The
    virtual_machine object is garbage collected by the OCaml runtime
    and anything that inherits from js_ref will call release when
    garbage collected by the OCaml runtime *)

type class_definition

module Raw_calls = struct
  type js_ptr
  external make_vm :
    unit -> js_ptr = "jsc_ml_make_vm"
  external evaluate_script :
    js_ptr -> string -> string = "jsc_ml_eval_script"
  external check_script_syntax :
    js_ptr -> string -> bool = "jsc_ml_check_syntax"
  external garbage_collect :
    js_ptr -> unit = "jsc_ml_garbage_collect"
  external make_js_string_with_ml_string :
    string -> js_ptr = "jsc_ml_make_string_with_ml_str"
  external release_js_string
    : js_ptr -> unit = "jsc_ml_release_jsc_string" [@@noalloc]
  external retain_js_string
    : js_ptr -> unit = "jsc_ml_retain_jsc_string" [@@noalloc]
  external length_js_string : js_ptr -> int = "jsc_ml_jsc_length"
  external make_jsc_context_group : unit -> js_ptr = "jsc_ml_make_context_group"
  external retain_js_context_group :
    js_ptr -> unit = "jsc_ml_retain_context_group" [@@noalloc]
  external release_js_context_group
    : js_ptr -> unit = "jsc_ml_release_context_group" [@@noalloc]
  external set_vm_context_name : js_ptr -> string -> unit = "jsc_ml_set_context_name"
  external get_vm_context_name : js_ptr -> string = "jsc_ml_get_context_name"
  external make_jsc_js_class : class_definition -> js_ptr = "jsc_ml_make_class"
  external retain_js_class : js_ptr -> unit = "jsc_ml_retain_class"
  external release_js_class : js_ptr -> unit = "jsc_ml_release_class"

end

(** A JavaScript execution context. Holds the global object and
    other execution state. *)
class virtual_machine ?named () = object(self)
  val raw_ptr = Raw_calls.make_vm ()
  initializer
    match named with None -> self#set_name "" | Some s -> self#set_name s
  method evaluate_script src = Raw_calls.evaluate_script raw_ptr src
  method garbage_collect = Raw_calls.garbage_collect raw_ptr
  method check_syntax src = Raw_calls.check_script_syntax raw_ptr src
  method set_name name = Raw_calls.set_vm_context_name raw_ptr name
  method name = Raw_calls.get_vm_context_name raw_ptr
end

class virtual js_ref = object(self)
  val virtual raw_ptr : Raw_calls.js_ptr
  initializer
    Gc.finalise (fun instance -> instance#release) self
  method virtual retain : unit
  method virtual release : unit
end

(** A JavaScript String *)
class js_string ~ml_string = object
  inherit js_ref
  val raw_ptr = Raw_calls.make_js_string_with_ml_string ml_string
  method retain = Raw_calls.retain_js_string raw_ptr
  method release = Raw_calls.release_js_string raw_ptr
  method length = Raw_calls.length_js_string raw_ptr
end

(** A JavaScript Context Group *)
class context_group = object
  inherit js_ref
  val raw_ptr = Raw_calls.make_jsc_context_group ()
  method retain = Raw_calls.retain_js_context_group raw_ptr
  method release = Raw_calls.release_js_context_group raw_ptr
end


(** A JavaScript class *)
class js_class class_def = object
  inherit js_ref

  val raw_ptr = Raw_calls.make_jsc_js_class class_def

  method retain = Raw_calls.retain_js_class raw_ptr
  method release = Raw_calls.release_js_class raw_ptr

end

