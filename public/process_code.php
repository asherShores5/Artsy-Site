<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Set the code variable
  $code = $_COOKIE['codeInput'];

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

  // Generate a random string
  $length = 10;
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
      $randomString .= $characters[rand(0, strlen($characters) - 1)];
  }

  // Save the code to a file
  $filename = $dir_in . $randomString . '.txt';

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
  $filename = './in/' . $randomString . '.txt';
  $command = 'make artsy ' . escapeshellarg($filename);

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
  $wasmFile = $dir_in . $randomString . '.txt.wasm';
  if (!file_exists($wasmFile)) {
    // Return back error file name
    $errorFile = $dir_in . $randomString . '_error.txt';
    echo $errorFile;
    exit;
  }

  echo $wasmFile;
  exit;
}
?>
