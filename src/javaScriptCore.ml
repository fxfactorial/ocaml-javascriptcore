module Base = struct

  type js_context_group_ref

  type js_context_ref

  type js_global_context_ref

  type js_string_ref

  type js_class_ref

  type js_property_name_array_ref

  type js_property_name_accumulator_ref

  type js_value_ref

  type js_object_ref

  (* external evaluate_script_exn : *)
  (*   context:js_context_ref -> *)
  (*   script:js_string_ref -> *)
  (*   this:js_object_ref option -> *)
  (*   source_url:string option -> *)
  (*   starting_line_number:int -> *)
  (*   js_value_ref = "jsc_ml_evaluate_script" *)

  (* external check_script_syntax_exn : *)
  (*   context:js_context_ref -> *)
  (*   script:js_string_ref -> *)
  (*   source_url:string option -> *)
  (*   starting_line_number:int -> *)
  (*   js_value_ref = "jsc_ml_check_script_syntax" *)

  (* external garbage_collect : *)
  (*   js_context_ref -> unit = "jsc_ml_garbage_collect" *)

end

module Value = struct

  open Base
  type js_t = Undefined | Null | Boolean | Number | String | Object

  (* external get_type : *)
  (*   js_context_ref -> js_value_ref -> js_t = "jsc_ml_get_type" *)

  (* external is_undefined : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_undefined" *)

  (* external is_null : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_null" *)

  (* external is_boolean : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_boolean" *)

  (* external is_number : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_number" *)

  (* external is_string : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_string" *)

  (* external is_object : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_object" *)

  (* external is_object_of_class : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_is_object_of_class" *)

  (* external is_equal_exn : *)
  (*   js_context_ref -> js_value_ref -> js_value_ref -> bool = "jsc_ml_is_equal" *)

  (* (\* === *\) *)
  (* external is_strict_equal : *)
  (*   js_context_ref -> js_value_ref -> js_value_ref -> bool *)
  (*   = "jsc_ml_is_strict_equal" *)

  (* external is_instance_of_constructor : *)
  (*   js_context_ref -> js_value_ref -> js_object_ref -> bool *)
  (*   = "jsc_ml_is_instance_of_constructor" *)

  (* external make_undefined : js_context_ref -> js_value_ref *)
  (*   = "jsc_ml_make_undefined" *)

  (* external make_null : js_context_ref -> js_value_ref = "jsc_ml_make_null" *)

  (* external make_boolean : *)
  (*   js_context_ref -> bool -> js_value_ref = "jsc_ml_make_boolean" *)

  (* external make_number : *)
  (*   js_context_ref -> float -> js_value_ref = "jsc_ml_make_number" *)

  (* external make_from_json_string : *)
  (*   js_context_ref -> js_string_ref -> js_value_ref *)
  (*   = "jsc_ml_make_from_json_string" *)

  (* external create_json_string : *)
  (*   js_context_ref -> js_value_ref -> indent:int -> js_string_ref *)
  (*   = "jsc_ml_create_json_string" *)

  (* external to_boolean : *)
  (*   js_context_ref -> js_value_ref -> bool = "jsc_ml_to_boolean" *)

  (* external to_number_exn : *)
  (*   js_context_ref -> js_value_ref -> float = "jsc_ml_to_number" *)

  (* Converts a JavaScript value to string and copies the result into
     a JavaScript string. *)
  (* external to_string_copy : *)
  (*   js_context_ref -> js_value_ref -> js_string_ref *)
  (*   = "jsc_ml_to_string_copy" *)

  (* external to_object_exn : *)
  (*   js_context_ref -> js_value_ref -> js_object_ref = "jsc_ml_to_object" *)

  (* external protect : *)
  (*   js_context_ref -> js_value_ref -> unit = "jsc_ml_protect" *)

  (* external unprotect : *)
  (*   js_context_ref -> js_value_ref -> unit = "jsc_ml_unprotect" *)

end

module Object = struct

  open Base

  type property_attr = None | Read_only | Don't_enum | Don't_delete

  type class_attr = None | Prototype

  type init_callback = context:js_context_ref -> js_object_ref -> unit

  type finalize_callback = js_object_ref -> unit

  type has_property_callback =
    context:js_context_ref -> obj:js_object_ref -> js_string_ref -> bool

  type get_property_callback_exn =
    context:js_context_ref -> obj:js_object_ref -> js_string_ref -> js_value_ref

  type set_property_callback_exn =
    context:js_context_ref -> obj:js_object_ref ->
    prop_name:js_string_ref -> js_value_ref -> js_value_ref

  type delete_property_callback_exn =
    context:js_context_ref -> obj:js_object_ref -> js_string_ref -> bool

  type get_property_names_callback =
    context:js_context_ref -> obj:js_object_ref ->
    js_property_name_accumulator_ref -> unit

  type call_as_function_callback_exn =
    context:js_context_ref -> func:js_object_ref -> this:js_object_ref ->
    arg_count:int -> arguments:js_value_ref list -> js_object_ref

  type call_as_constructor_callback_exn =
    context:js_context_ref -> constructor:js_object_ref ->
    arg_count:int -> arguments:js_value_ref list -> js_object_ref

  type has_instance_callback_exn =
    context:js_context_ref -> constructor:js_object_ref ->
    poss_instance:js_value_ref -> bool

  type convert_to_type_callback_exn =
    context:js_context_ref -> obj:js_object_ref ->
    type_:Value.js_t -> js_value_ref


  type static_value = {name : string;
                       get_property : get_property_callback_exn;
                       set_property : set_property_callback_exn;
                       property_attributes : property_attr list }

  type static_function = {name: string;
                          call_as_function : call_as_function_callback_exn;
                          property_attributes: property_attr list}

  type class_definition =
    { mutable attributes: class_attr list option;
      mutable class_name : string option;
      mutable parent_class : js_class_ref option;
      mutable static_values : static_value list option;
      mutable static_functions : static_function list option;
      mutable init : init_callback option;
      mutable finalize : finalize_callback option;
      mutable has_property : has_property_callback option;
      mutable get_property : get_property_callback_exn option;
      mutable set_property : set_property_callback_exn option;
      mutable delete_property : delete_property_callback_exn option;
      mutable get_property_names : get_property_names_callback option;
      mutable call_as_function : call_as_function_callback_exn option;
      mutable call_as_constructor : call_as_constructor_callback_exn option;
      mutable has_instance : has_instance_callback_exn option;
      mutable convert_to_type : convert_to_type_callback_exn option }

  let class_definition_empty () =
    {attributes = None; class_name = None; parent_class = None; static_values = None;
     static_functions = None; init = None; finalize = None; has_property = None;
     get_property = None; set_property = None; delete_property = None;
     get_property_names = None; call_as_function = None; call_as_constructor = None;
     has_instance = None; convert_to_type = None;}

  (* external class_create : *)
  (*   class_definition -> js_class_ref = "jsc_ml_class_create" *)

  (* external class_retain : *)
  (*   js_class_ref -> unit = "jsc_ml_class_retain" [@@noalloc] *)

  (* external class_release : *)
  (*   js_class_ref -> unit = "jsc_ml_class_release" [@@noalloc] *)

  (* external make : *)
  (*   js_context_ref -> js_class_ref -> js_object_ref = "jsc_ml_object_make" *)

  (* external make_function_with_callback : *)
  (*   js_context_ref -> js_string_ref option -> *)
  (*   call_as_function_callback_exn -> js_object_ref *)
  (*   = "jsc_ml_make_function_with_callback" *)

  (* external make_constructor : *)
  (*   js_context_ref -> js_class_ref -> *)
  (*   call_as_constructor_callback_exn -> js_object_ref *)
  (*   = "jsc_ml_make_constructor" *)

  (* external make_array_exn : *)
  (*   js_context_ref -> argc:int -> js_value_ref list -> js_object_ref *)
  (*   = "jsc_ml_make_array" *)

  (* external make_date_exn : *)
  (*   js_context_ref -> argc:int -> js_value_ref list -> js_object_ref *)
  (*   = "jsc_ml_make_date" *)

  (* external make_error_exn : *)
  (*   js_context_ref -> argc:int -> js_value_ref list -> js_object_ref *)
  (*   = "jsc_ml_make_error" *)

  (* external make_regexp_exn : *)
  (*   js_context_ref -> argc:int -> js_value_ref list -> js_object_ref *)
  (*   = "jsc_ml_make_regexp" *)

  (* external make_function_exn : *)
  (*   js_context_ref -> name:js_string_ref option -> *)
  (*   param_count:int -> param_names:js_string_ref list option -> *)
  (*   body:js_string_ref -> source_url:js_string_ref option -> *)
  (*   starting_line:int -> js_object_ref *)
  (*   = "jsc_ml_make_function_bytecode" "jsc_ml_make_function_native" *)

  (* external get_prototype : *)
  (*   js_context_ref -> js_object_ref -> js_value_ref *)
  (*   = "jsc_ml_get_prototype" *)

  (* external set_prototype : *)
  (*   js_context_ref -> js_object_ref -> js_value_ref -> unit *)
  (*   = "jsc_ml_set_prototype" *)

  (* external has_property : *)
  (*   js_context_ref -> js_object_ref -> js_string_ref -> bool *)
  (*   = "jsc_ml_has_property" *)

  (* external get_property_exn : *)
  (*   js_context_ref -> js_object_ref -> js_string_ref -> js_value_ref *)
  (*   = "jsc_ml_get_property" *)

  (* external set_property_exn : *)
  (*   js_context_ref -> obj:js_object_ref -> js_string_ref -> js_value_ref -> *)
  (*   property_attr list -> unit = "jsc_ml_set_property" *)

  (* external delete_property_exn : *)
  (*   js_context_ref -> obj:js_object_ref -> js_string_ref -> bool *)
  (*   = "jsc_ml_delete_property" *)

  (* external property_at_index_exn : *)
  (*   js_context_ref -> obj:js_object_ref -> index:int -> js_value_ref *)
  (*   = "jsc_ml_get_property_at_index" *)

  (* external set_property_at_index_exn : *)
  (*   js_context_ref -> obj:js_object_ref -> index:int -> *)
  (*   value:js_value_ref -> unit *)
  (*   = "jsc_ml_get_property_at_index" *)

  (* external is_function : *)
  (*   js_context_ref -> js_object_ref -> bool *)
  (*   = "jsc_ml_is_function" *)

  (* external call_as_function_exn : *)
  (*   js_context_ref -> js_object_ref -> this:js_object_ref option -> *)
  (*   argc:int -> js_value_ref list option -> js_value_ref *)
  (*   = "jsc_ml_call_as_function" *)

  (* external is_constructor : *)
  (*   js_context_ref -> js_object_ref -> bool *)
  (*   = "jsc_ml_is_constructor" *)

  (* external call_as_constructor_exn : *)
  (*   js_context_ref -> js_object_ref -> argc:int -> *)
  (*   js_value_ref list option -> js_object_ref *)
  (*   = "jsc_ml_call_as_constructor" *)

  (* external property_names : *)
  (*   js_context_ref -> js_object_ref -> js_property_name_array_ref *)
  (*   = "jsc_ml_property_names" *)

  (* external property_name_retain : *)
  (*   js_property_name_array_ref -> unit *)
  (*   = "jsc_ml_property_name_retain" [@@noalloc] *)

  (* external property_name_release : *)
  (*   js_property_name_array_ref -> unit *)
  (*   = "jsc_ml_property_name_release" [@@noalloc] *)

  (* external property_name_get_count : *)
  (*   js_property_name_array_ref -> int *)
  (*   = "jsc_ml_property_name_array_get_count" *)

  (* external property_name_get_name_at_index : *)
  (*   js_property_name_array_ref -> int -> js_string_ref *)
  (*   = "jsc_ml_property_name_get_name_at_index" *)

  (* external property_name_accumulator_add_name : *)
  (*   js_property_name_array_ref -> js_string_ref -> unit *)
  (*   = "jsc_ml_property_name_accumulator_add_name" *)

end

module Context = struct

  open Base

  (* external group_create : *)
  (*   unit -> js_context_group_ref *)
  (*   = "jsc_ml_context_group_create" *)

  (* external group_retain : *)
  (*   js_context_group_ref -> js_context_group_ref *)
  (*   = "jsc_ml_context_group_retain" *)

  (* external group_retain : *)
  (*   js_context_group_ref -> unit = "jsc_ml_context_group_retain" *)

  (* external global_context_create : *)
  (*   js_class_ref option -> unit -> js_global_context_ref *)
  (*   = "jsc_ml_global_context_create" *)

  external global_context_create_in_group :
    js_context_group_ref option -> js_class_ref option ->
    unit -> js_global_context_ref
    = "jsc_ml_global_context_create_in_group"

  (* external global_context_retain : *)
  (*   js_global_context_ref -> js_global_context_ref *)
  (*   = "jsc_ml_global_context_retain" [@@noalloc] *)

  external global_context_release :
    js_global_context_ref -> unit = "jsc_ml_global_context_release" [@@noalloc]

  (* external get_global_object : *)
  (*   js_context_ref -> js_object_ref = "jsc_ml_get_global_object" *)

  (* external get_group : *)
  (*   js_context_ref -> js_context_group_ref *)
  (*   = "jsc_ml_get_context_group" *)

  (* external get_global_context : *)
  (*   js_context_ref -> js_global_context_ref *)
  (*   = "jsc_ml_get_global_context" *)

  (* external global_context_name : *)
  (*   js_global_context_ref -> js_string_ref = "jsc_ml_context_copy_name" *)

  (* external global_context_set_name : *)
  (*   js_global_context_ref -> js_string_ref -> unit *)
  (*   = "jsc_ml_global_context_set_name" *)

end

module String = struct

  open Base

  (* external create_with_data : *)
  (*   Bigarray.int8_unsigned_elt -> unit = "jsc_ml_string_create_with_chars" *)

  (* external create_with_utf8_string : *)
  (*   string -> js_string_ref = "jsc_ml_string_create_with_utf8" *)

  (* external retain : js_string_ref -> js_string_ref = "jsc_ml_string_retain" [@@noalloc] *)

  (* external release : js_string_ref -> unit = "jsc_ml_string_release" [@@noalloc] *)

  (* external length : js_string_ref -> int = "jsc_ml_string_length" *)

  (* external max_size_if_converted_to_utf8_string : *)
  (*   js_string_ref -> int = "jsc_ml_string_max_size" *)

  (* external is_equal : *)
  (*   js_string_ref -> js_string_ref -> bool *)
  (*   = "jsc_ml_string_is_equal" *)

  (* external is_equal_to_utf8_string : *)
  (*   js_string_ref -> string -> bool = "jsc_ml_is_equal_to_utf8_string" *)

end
