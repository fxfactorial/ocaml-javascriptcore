(** These are high level bindings to JavaScriptcore. The
    virtual_machine object is garbage collected by the OCaml runtime
    everything else will call release when garbage collected by the
    OCaml runtime *)

(** An exception raised by the JavaScript side during script
    evaluation *)
exception JavaScript_eval of string

(** A JavaScript execution context. Holds the global object and
      other execution state. *)
class virtual_machine : ?named:string -> unit -> object

    (** Evaluate JavaScript code, gives back result of evaluation *)
    method evaluate_script : string -> string

    (** Force a garbage collection in the VM *)
    method garbage_collect : unit

    (** Check if the JavaScript given has a syntax error *)
    method check_syntax : string -> bool

    (** Name of the given context *)
    method name : string

    (** Set the name of the context *)
    method set_name : string -> unit

  end
