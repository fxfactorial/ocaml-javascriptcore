(** These are high level bindings to JavaScriptcore. The
    virtual_machine object is garbage collected by the OCaml runtime
    everything else will call release when garbage collected by the
    OCaml runtime *)

module rec Types :
sig
  type js_ptr
  type class_def = {name : string;
                    parent : Objects.js_class}
  type date_opt = {year : int;
                   month : int;
                   day : int option;
                   hour : int option;
                   minutes : int option;
                   seconds : int option;
                   milliseconds : int option}
  type date_opt_t = [`Date_string of string
                    | `Date_opts of date_opt]
end
and Top_level : sig
  (** A JavaScript execution context. Holds the global object and
        other execution state. *)
  class virtual_machine : ?named:string -> unit ->
    object
      method check_syntax : string -> bool
      method evaluate_script : string -> string
      method garbage_collect : unit
      method name : string
      method set_name : string -> unit
      method unsafe_ptr_value : Types.js_ptr
    end

end
and Objects :
sig

  (** A JavaScript class *)
  class js_class : ?class_def:Types.class_def -> unit ->
    object
      method release : unit
      method retain : unit
      method unsafe_ptr_value : Types.js_ptr
    end

  (** A JavaScript String *)
  class js_string : ml_string:string ->
    object
      method length : int
      method release : unit
      method retain : unit
    end


  (** A JavaScript Context Group *)
  class context_group :
    object
      method release : unit
      method retain : unit
    end

  (** A JavaScript Object *)
  class js_object : ?js_class:js_class -> Top_level.virtual_machine ->
    object

    end

  (** A JavaScript Date Object *)
  class js_date : ?opts:Types.date_opt_t -> Top_level.virtual_machine ->
    object

    end

end

class virtual_machine : ?named:string -> unit ->
  object
    method check_syntax : string -> bool
    method evaluate_script : string -> string
    method garbage_collect : unit
    method name : string
    method set_name : string -> unit
    method unsafe_ptr_value : Types.js_ptr
  end
