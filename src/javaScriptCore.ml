module rec JSC : sig
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
  external length_js_string :
    js_ptr -> int = "jsc_ml_jsc_length" [@@noalloc]
  external make_jsc_context_group : unit -> js_ptr = "jsc_ml_make_context_group"
  external retain_js_context_group :
    js_ptr -> unit = "jsc_ml_retain_context_group" [@@noalloc]
  external release_js_context_group
    : js_ptr -> unit = "jsc_ml_release_context_group" [@@noalloc]
  external set_vm_context_name : js_ptr -> string -> unit = "jsc_ml_set_context_name"
  external get_vm_context_name : js_ptr -> string = "jsc_ml_get_context_name"
  external make_jsc_js_class : Types.class_def option -> js_ptr = "jsc_ml_make_class"
  external retain_js_class :
    js_ptr -> unit = "jsc_ml_retain_class" [@@noalloc]
  external release_js_class :
    js_ptr -> unit = "jsc_ml_release_class" [@@noalloc]

end = JSC

and Types : sig
  type class_def = {name: string;
                    parent : Objects.js_class; }

end = Types

and Objects : sig

  class js_class : ?class_def:Types.class_def -> unit -> object
      method retain : unit
      method release : unit
    end

  class js_string : ml_string:string -> object
      method retain : unit
      method release : unit
      method length : int
    end

  class virtual_machine : ?named:string -> unit -> object
      method evaluate_script : string -> string
      method garbage_collect : unit
      method check_syntax : string -> bool
      method set_name : string -> unit
      method name : string
    end

  class context_group : object
    method retain : unit
    method release : unit
  end

end = struct

  class virtual js_ref = object(self)
    val virtual raw_ptr : JSC.js_ptr
    initializer
      Gc.finalise (fun instance -> instance#release) self
    method virtual retain : unit
    method virtual release : unit
  end

  class js_class ?class_def () = object
    inherit js_ref
    val raw_ptr = JSC.make_jsc_js_class class_def
    method retain = JSC.retain_js_class raw_ptr
    method release = JSC.release_js_class raw_ptr
  end

  type class_def = {name: string;
                    parent : js_class}

  class js_string ~ml_string = object
    inherit js_ref
    val raw_ptr = JSC.make_js_string_with_ml_string ml_string
    method retain = JSC.retain_js_string raw_ptr
    method release = JSC.release_js_string raw_ptr
    method length = JSC.length_js_string raw_ptr
  end

  class virtual_machine ?named () = object(self)
    val raw_ptr = JSC.make_vm ()
    initializer
      match named with None -> self#set_name "" | Some s -> self#set_name s
    method evaluate_script src = JSC.evaluate_script raw_ptr src
    method garbage_collect = JSC.garbage_collect raw_ptr
    method check_syntax src = JSC.check_script_syntax raw_ptr src
    method set_name name = JSC.set_vm_context_name raw_ptr name
    method name = JSC.get_vm_context_name raw_ptr
  end

  class context_group = object
    inherit js_ref
    val raw_ptr = JSC.make_jsc_context_group ()
    method retain = JSC.retain_js_context_group raw_ptr
    method release = JSC.release_js_context_group raw_ptr
  end

end
