type js_ptr

exception JavaScript_eval of string

let () = Callback.register_exception "js-exn" (JavaScript_eval "")

(* external make_vm : unit -> js_ptr = "jsc_ml_make_vm" *)
(* external evaluate_script : js_ptr -> string -> string = "jsc_ml_eval_script" *)
(* external check_script_syntax : js_ptr -> string -> bool = "jsc_ml_check_syntax" *)
(* external garbage_collect : js_ptr -> unit = "jsc_ml_garbage_collect" *)
(* external set_vm_context_name : js_ptr -> string -> unit = "jsc_ml_set_context_name" *)
(* external get_vm_context_name : js_ptr -> string = "jsc_ml_get_context_name" *)

type js_context_group
type js_context
type js_global_context = js_context
type js_string
type js_class
type js_property_name_array
type js_property_name_accumulator
type js_value
type js_object = js_value

module Context = struct
  external create : unit -> js_context_group = "jsc_ml_context_group_create"
  external retain : js_context_group -> unit = "jsc_ml_context_group_retain" [@@noalloc]
  external release : js_context_group -> unit = "jsc_ml_context_group_release" [@@noalloc]
  external global_context_create :
    js_class option -> js_global_context = "jsc_ml_global_context_create"
  external global_context_create_in_group :
    js_context_group option -> js_class option -> js_global_context
    = "jsc_ml_global_context_create_in_group"
  external global_context_retain :
    js_global_context -> unit = "jsc_ml_global_context_retain" [@@noalloc]
  external global_context_release :
    js_global_context -> unit = "jsc_ml_global_context_release" [@@noalloc]

  external global_context_get_global_object :
    js_context -> js_object = "jsc_ml_get_global_object"

  external get_group : js_context -> js_context_group = "jsc_ml_get_group"
end

module JSValue = struct

end



(* class virtual_machine ?named () = object(self) *)
(*   val raw_ptr = make_vm () *)
(*   initializer *)
(*     match named with None -> self#set_name "" | Some s -> self#set_name s *)

(*   method evaluate_script src = evaluate_script raw_ptr src *)
(*   method garbage_collect = garbage_collect raw_ptr *)
(*   method check_syntax src = check_script_syntax raw_ptr src *)
(*   method set_name name = set_vm_context_name raw_ptr name *)
(*   method name = get_vm_context_name raw_ptr *)
(* end *)
