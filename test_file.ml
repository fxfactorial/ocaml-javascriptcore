open JavaScriptCore

let () =
  let js_code = {|
    (function(){
return "Hello world string from JS to OCaml";
})()
|}
  in
  exec_js js_code
  |> print_endline
