<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $data = json_decode(file_get_contents("php://input"), true);
  if ($data === null) {
    http_response_code(400);
    echo 'Invalid input';
    exit;
  }
  $message = $data['message'];
  echo 'Received message: ' . $message;
}
?>
