use std::fs;

use anyhow::Result;
use lsp_server::Connection;
use lsp_server::Message;
use lsp_server::Response;
use lsp_types::ClientCapabilities;
use lsp_types::GotoDefinitionResponse;
use lsp_types::InitializeParams;
use lsp_types::ServerCapabilities;

fn actual_main() -> Result<()> {
    let (connection, io_threads) = Connection::stdio();
    let (id, params) = connection.initialize_start()?;
    let init_params: InitializeParams = serde_json::from_value(params).unwrap();
    let client_capabilities: ClientCapabilities = init_params.capabilities;

    let mut server_capabilities = ServerCapabilities::default();
    server_capabilities.definition_provider = Some(lsp_types::OneOf::Left(true));

    let initialize_data = serde_json::json!({
        "capabilities": server_capabilities,
        "serverInfo": {
            "name": "lsp-server-test",
            "version": "0.1"
        }
    });

    connection.initialize_finish(id, initialize_data)?;
    fs::write("/home/khang/repos/mylsp/log.txt", "Completed init")?;

    for msg in &connection.receiver {
        eprintln!("got msg: {msg:?}");
        match msg {
            Message::Request(req) => {
                if connection.handle_shutdown(&req)? {
                    return Ok(());
                }
                if req.method == "textDocument/definition" {
                    let resp = Response {
                        id: req.id,
                        result: Some(
                            serde_json::to_value(GotoDefinitionResponse::Array(vec![])).unwrap(),
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
