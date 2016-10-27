type js_ptr

exception JavaScript_eval of string

let () = Callback.register_exception "js-exn" (JavaScript_eval "")

external make_vm : unit -> js_ptr = "jsc_ml_make_vm"
external evaluate_script : js_ptr -> string -> string = "jsc_ml_eval_script"
external check_script_syntax : js_ptr -> string -> bool = "jsc_ml_check_syntax"
external garbage_collect : js_ptr -> unit = "jsc_ml_garbage_collect"
external set_vm_context_name : js_ptr -> string -> unit = "jsc_ml_set_context_name"
external get_vm_context_name : js_ptr -> string = "jsc_ml_get_context_name"

class virtual_machine ?named () = object(self)
  val raw_ptr = make_vm ()
  initializer
    match named with None -> self#set_name "" | Some s -> self#set_name s

  method evaluate_script src = evaluate_script raw_ptr src
  method garbage_collect = garbage_collect raw_ptr
  method check_syntax src = check_script_syntax raw_ptr src
  method set_name name = set_vm_context_name raw_ptr name
  method name = get_vm_context_name raw_ptr
end
