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

  // Set the code variable
  $code = $data['code'];

  // Validate the input
  if (!$code) {
    // Return an error message if the input is invalid
    http_response_code(400);
    echo 'Invalid input';
    exit;
  }
  
  // Check if the user has permissions to write to each directory
  $dir = '../src/';
  $dir_in = '../src/in/';

  if (!is_writable($dir)) {
    http_response_code(500);
    echo 'Cannot write to directory /src';
    exit;
  } else if (!is_writable($dir_in)) {
    http_response_code(500);
    echo 'Cannot write to directory /src/in';
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
  $file = $dir . '/in/' . $randomString . '.txt';
  if (!file_put_contents($file, $code)) {
    http_response_code(500);
    echo 'Error saving file';
    exit;
  }
  
  // Execute the set of commands
  $outputText = 'make -C ' . $dir . ' artsy ./in/' . $randomString . '.txt';
  $output = exec($outputText);

  // Check if the command actually executed
  if ($output === null) {
    echo 'Error running make';
    exit;
  }

  // Check if the wasm file was created
  if (!file_exists($dir_in . $randomString . '.txt.wasm')) {
    echo 'Make command failed to create the file: ' . $randomString . '.txt.wasm';
    exit;
  }

  echo $randomString . '.txt';
  exit;
}
?>
