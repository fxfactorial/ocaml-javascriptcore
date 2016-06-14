(** A group that associates JavaScript contexts with one
    another. Contexts in the same group may share and exchange
    JavaScript objects.*)
type context_group

(** A JavaScript execution context. Holds the global object and
    other execution state. *)
type virtual_machine

(** A context, imagine multiple contexts being like iFrames*)
type context

(** A UTF16 character buffer. The fundamental string representation
    in JavaScript. *)
type js_string

(** A JavaScript class. Used with JSObjectMake to construct objects
    with custom behavior. *)
type js_class

(** An array of JavaScript property names. *)
type property_name_array

(** An ordered set used to collect the names of a JavaScript object's
    properties. *)
type property_name_accumulator

(** A JavaScript value. The base type for all JavaScript values, and
    polymorphic functions on them. *)
type js_value

(** A JavaScript object. an object_ is a value. *)
type js_object

module Raw_calls = struct
  external make_vm : unit -> virtual_machine = "jsc_ml_make_vm"
  external evaluate_script : virtual_machine -> string -> string = "jsc_ml_eval_script"
  external check_script_syntax : virtual_machine -> string -> bool = "jsc_ml_check_syntax"
  external garbage_collect : virtual_machine -> unit = "jsc_ml_garbage_collect"
end

class vm = object
  val vm_ = Raw_calls.make_vm ()
  method evaluate_script src = Raw_calls.evaluate_script vm_ src
  method gc = Raw_calls.garbage_collect vm_
  method check_syntax src = Raw_calls.check_script_syntax vm_ src
end

module JSString = struct

  (* external create_with_chars : *)
  (*   Bigarray.int8_unsigned_elt -> js_string = "jsc_ml_create_with_chars" *)

  (* external create_with_utf8_string : *)
  (*   string -> js_string = "jsc_ml_create_with_null_string" *)

  (* external length : js_string -> int = "jsc_ml_string_length" *)

  (* external is_equal : js_string -> js_string -> bool = "jsc_ml_string_is_equal" *)

  (* external to_string : js_string -> string = "jsc_ml_to_ml_string" *)

end

module JSContext = struct

  (* external create_group : unit -> context_group = "jsc_ml_create_context_group" *)

  (* external create : *)
  (*   ?js_class:js_class -> unit -> context = "jsc_ml_create_context" *)

  (* external create_in_group : *)
  (*   ?context_group:context_group -> ?js_class:js_class -> unit -> context = *)
  (*   "jsc_ml_create_context_in_group" *)

  (* external get_global : context -> js_object = "jsc_ml_get_global" *)

  (* external get_context_group : *)
  (*   context -> context_group = "jsc_ml_get_context_group" *)

  (* external context_name : context -> string = "jsc_ml_get_context_name" *)

  (* external set_context_name : *)
  (*   context -> string -> unit = "jsc_ml_set_context_name" *)

end

module JSValue = struct

  (* type js_type = Undefined | Null | Bool | Number | String | Object *)

  (* type created_value = {js_type : js_type; *)
  (*                       js_value : js_value} *)

  (* external get_type : context -> js_value -> js_type = "jsc_ml_get_type" *)

  (* external is_undefined : context -> js_value -> bool = "jsc_ml_is_undefined" *)

  (* external is_null : context -> js_value -> bool = "jsc_ml_is_null" *)

  (* external is_boolean : context -> js_value -> bool = "jsc_ml_is_boolean" *)

  (* external is_number : context -> js_value -> bool = "jsc_ml_is_number" *)

  (* external is_string : context -> js_value -> bool = "jsc_ml_is_string" *)

  (* external is_object : context -> js_value -> bool = "jsc_ml_is_object" *)

  (* external is_object_of_class : *)
  (*   context -> js_value -> js_class -> bool = "jsc_ml_is_object_of_class" *)

  (* external is_array : context -> js_value -> bool = "jsc_ml_is_array" *)

  (* external is_date : context -> js_value -> bool = "jsc_ml_is_date" *)

  (* external is_equal : *)
  (*   context -> js_value -> js_value -> bool = "jsc_ml_is_equal" *)

  (* external is_strict_equal : *)
  (*   context -> js_value -> js_value -> bool = "jsc_ml_is_strict_equal" *)

  (* external is_instance_of_constructor : *)
  (*   context -> js_value -> js_object -> bool = "jsc_ml_is_instance_of_constr" *)

  (* external make_undefined : context -> created_value = "jsc_ml_make_undefined" *)

  (* external make_null : context -> created_value = "jsc_ml_make_null" *)

  (* external make_boolean : context -> created_value = "jsc_ml_make_boolean" *)

  (* external make_number : context -> created_value = "jsc_ml_make_number" *)

  (* external make_string : *)
  (*   context -> js_string -> created_value = "jsc_ml_make_string" *)

  (* external make_from_json_string : *)
  (*   context -> js_string -> created_value = "jsc_ml_make_from_json_string" *)

  (* external create_json_of_value : *)
  (*   context -> js_value -> int -> js_string = "jsc_ml_create_json_of_value" *)

  (* external to_bool : context -> js_value -> bool = "jsc_ml_value_to_bool" *)

  (* external to_number : context -> js_value -> float = "jsc_ml_value_to_number" *)

  (* external to_string : *)
  (*   context -> js_value -> js_string = "jsc_ml_value_to_string" *)

  (* external to_object : *)
  (*   context -> js_value -> js_object = "jsc_ml_value_to_object" *)

end
