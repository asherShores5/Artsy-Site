<?php
require('vendor/autoload.php');

use Dotenv\Dotenv;
use Aws\S3\S3Client;
use Aws\Exception\AwsException;

$dotenv = Dotenv::createImmutable(__DIR__);
$dotenv->load();

// This will simply read AWS_ACCESS_KEY_ID and AWS_SECRET_ACCESS_KEY from env vars
$s3 = new S3Client([
    'version'      => 'latest',
    'region'       => 'us-west-1',
    'credentials'  => [
        'key'    => getenv('AWS_ACCESS_KEY_ID'),
        'secret' => getenv('AWS_SECRET_ACCESS_KEY'),
    ],
]);

$bucket = getenv('S3_BUCKET') ?: die('No "S3_BUCKET" config var in found in env!');
$key = uniqid() . '.txt'; // replace with the name of the object you want to upload
$content = 'This is the content of your file.';

try {
    $result = $s3->putObject([
        'Bucket' => $bucket,
        'Key'    => $key,
        'Body'   => $content,
    ]);
    echo "YAY! IT UPLOADED!!! File: " . $key;
} catch (AwsException $e) {
    // Handle the error
    echo 'Error connecting to S3: ' . $e->getMessage();
}