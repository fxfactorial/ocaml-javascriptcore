type context

external make_context : unit -> context = "create_js_context_ml"

external exec_js : ctx:context -> string -> string = "exec_js"
