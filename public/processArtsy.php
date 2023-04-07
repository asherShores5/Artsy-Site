<?php
ini_set('display_errors', '1');
ini_set('display_startup_errors', '1');
error_reporting(E_ALL);
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Decode the POST request
  $data = json_decode(file_get_contents("php://input"), true);

  // Validate that data isn't empty
  if ($data === null) {
    $error = 'Invalid code input.';
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }

  // Set the code variable
  $code = $data['code'];

  // Validate the input
  if (!$code) {
    // Return an error message if the input is invalid
    $error = 'Invalid code input.';
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }
  
  // Check if the user has permissions to write to each directory
  $temp_dir = sys_get_temp_dir();
  $dir = '../src/';
  // $dir_in = '../src/in/';

  if (!is_writable($temp_dir)) {
    $error = 'Cannot write to directory /tmp';
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }

  // Generate a random string
  $length = 10;
  $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
  $randomString = '';
  for ($i = 0; $i < $length; $i++) {
      $randomString .= $characters[rand(0, strlen($characters) - 1)];
  }
//$temp_dir == /tmp
  // Save the code to a file
  $filename = $temp_dir . '/' . $randomString . '.txt';
  $mode = "w";

  $filename = $temp_dir . "/helloWorld.txt";

  $file_handle = fopen($filename, $mode);

  if (!fwrite($file_handle, $code)) {
    $error = "Cannot put code into file";
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }

  // Close the file
  fclose($file_handle);

  // Check if the text file actually exists
  if (!file_exists($filename)) {
    $error = 'Input file was not created' . $filename;
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }
  
  // Execute the set of commands
  $outputText = 'make -C ' . $dir . ' artsy ' . $filename;
  $output = exec($outputText);

  // Check if the command actually executed
  if ($output === null) {
    $error = 'Error running make';
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }

  // Check if the wasm file was created
  $wasmFile = $filename . '.wasm';
  if (!file_exists($wasmFile)) {
    $error = 'Make command failed to create the file: ' . $wasmFile;
    echo json_encode(array("success" => false, "message" => $error));
    exit;
  }

  echo $wasmFile;
  exit;
}
?>
