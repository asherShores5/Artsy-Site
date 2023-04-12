<?php
namespace MyNamespace;

// Load environment variables
$aws_access_key_id = getenv('AWS_ACCESS_KEY_ID');
$aws_secret_access_key = getenv('AWS_SECRET_ACCESS_KEY');
$s3_bucket = getenv('S3_BUCKET');

use Aws\S3\S3Client;
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  // Create an instance of the AWS SDK for PHP
  require 'vendor/autoload.php';

  // Retrieve the saved code from the S3 bucket
  $s3 = new S3Client([
    'version' => 'latest',
    'region' => 'us-east-1',
    'credentials' => [
      'key' => $aws_access_key_id,
      'secret' => $aws_secret_access_key,
    ],
  ]);
  $bucket = 'artsy-project';
  $key = 'my-file.artsy';
  try {
    $result = $s3->getObject([
      'Bucket' => $bucket,
      'Key' => $key,
    ]);
    $code = $result['Body'];
  } catch (Aws\S3\Exception\S3Exception $e) {
    // Send an error response
    $response = array(
        "status" => "error",
        "message" => "Error retrieving code from S3 bucket.",
    );
    echo json_encode($response);
    exit;
  }

  // Check if the code is empty
  if (empty($code)) {
    // Send an error response
    $response = array(
        "status" => "error",
        "message" => "Invalid code input.",
    );
    echo json_encode($response);
    exit;
  }

  // Save the code to a temporary file
  $filename = tempnam(sys_get_temp_dir(), 'code_');
  file_put_contents($filename, $code);

  // Execute the set of commands in a new working directory
  $dir = dirname($filename);
  chdir($dir);
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
  $wasmFile = $dir . '/out.wasm';
  if (!file_exists($wasmFile)) {
    // Return back error file name
    $errorFile = $dir . '/error.txt';
    echo $errorFile;
    exit;
  }

  echo $wasmFile;
  exit;
}
?>
