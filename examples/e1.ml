module JSC = JavaScriptCore

let () =
  let class_definition = JSC.Object.class_definition_empty () in
  JSC.Object.(
    class_definition.class_name <- "File";
    class_definition.init <- Some (fun ~context ~init_obj ->
        print_endline "being init"
      );
    class_definition.finalizer <- Some (fun ~finalized_obj ->
        print_endline "Object being garbaged collected"
      );
    class_definition.has_property <- Some (fun ~context ~obj ~prop_name ->
        let prop =
          JSC.Value.make_string context prop_name |> JSC.to_string_of_jsvalue ~context
        in
        Printf.printf "Prop name requested: %s\n" prop;
        true
      );
    class_definition.call_as_constructor <- Some (fun ~context ~constructor ~args ->
        let cwd = Sys.getcwd () in
        let filename =
          JSC.String.create_with_utf8 cwd
          |> JSC.Value.make_string context
        in
        let property_name = JSC.String.create_with_utf8 "cwdName" in
        let target = JSC.Object.make context in
        JSC.Object.set_property_exn ~context ~target ~property_name filename;
        target
      )
  );
  let context = JSC.Context.make () in
  let js_example_class = JSC.Object.class_create class_definition in
  let example_obj = JSC.Object.make ~default_class:js_example_class context in
  let global_object = JSC.Context.global_object context in

  JSC.Object.set_property_exn
    ~context
    ~property_name:(JSC.String.create_with_utf8 "File")
    ~target:global_object
    example_obj;

  let code_to_eval = {|
const example_code = new File;
example_code.cwdName
|}
  in
  let result =
    JSC.eval_script_exn
      context
      (JSC.String.create_with_utf8 code_to_eval)
  in
  JSC.to_string_of_jsvalue context result
  |> print_endline

let () =
  let f () =
    let vm = new JSC.virtual_machine in
    let result =
      vm#eval_script {|"foo".toUpperCase()|} |> vm#value_to_string
    in
    print_endline result
  in
  f ();
  let vm = new JSC.virtual_machine in
  match vm#eval_script {|10|} |> vm#get_type with
  | JSC.Value.Undefined -> print_endline "Was undefined"
  | JSC.Value.Number -> print_endline "Value was a number"
  | JSC.Value.String -> print_endline "A string"
  | _ -> print_endline "Else"

let () =
  let ctx = JSC.Context.make () in
  let str = JSC.String.create_with_utf8 "{\"foo\":123}" in
  let as_json = JSC.Value.make_from_json_string_exn ctx str in
  let pretty_string =
    JSC.Value.make_json_string_from_value_exn ctx as_json JSC.Value.Three
  in
  print_endline (JSC.to_string_of_jsstring ctx pretty_string)

let () =
  let ctx = JSC.Context.make () in
  let obj = JSC.Object.make ctx in
  JSC.Object.set_property_exn
    ~context:ctx
    ~target:obj
    ~property_name:(JSC.String.create_with_utf8 "test")
    (JSC.Value.make_number ctx 4.5);

  JSC.property_names_of_object ctx obj
  |> Array.iter print_endline

(* Segfaults *)
(* let () = *)
(*   let ctx = JSC.Context.make () in *)
(*   let value = JSC.Value.make_string ctx (JSC.String.create_with_utf8 "hello") in *)
(*   let prototype = JSC.Object.get_prototype ctx value in *)
(*   JSC.property_names_of_object ctx prototype *)
(*   |> Array.iter print_endline *)
