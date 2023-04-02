<?php
$file = 'example-programs/run.artsy';
$textToWrite = $_POST['textToWrite'];

file_put_contents($file, $textToWrite);
?>
