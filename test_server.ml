open JavaScriptCore

module S = Cohttp_lwt_unix.Server

let () =
  let reactjs_src = Podge.Unix.read_all "static/react.min.js" in
  let ctx = make_context () in

  if not (exec_js ctx reactjs_src |> bool_of_string)
  then raise (Failure "Couldn't load JavaScript");

  let reactjs_code = {| (function(){

var Counter = React.createClass({
        getInitialState: function () {
          return { clickCount: 0 };
        },
        handleClick: function () {
          this.setState(function(state) {
            return {clickCount: state.clickCount + 1};
          });
        },
        render: function () {
    return React.DOM.h2({onClick:this.handleClick},
                        "Click Me! Number of clicks:" + this.state.clickCount);
        }
      });

   return React.__SECRET_DOM_SERVER_DO_NOT_USE_OR_YOU_WILL_BE_FIRED.
                        renderToString(React.createElement(Counter), null);

})();
|}
  in

  let callback conn _ _ =
    let react_created_body = exec_js ctx reactjs_code in
    S.respond_string ~status:`OK ~body:react_created_body ()
  in
  let server = S.make callback () in
  let port = 8080 in
  Lwt_io.printlf "Started Server on port:%d" port >>
  Cohttp_lwt_unix.Server.create ~mode:(`TCP (`Port port)) server
  |> Lwt_main.run
