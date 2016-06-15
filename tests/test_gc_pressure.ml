let gc_pressure_1 () =
  let mini_func () = 
    ignore (new JavaScriptCore.vm)
  in
  mini_func ();
  Gc.major ()

let gc_pressure_2 () =
  let js_vm = new JavaScriptCore.vm in

  ()
  (* let js_code = "const a = 1 + 2;" in *)
  (* Alcotest.(check bool) *)
  (*   (Printf.sprintf "Checking syntax of JS code: %s" js_code) true *)
  (*   (js_vm#check_syntax js_code) *)

let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, gc_pressure_1);
]

let test_set_two = [
  ("\xF0\x9F\x90\xAB", `Quick, gc_pressure_2)
]

let () =
  Alcotest.run "GC Pressure testing" [
    ("Create gc'ed VM, force gc", test_set_one);
    (* ("plain assignment of numbers", test_set_two) *)
  ]
