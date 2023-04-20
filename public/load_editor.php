<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Decode the POST request
    $data = json_decode(file_get_contents("php://input"), true);

    // Validate that data isn't empty
    if ($data === null) {
        http_response_code(400);
        echo 'Invalid input';
        exit;
    }

    // Set the uuid variable
    $uuid = $data['uuid'];

    // Check if the user has permissions to write to the directory
    $dir_in = '../src/in/';

    if (!is_writable($dir_in)) {
        $response = array(
        "status" => "error",
        "message" => "Cannot write to directory /src/in",
        );
        echo json_encode($response);
        exit;
    }

    // Check if a wasm file or an error file exists
    $wasmFile = $dir_in . $uuid . '.wasm';
    $errorFile = $dir_in . $uuid . '_error.txt';
    if (file_exists($wasmFile)) {
        echo $wasmFile;
    } else if (file_exists($errorFile)) {
        echo $errorFile;
    } else {
        echo 'File not found';
    }
}
?>