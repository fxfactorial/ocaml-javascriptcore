open JavaScriptCore

let () =
  let script = String.create_with_utf8 "return new Array" in
  let options =
    Object.class_definition_empty ()
  in
  options.Object.init <- Some (fun ctx obj ->
      print_endline "Initialized the object!"
    );
  let some_js_class = Object.class_create options in
  let context = Context.global_context_create None in
  let object_itself =
    Object.object_make context (Some some_js_class) (Some options.Object.identifier)
  in
  ()

