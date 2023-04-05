<?php
// $file = 'example-programs/run.artsy';
// $textToWrite = $_POST['textToWrite'];

// file_put_contents($file, $textToWrite);

$code = $_POST["code"];
$filename = "../src/input.artsy"; // Change this to the name of the file you want to save to
file_put_contents($filename, $code);

?>
