use std::fs;
use std::io::Write;

use anyhow::Result;
use lsp_server::{Connection, Message, Response};
use lsp_types::{
    ClientCapabilities, GotoDefinitionResponse, InitializeParams,
    ServerCapabilities,
};

fn log<S: AsRef<str>>(x: S) {
    let mut f = fs::File::options()
        .append(true)
        .open("/home/khang/repos/mylsp/log.txt")
        .unwrap();
    writeln!(f, "{}", x.as_ref()).unwrap();
}

fn actual_main() -> Result<()> {
    let (connection, io_threads) = Connection::stdio();
    let (id, params) = connection.initialize_start()?;
    let init_params: InitializeParams = serde_json::from_value(params).unwrap();
    let client_capabilities: ClientCapabilities = init_params.capabilities;

    let mut server_capabilities = ServerCapabilities::default();
    server_capabilities.definition_provider =
        Some(lsp_types::OneOf::Left(true));

    let initialize_data = serde_json::json!({
        "capabilities": server_capabilities,
        "serverInfo": {
            "name": "math-lsp",
            "version": "0.0.1"
        }
    });

    connection.initialize_finish(id, initialize_data)?;

    for msg in &connection.receiver {
        eprintln!("got msg: {msg:?}");
        match msg {
            Message::Request(req) => {
                if connection.handle_shutdown(&req)? {
                    return Ok(());
                }
                if req.method == "textDocument/definition" {
                    log("asked for def");
                    let resp = Response {
                        id: req.id,
                        result: Some(
                            serde_json::to_value(
                                GotoDefinitionResponse::Array(vec![]),
                            )
                            .unwrap(),
                        ),
                        error: None,
                    };
                    connection.sender.send(Message::Response(resp))?;
                }
                // let x: GotoDefinition;
                // eprintln!("got request: {req:?}");
                // match cast::<GotoDefinition>(req) {
                //     Ok((id, params)) => {
                //         eprintln!("got gotoDefinition request #{id}: {params:?}");
                //         let result = Some(GotoDefinitionResponse::Array(Vec::new()));
                //         let result = serde_json::to_value(&result).unwrap();
                //         let resp = Response {
                //             id,
                //             result: Some(result),
                //             error: None,
                //         };
                //         connection.sender.send(Message::Response(resp))?;
                //         continue;
                //     }
                //     Err(err @ ExtractError::JsonError { .. }) => panic!("{err:?}"),
                //     Err(ExtractError::MethodMismatch(req)) => req,
                // };
                // ...
            }
            Message::Response(resp) => {
                eprintln!("got response: {resp:?}");
            }
            Message::Notification(not) => {
                eprintln!("got notification: {not:?}");
            }
        }
    }
    Ok(())
}

fn main() {
    actual_main().unwrap();
}
