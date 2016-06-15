let eval_script_one () =
  let js_vm = new JavaScriptCore.vm in
  Alcotest.(check string)
    (Printf.sprintf "Evaling script") "Hello World"
    (js_vm#evaluate_script {|
const first = "Hello";
const second = " World";
first + second
|})

let test_set_one = [
  ("\xF0\x9F\x90\xAB", `Slow, eval_script_one)
]

let () =
  Alcotest.run "Evaluation of scripts" [
    ("Eval a JavaScript string", test_set_one);
  ]
