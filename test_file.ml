open JavaScriptCore

let () =
  let js_code = {|
    (function(){
return "Hello world string from JS to OCaml";
})()
|}
  in
  let ctx = make_context () in
  exec_js ctx js_code
  |> print_endline
