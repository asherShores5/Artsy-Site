<?php
// header("Access-Control-Allow-Origin: *");
// header("Access-Control-Allow-Methods: POST");

// $code = $_POST["code"];
// $filename = "../src/input.artsy"; // Change this to the name of the file you want to save to
// file_put_contents($filename, $code);

  // Get the code from the POST request
  $code = $_POST["code"];

  // Open the file for writing (this will create a new file if it doesn't exist)
  $file = fopen("../src/input.artsy", "w");

  // Write the code to the file
  fwrite($file, $code);

  // Close the file
  fclose($file);
?>
