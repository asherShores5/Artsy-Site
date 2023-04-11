<?php
$file_path = '/tmp/test_file.txt';
$file_content = 'This is a test file';

// Open the file for writing
$file_handle = fopen($file_path, 'w');

// Write the content to the file
fwrite($file_handle, $file_content);

// Close the file
fclose($file_handle);

echo "File created successfully!";
?>
