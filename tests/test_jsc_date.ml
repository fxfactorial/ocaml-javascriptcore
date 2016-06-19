let make_date () = let open JavaScriptCore in
  let vm = new virtual_machine () in
  vm#make_js_date ()

let date_test_1 () = let open JavaScriptCore in
  let date = make_date () in
  let msg =
    "Checking Date property presence: setYear"
  in
  Alcotest.(check bool) msg true (date#has_property "setYear")

let date_test_1 () = let open JavaScriptCore in
  let date = make_date () in
  let msg =
    "Checking Date property presence: FAKE_FIELD"
  in
  Alcotest.(check bool) msg false (date#has_property "FAKE_FIELD")

(* Think of this like a set of tests for this particular
   functionality *)
let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, date_test_1);
]

let () =
  Alcotest.run "JavaScriptCore Dates" [
    ("Test Date Property Ending", test_set_one);
  ]
