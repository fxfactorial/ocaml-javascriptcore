

let test_program () =
  let script_path = "../static/testapi.js" in
  let context =
    JavaScriptCore.Context.global_context_create_in_group None None () in
  JavaScriptCore.Context.global_context_release context;

  let global_object_class_definition =
    JavaScriptCore.Object.class_definition_empty ()
  in
  
  "Completed test"

let main () =
  print_endline "\tStarting test"
  |> test_program
  |> print_endline

let () = main ()
