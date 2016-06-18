let break_keyword () =
  let js_vm = new JavaScriptCore.virtual_machine () in
  let msg = Printf.sprintf "Checking syntax of JS code: %s" "break" in
  Alcotest.(check bool) msg false (js_vm#check_syntax "break")

let simple_assignment () =
  let js_vm = new JavaScriptCore.virtual_machine () in
  let js_code = "const a = 1 + 2;" in
  Alcotest.(check bool)
    (Printf.sprintf "Checking syntax of JS code: %s" js_code) true
    (js_vm#check_syntax js_code)

let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, break_keyword);
]

let test_set_two = [
  ("\xF0\x9F\x90\xAB", `Quick, simple_assignment)
]

let () =
  Alcotest.run "JS Syntax" [
    ("Keywords out of place", test_set_one);
    ("Assignments", test_set_two)
  ]
