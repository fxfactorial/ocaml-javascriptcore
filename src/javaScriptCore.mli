(** These are high level bindings to JavaScriptcore. The
    virtual_machine object is garbage collected by the OCaml runtime
    everything else will call release when garbage collected by the
    OCaml runtime *)

module rec Types :
sig
  type class_def = {name : string;
                    parent : Objects.js_class}
end
and Objects :
sig

  (** A JavaScript class *)
  class js_class : Types.class_def ->
    object
      method release : unit
      method retain : unit
    end

  (** A JavaScript String *)
  class js_string : ml_string:string ->
    object
      method length : int
      method release : unit
      method retain : unit
    end

  (** A JavaScript execution context. Holds the global object and
        other execution state. *)
  class virtual_machine : ?named:string -> unit ->
    object
      method check_syntax : string -> bool
      method evaluate_script : string -> string
      method garbage_collect : unit
      method name : string
      method set_name : string -> unit
    end

  (** A JavaScript Context Group *)
  class context_group :
    object
      method release : unit
      method retain : unit
    end

end
