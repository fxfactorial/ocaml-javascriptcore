module rec JSC : sig
  external make_vm :
    unit -> Types.js_ptr = "jsc_ml_make_vm"
  external evaluate_script :
    Types.js_ptr -> string -> string = "jsc_ml_eval_script"
  external check_script_syntax :
    Types.js_ptr -> string -> bool = "jsc_ml_check_syntax"
  external garbage_collect :
    Types.js_ptr -> unit = "jsc_ml_garbage_collect"
  external make_js_string_with_ml_string :
    string -> Types.js_ptr = "jsc_ml_make_string_with_ml_str"
  external release_js_string
    : Types.js_ptr -> unit = "jsc_ml_release_jsc_string" [@@noalloc]
  external retain_js_string
    : Types.js_ptr -> unit = "jsc_ml_retain_jsc_string" [@@noalloc]
  external length_js_string :
    Types.js_ptr -> int = "jsc_ml_jsc_length" [@@noalloc]
  external make_jsc_context_group : unit -> Types.js_ptr = "jsc_ml_make_context_group"
  external retain_js_context_group :
    Types.js_ptr -> unit = "jsc_ml_retain_context_group" [@@noalloc]
  external release_js_context_group
    : Types.js_ptr -> unit = "jsc_ml_release_context_group" [@@noalloc]
  external set_vm_context_name : Types.js_ptr -> string -> unit = "jsc_ml_set_context_name"
  external get_vm_context_name : Types.js_ptr -> string = "jsc_ml_get_context_name"
  external make_jsc_js_class : Types.class_def option -> Types.js_ptr = "jsc_ml_make_class"
  external retain_js_class :
    Types.js_ptr -> unit = "jsc_ml_retain_class" [@@noalloc]
  external release_js_class :
    Types.js_ptr -> unit = "jsc_ml_release_class" [@@noalloc]
  external make_jsc_object :
    Types.js_ptr -> Types.js_ptr option -> Types.js_ptr = "jsc_ml_make_object"
  external make_jsc_date_now :
    Types.js_ptr -> Types.js_ptr = "jsc_ml_make_date_now"
  external make_jsc_date_with_dates :
    Types.js_ptr -> Types.date_opt -> Types.js_ptr = "jsc_ml_make_date_with_dates"
  external make_jsc_date_with_datestring :
    Types.js_ptr -> string -> Types.js_ptr = "jsc_ml_make_date_with_datestring"

end = JSC

and Types : sig
  type js_ptr
  type class_def = {name: string;
                    parent : Objects.js_class; }
  type date_opt = {year : int;
                   month : int;
                   day : int option;
                   hour : int option;
                   minutes : int option;
                   seconds : int option;
                   milliseconds : int option}

  type date_opt_t = [`Date_string of string
                    | `Date_opts of date_opt]

end = Types

and Top_level : sig

  class virtual_machine : ?named:string -> unit ->
    object
      method evaluate_script : string -> string
      method garbage_collect : unit
      method check_syntax : string -> bool
      method set_name : string -> unit
      method name : string
      method unsafe_ptr_value : Types.js_ptr
    end

end = struct
  class virtual_machine ?named () =
    object(self)
      val raw_ptr = JSC.make_vm ()
      initializer
        match named with None -> self#set_name "" | Some s -> self#set_name s
      method evaluate_script src = JSC.evaluate_script raw_ptr src
      method garbage_collect = JSC.garbage_collect raw_ptr
      method check_syntax src = JSC.check_script_syntax raw_ptr src
      method set_name name = JSC.set_vm_context_name raw_ptr name
      method name = JSC.get_vm_context_name raw_ptr
      method unsafe_ptr_value = raw_ptr
    end
end

and Objects : sig

  class js_class : ?class_def:Types.class_def -> unit ->
    object
      method retain : unit
      method release : unit
      method unsafe_ptr_value : Types.js_ptr
    end

  class js_string : ml_string:string ->
    object
      method retain : unit
      method release : unit
      method length : int
    end


  class context_group :
    object
      method retain : unit
      method release : unit
    end

  class js_object : ?js_class:js_class -> Top_level.virtual_machine ->
    object

    end

  class js_date : ?opts:Types.date_opt_t -> Top_level.virtual_machine ->
    object

    end

end = struct

  class virtual js_ref = object(self)
    val virtual raw_ptr : Types.js_ptr
    initializer
      Gc.finalise (fun instance -> instance#release) self
    method virtual retain : unit
    method virtual release : unit
  end

  class js_class ?class_def () =
    object
      inherit js_ref
      val raw_ptr = JSC.make_jsc_js_class class_def
      method retain = JSC.retain_js_class raw_ptr
      method release = JSC.release_js_class raw_ptr
      method unsafe_ptr_value = raw_ptr
    end

  type class_def = {name: string;
                    parent : js_class}

  class js_string ~ml_string =
    object
      inherit js_ref
      val raw_ptr = JSC.make_js_string_with_ml_string ml_string
      method retain = JSC.retain_js_string raw_ptr
      method release = JSC.release_js_string raw_ptr
      method length = JSC.length_js_string raw_ptr
    end


  class context_group =
    object
      inherit js_ref
      val raw_ptr = JSC.make_jsc_context_group ()
      method retain = JSC.retain_js_context_group raw_ptr
      method release = JSC.release_js_context_group raw_ptr
    end

  class js_object ?js_class vm =
    object
      val raw_ptr =
        (match js_class with Some c -> Some c#unsafe_ptr_value | _ -> None)
        |> JSC.make_jsc_object vm#unsafe_ptr_value
    end

  class js_date ?(opts : Types.date_opt_t option) vm =
    object
      val raw_ptr = Types.(JSC.(match opts with
          | None -> make_jsc_date_now vm#unsafe_ptr_value
          | Some `Date_string s ->
            make_jsc_date_with_datestring vm#unsafe_ptr_value s
          | Some `Date_opts opt ->
            make_jsc_date_with_dates vm#unsafe_ptr_value opt
        ))
    end

end

include Top_level
