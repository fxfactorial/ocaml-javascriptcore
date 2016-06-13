let syntax_1 () = let open JavaScriptCore in
  let ctx = make_context () in
  let msg = Printf.sprintf "Checking syntax of JS code: %s" "break" in
  Alcotest.(check bool) msg false (check_script_syntax ctx "break")

let syntax_2 () = let open JavaScriptCore in
  let ctx = make_context () in
  let js_code = "const a = 1 + 2;" in
  Alcotest.(check bool)
    (Printf.sprintf "Checking syntax of JS code: %s" js_code)
    true
    (check_script_syntax ctx js_code)

let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, syntax_1);
]

let test_set_two = [
  ("\xF0\x9F\x90\xAB", `Quick, syntax_2)
]

let () =
  Alcotest.run "JS Syntax" [
    ("break statement out of loop", test_set_one);
    ("plain assignment of numbers", test_set_two)
  ]
