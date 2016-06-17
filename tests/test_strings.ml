let string_test_1 () =
  let ml_string = "Hello World" in
  let s =
    new JavaScriptCore.js_string ~ml_string
  in
  let msg =
    "Checking length of created JavaScriptCore string"
  in
  Alcotest.(check int) msg (String.length ml_string) s#length

(* Think of this like a set of tests for this particular
   functionality *)
let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, string_test_1);

]

let () =
  Alcotest.run "JavaScriptCore Strings" [
    ("Test Lengths", test_set_one);
    (* Aka cause crashes with retain/release sequence of calls *)
    (* ("Cause crashes", test_set_two) *)
  ]
