<?php
// Start the session if it hasn't been started already
session_start();

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Decode the POST request
  $data = json_decode(file_get_contents("php://input"), true);

  // Validate that data isn't empty
  if ($data === null) {
    http_response_code(400);
    echo 'Invalid input';
    exit;
  }

  // Set the code variable
  $code = $data['code'];

  // Validate the input
  if (!$code) {
    // Send an error response
    $response = array(
        "status" => "error",
        "message" => "Invalid code input."
    );
    echo json_encode($response);
    exit;
  }
  
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

  // Get the session ID
  $sessionId = session_id();

  // Save the code to a file
  $filename = $dir_in . $sessionId . '.txt';

  if (!file_put_contents($filename, $code)) {
    $response = array(
      "status" => "error",
      "message" => "Cannot put code into file", 
    );
    echo json_encode($response);
    exit;
  }

  // Check if the text file actually exists
  if (!file_exists($filename)) {
    $error = 'Input file was not created' . $filename;
    $response = array(
      "status" => "error",
      "message" => $error, 
    );
    echo json_encode($response);
    exit;
  }
  
  // Execute the set of commands in a new working directory
  $dir = '../src/';
  chdir($dir);
  $command = "make artsy ARGS=\"" . $sessionId . "\"";

  $output = shell_exec(escapeshellcmd($command));

  // Check if the command actually executed
  if ($output === null) {
    $response = array(
      "status" => "error",
      "message" => "Error running make", 
    );
    echo json_encode($response);
    exit;
  }

  // Check if the wasm file was created
  $wasmFile = $dir_in . $sessionId . '.wasm';
  if (!file_exists($wasmFile)) {
    // Return back error file name
    $errorFile = $dir_in . $sessionId . '_error.txt';
    echo $errorFile;
    exit;
  }

  echo $wasmFile;
  exit;
}
?>
