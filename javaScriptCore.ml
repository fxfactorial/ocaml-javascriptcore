type context

external make_context : unit -> context = "create_js_context_ml"

external evaluate_script : ctx:context -> string -> string = "eval_js_ml"
external garbage_collect : context -> unit = "gc_js_ml"
