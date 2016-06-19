let date_test_1 () = let open JavaScriptCore in
  let s =
    new Objects.js_date (new virtual_machine ())
  in
  let msg =
    "Checking Date creation"
  in
  ()

(* Think of this like a set of tests for this particular
   functionality *)
let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, date_test_1);
]

let () =
  Alcotest.run "JavaScriptCore Dates" [
    ("Test Dates", test_set_one);
  ]
