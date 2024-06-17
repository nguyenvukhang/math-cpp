use std::collections::HashMap;
use std::io::Write;
use std::path::{Path, PathBuf};
use std::{fs, io};

use anyhow::Result;
use lsp_server::{Connection, Message, Response};
use lsp_types::{
    ClientCapabilities, GotoDefinitionResponse, InitializeParams,
    ServerCapabilities, WorkspaceFolder,
};

fn handle_message(connection: &Connection, msg: Message) -> Result<()> {
    match msg {
        Message::Request(req) => {
            eprintln!("REQUEST: {req:?}");
            if connection.handle_shutdown(&req)? {
                return Ok(());
            }
            match req.method.as_str() {
                "textDocument/definition" => {
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
                _ => (),
            }
        }
        Message::Response(resp) => {
            eprintln!("RESPONSE: {resp:?}");
        }
        Message::Notification(not) => {
            eprintln!("NOTIFICATION: {not:?}");
        }
    }
    Ok(())
}

#[derive(Debug, Default)]
struct State {
    files: HashMap<PathBuf, Vec<String>>,
}

impl State {
    fn load_folder_recursively<P: AsRef<Path>>(
        &mut self,
        root: P,
    ) -> io::Result<()> {
        for entry in
            walkdir::WalkDir::new(root).into_iter().filter_map(|e| e.ok())
        {
            if entry.metadata().map_or(false, |v| v.is_file()) {
                if entry.path().extension().map_or(false, |v| v == "tex") {
                    self.update_file(entry.path())?;
                }
            }
        }
        Ok(())
    }

    fn update_file<P: AsRef<Path>>(&mut self, path: P) -> io::Result<()> {
        let mut lines = vec![];
        if let Ok(text) = fs::read_to_string(path.as_ref()) {
            for line in text.lines() {
                if line.starts_with("\\Theorem") {
                    lines.push(line.to_string());
                }
            }
        }
        self.files.insert(path.as_ref().to_path_buf(), lines);
        Ok(())
    }

    fn print_lines(&self) {
        for lines in self.files.values() {
            for line in lines {
                eprintln!("->{line}");
            }
        }
    }
}

fn actual_main() -> Result<()> {
    let mut state = State::default();
    let (connection, io_threads) = Connection::stdio();
    let (id, params) = connection.initialize_start()?;
    let init_params: InitializeParams = serde_json::from_value(params).unwrap();
    // eprintln!("INIT PARAMS {:?}", init_params);
    if let Some(workspace_folders) = init_params.workspace_folders {
        for wsf in workspace_folders {
            state.load_folder_recursively(wsf.uri.path().as_str())?;
        }
    }
    eprintln!("LINES");
    state.print_lines();

    let client_capabilities: ClientCapabilities = init_params.capabilities;
    // eprintln!("CLIENT CAPABILITIES {:?}", client_capabilities);

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
        handle_message(&connection, msg)?;
    }
    Ok(())
}

fn main() {
    actual_main().unwrap();
}
