type context
type context_group
type global_context

type js_string
type js_class
type js_prop_name_array
type js_prop_name_accum

type js_value
type js_object


external make_context : unit -> context = "jsc_ml_make_context"

external evaluate_script : ctx:context -> string -> string = "jsc_ml_eval_script"

external check_script_syntax : context -> string -> bool = "jsc_ml_check_syntax"

external garbage_collect : context -> unit = "jsc_ml_gc"

module JSValue = struct

  type js_t = Undefined | Null | Bool | Number | Object

  external get_type : context -> js_value -> js_t = "jsc_ml_get_type"

end


module JSObject = struct

end

module JSString = struct

end


module JSContext = struct

end
