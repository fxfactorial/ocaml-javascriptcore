(* type js_ptr *)

exception JavaScript_exception of string

let () = Callback.register_exception "js-exn" (JavaScript_exception "")

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
  external create_context_group : unit -> js_context_group = "jsc_ml_context_group_create"
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
  type t = Undefined | Null | Bool | Number | String | Object
  (* external get_type : js_context -> js_value -> t = "jsc_ml_value_get_type" *)
  (* external is_undefined : js_context -> js_value -> bool = *)
  (*   "jsc_ml_value_is_undefined" *)
  (* external is_null : js_context -> js_value -> bool = "jsc_ml_value_is_null" *)
  (* external is_bool : js_context -> js_value -> bool = "jsc_ml_value_is_bool" *)
  (* external is_number : js_context -> js_value -> bool = "jsc_ml_value_is_number" *)
  (* external is_string : js_context -> js_value -> bool = "jsc_ml_value_is_string" *)
  (* external is_object : js_context -> js_value -> bool = "jsc_ml_value_is_object" *)
  (* external is_object_of_class : js_context -> js_value -> js_class -> bool = *)
  (*   "jsc_ml_value_is_object_of_class" *)
  (* external is_equal : js_context -> js_value -> js_value -> bool = *)
  (*   "jsc_ml_value_is_equal" *)
  (* external is_strict_equal : js_context -> js_value -> js_value -> bool = *)
  (*   "jsc_ml_value_is_strict_equal" *)

  (* Stopped at Is Instance of Constructor *)
end

module Object = struct
  type property_attribute = None | Read_only | Don't_enum | Don't_delete
  type class_attribute = None | No_automatic_prototype
  type initializer_cb = js_context -> js_object -> unit
  type finalizer_cb = js_object -> unit
  type has_property_cb = js_context -> js_object -> js_string -> bool
  type get_property_cb_exn = js_context -> js_object -> js_string -> js_value
  type set_property_cb_exn = js_context -> js_object -> js_string -> js_value -> bool
  type delete_property_cb_exn = js_context -> js_object -> js_string -> bool
  type get_property_names_cb =
    js_context -> js_object -> js_property_name_accumulator -> unit
  type call_as_function_cb_exn =
    js_context -> js_object -> js_object -> int -> js_value list -> js_value
  type call_as_constructor_cb_exn =
      js_context -> js_object -> int -> js_value list -> js_object
  type has_instance_cb_exn =
    js_context -> js_object -> js_value -> bool
  type convert_to_type_cb_exn =
    js_context -> js_object -> JSValue.t -> js_value
  type static_value = {
    name : string;
    get_prop_cb_exn : get_property_cb_exn;
    set_prop_cb_exn : set_property_cb_exn;
    attributes : property_attribute list
  }
  type static_function = {
    name : string;
    call_as_function : call_as_function_cb_exn;
    attributes : property_attribute list
  }
  type class_definition = {
    (* 0 *) version : int;
    (* 1 *) mutable attributes : class_attribute list;
    (* 2 *) mutable class_name : string;
    (* 3 *) mutable parent_class : js_class option;
    (* 4 *) mutable static_values : static_value list;
    (* 5 *) mutable static_functions : static_function list;
    (* 6 *) mutable init : initializer_cb option;
    (* 7 *) mutable finalizer : finalizer_cb option;
    (* 8 *) mutable has_property : has_property_cb option;
    (* 9 *) mutable get_property : get_property_cb_exn option;
    (* 10 *) mutable set_property : set_property_cb_exn option;
    (* 11 *) mutable delete_property : delete_property_cb_exn option;
    (* 12 *) mutable get_property_names : get_property_names_cb option;
    (* 13 *) mutable call_as_function : call_as_function_cb_exn option;
    (* 14 *) mutable call_as_constructor : call_as_constructor_cb_exn option;
    (* 15 *) mutable has_instance : has_instance_cb_exn option;
    (* 16 *) mutable convert_to_type : convert_to_type_cb_exn option;
    (* 17 *) identifier : string;
  }
  let class_definition_empty () = {
    version = 0; attributes = []; class_name = ""; parent_class = None;
    static_values = []; static_functions = []; init = None;
    finalizer = None; has_property = None; get_property = None;
    set_property = None; delete_property = None; get_property_names = None;
    call_as_function = None; call_as_constructor = None; has_instance = None;
    convert_to_type = None; identifier = Uuidm.(create `V4 |> to_string)
  }
  external class_create : class_definition -> js_class = "jsc_ml_object_class_create"
  (* external class_retain : js_class -> unit = "jsc_ml_object_class_retain" [@@noalloc] *)
  (* external class_release : js_class -> unit = "jsc_ml_object_class_release" [@@noalloc] *)
  external object_make : js_context -> js_class option -> 'string option -> js_object =
    "jsc_ml_object_object_make"
  (* external object_make_with_callback : *)
  (*   js_context -> js_string option -> call_as_function_cb_exn -> js_object = *)
  (*   "jsc_ml_object_make_with_callback" *)
  (* external object_make_constructor : *)
  (*   js_context -> js_class option -> call_as_constructor_cb_exn option -> js_object = *)
  (*   "jsc_ml_object_make_constructor" *)
  (* external object_make_array_exn : js_context -> int -> js_value list -> js_object = *)
  (*   "jsc_ml_object_make_array" *)
  (* external object_make_date_exn : js_context -> int -> js_value list -> js_object = *)
  (*   "jsc_ml_object_make_date" *)
  (* external object_make_error_exn : js_context -> int -> js_value list -> js_object = *)
  (*   "jsc_ml_object_make_error" *)
  (* external object_make_regexp_exn : js_context -> int -> js_value list -> js_object = *)
  (*   "jsc_ml_object_make_regexp" *)
  (* external object_make_function_exn : *)
  (*   js_context -> js_string option -> int -> *)
  (*   js_string list -> js_string -> js_string option -> int -> js_object = *)
  (*   "jsc_ml_object_make_function_byte" *)
  (*   "jsc_ml_object_make_function_native" *)
  (* external object_get_prototype : js_context -> js_object -> js_value = *)
  (*   "jsc_ml_object_get_prototype" *)
  (* external object_set_prototype : js_context -> js_object -> js_value -> unit = *)
  (*   "jsc_ml_object_set_prototype" *)
  (* external object_has_property : js_context -> js_object -> js_string -> bool = *)
  (*   "jsc_ml_object_has_property" *)
  (* external object_get_property_exn : js_context -> js_object -> js_string -> js_value = *)
  (*   "jsc_ml_object_get_property" *)
  (* external object_set_property_exn : *)
  (*   js_context -> js_object -> js_string -> js_value -> property_attribute list -> unit = *)
  (*   "jsc_ml_object_set_property" *)
  (* external object_delete_property_exn : *)
  (*   js_context -> js_object -> js_string -> bool = "jsc_ml_object_delete_property" *)
  (* external object_get_property_at_index_exn : *)
  (*   js_context -> js_object -> int -> js_value = *)
  (*   "jsc_ml_object_get_property_at_index" *)
  (* external object_set_property_at_index_exn : *)
  (*   js_context -> js_object -> int -> js_value -> unit = *)
  (*   "jsc_ml_object_set_property_at_index" *)
  (* external object_get_private_data : *)
  (*   js_object -> 'any_private_data option = "jsc_ml_object_get_private" *)
  (* external object_set_private_data : *)
  (*   js_object -> 'any_private_data -> bool = "jsc_ml_object_set_private" *)
  (* external object_is_function : *)
  (*   js_context -> js_object -> bool = "jsc_ml_object_is_function" *)
  (* external object_call_as_function_exn : *)
  (*   js_context -> js_object -> this:js_object option -> int -> js_value list -> js_value = *)
  (*   "jsc_ml_object_call_as_function" *)
  (* external object_is_constructor : js_context -> js_object -> bool = *)
  (*   "jsc_ml_object_is_constructor" *)
  (* external object_call_as_constructor_exn : *)
  (*   js_context -> js_object -> int -> js_value list -> js_object = *)
  (*   "jsc_ml_object_call_as_constructor" *)
  (* external object_copy_property_names : *)
  (*   js_context -> js_object -> js_property_name_array = *)
  (*   "jsc_ml_object_copy_property_names" *)
  (* external property_name_array_retain : *)
  (*   js_property_name_array -> unit = "jsc_ml_property_name_array_retain" *)
  (* external property_name_array_release : *)
  (*   js_property_name_array -> unit = "jsc_ml_property_name_array_release" *)
  (* external property_name_length : js_property_name_array -> int = *)
  (*   "jsc_ml_property_name_array_length" *)
  (* external property_name_at_index : *)
  (*   js_property_name_array -> int -> js_string = *)
  (*   "jsc_ml_property_name_at_index" *)
  (* external property_name_accumulator_add_name : *)
  (*   js_property_name_accumulator -> js_string -> unit = *)
  (*   "jsc_ml_name_accumulator_add_name" *)
end


module String = struct
  external create_with_chars : string -> int -> js_string =
    "jsc_ml_string_create_with_chars"
  external create_with_utf8 : string -> js_string = "jsc_ml_string_create_with_utf8"
  external retain : js_string -> unit = "jsc_ml_string_retain" [@@noalloc]
  external release : js_string -> unit = "jsc_ml_string_release" [@@noalloc]
  external length : js_string -> int = "jsc_ml_string_length" [@@noalloc]
  external maximum_utf8_string_length : js_string -> int = "jsc_ml_string_max_size" [@@noalloc]
  (* external get_utf8_string : js_string -> string -> int -> int = *)
  (*   "jsc_ml_string_get_utf8_string" *)
  external is_equal : js_string -> js_string -> bool = "jsc_ml_string_is_equal"
  external is_equal_to_utf8_string : js_string -> string -> bool =
    "jsc_ml_string_is_equal_utf8"
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
