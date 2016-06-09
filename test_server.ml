open JavaScriptCore

module S = Cohttp_lwt_unix.Server

let () =
  let reactjs_src = Podge.Unix.read_all "static/react.min.js" in
  let ctx = make_context () in

  if not (exec_js ctx reactjs_src |> bool_of_string)
  then raise (Failure "Couldn't load JavaScript");

  let reactjs_code = {| (function(){

 var ExampleApplication = React.createClass({
        render: function() {
          var elapsed = Math.round(this.props.elapsed  / 100);
          var seconds = elapsed / 10 + (elapsed % 10 ? '' : '.0' );
          var message =
            'React has been successfully running for ' + seconds + ' seconds.';

          return React.DOM.p(null, message);
        }
      });
      var ExampleApplicationFactory = React.createFactory(ExampleApplication);

      var start = new Date().getTime();
    var e = ExampleApplicationFactory({elapsed: new Date().getTime() - start});

    return React.__SECRET_DOM_SERVER_DO_NOT_USE_OR_YOU_WILL_BE_FIRED.
renderToString(React.createElement(ExampleApplication), null);

})();
|}
  in

  let callback conn _ _ =
    let react_created_body = exec_js ctx reactjs_code in
    print_endline react_created_body;
    S.respond_string ~status:`OK ~body:react_created_body ()
  in
  let server = S.make callback () in
  let port = 8080 in
  Lwt_io.printlf "Started Server on port:%d" port >>
  Cohttp_lwt_unix.Server.create ~mode:(`TCP (`Port port)) server
  |> Lwt_main.run
