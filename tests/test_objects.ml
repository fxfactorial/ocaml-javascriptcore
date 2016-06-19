let objects_test_1 () =
  let s =
    new JavaScriptCore.Objects.js_class ()
  in
  let msg =
    "Checking Object creation"
  in
  ()

(* Think of this like a set of tests for this particular
   functionality *)
let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, objects_test_1);
]

let () =
  Alcotest.run "JavaScriptCore Objects" [
    ("Test Object Creation", test_set_one);
  ]
