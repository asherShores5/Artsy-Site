<?php
//echo 'PHP File Started';
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST");

$code = $_POST["code"];
$filename = "../src/input.artsy"; // Change this to the name of the file you want to save to
// file_put_contents($filename, $code);

$file = fopen($filename, "w");
fwrite($file, $code);
fclose($file);
?>
