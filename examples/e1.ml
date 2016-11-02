  (* let options = *)
  (*   Object.class_definition_empty () *)
  (* in *)
  (* options.Object.init <- Some (fun ctx obj -> *)
  (*     print_endline "Initialized the object!" *)
  (*   ); *)
  (* options.Object.finalizer <- Some (fun obj -> *)
  (*     print_endline "Object was garbage collected" *)
  (*   ); *)
  (* options.Object.has_property <- Some (fun ctx obj prop_name -> *)
  (*     print_endline "Something tried checking property names"; *)
  (*     true *)
  (*   ); *)
  (* let some_js_class = Object.class_create options in *)
  (* let object_itself = *)
  (*   Object.object_make context *)
  (*     (Some some_js_class) *)
  (*     (Some options.Object.identifier) *)
  (* in *)

open JavaScriptCore

let () =
  let context = Context.context_create None in
  let script = String.create_with_utf8 "return some_variable.toUpperCase()" in
  let fn =
    Object.(object_make_function_exn
              {context;
               function_name = None;
               parameter_names = [|String.create_with_utf8 "some_variable"|];
               function_body = script;
               source_url = None;
               starting_line_number = 1;
              })
  in
  let result =
    Object.object_call_as_function_exn
      context
      fn
      ~this:None
      [|JSValue.jsstring_value_of_string context "hello world"|]
  in
  print_js context result
