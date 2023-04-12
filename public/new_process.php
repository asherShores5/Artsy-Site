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

  // Trim whitespaces
  $code = trim($code);
  
  // Execute the set of commands in a new working directory
  $working_dir = '../src/';
  chdir($working_dir);
  // $command = 'make artsy ARGS=\"' . $code . "\"";

  $command = "make run ARGS=\"" . $code . "\"";
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

  echo $output;
  exit;
}
?>