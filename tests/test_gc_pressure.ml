let gc_pressure_1 () =
  let mini_func () = ignore (new JavaScriptCore.virtual_machine ()) in
  mini_func ();
  Gc.major ()

let gc_pressure_2 () =
  let mini_func () =
    let js_vm = new JavaScriptCore.virtual_machine () in
    js_vm#evaluate_script {|
const first = "Hello";
const second = " World";
first + second
|} |> ignore
  in
  mini_func ();
  Gc.major ()

let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Quick, gc_pressure_1);
]

let test_set_two = [
  ("\xF0\x9F\x90\xAB", `Slow, gc_pressure_2)
]

let () =
  Alcotest.run "GC Pressure" [
    ("Create one VM, then force gc", test_set_one);
    ("Evaluate Script, then force gc", test_set_two)
  ]
