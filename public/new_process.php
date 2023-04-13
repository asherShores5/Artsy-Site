<?php
require('../vendor/autoload.php');

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

  // Trim whitespaces
  $code = trim($code);
  
  // Execute the set of commands in a new working directory
  $working_dir = '../src/';
  chdir($working_dir);
  $command = "make artsy ARGS=\"" . $code . "\"";
  $output = shell_exec($command);

  // Check if the command actually executed
  if ($output === null) {
    $response = array(
      "status" => "error",
      "message" => "Error running make", 
    );
    echo json_encode($response);
    exit;
  }

  // $wat_text = strstr($output, "(module");
  // echo $wat_text;
  echo $output;

  // Convert WebAssembly Text to WebAssembly Binary
  // $wasmText = '(module (func (export "sum") (param i32) (param i32) (result i32) (i32.add (local.get 0) (local.get 1))))';
  // $waswmText = file_get_contents("../src/WATcode.wat");
  // $wasmBytes = exec("python3 ../src/get_wasm.py '$wasmText'");
  // echo $wasmBytes;
  exit;
}
?>