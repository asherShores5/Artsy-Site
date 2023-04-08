<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Validate and sanitize the code input
    // Decode the POST request
    $data = json_decode(file_get_contents("php://input"), true);

    // Validate that data isn't empty
    if ($data === null) {
        // Send an error response
        $response = array(
            "status" => "error",
            "message" => "Invalid code input."
        );
        echo json_encode($response);
        exit;
    }

    // Set the code variable
    $codeInput = $data['code'];

    if ($codeInput !== null) {
        // Save the code input to a cookie variable
        setcookie("codeInput", $codeInput, time() + (10 * 365 * 24 * 60 * 60), "/", "", true, true);

        // Send a success response
        $response = array(
            "status" => "success",
            "message" => "Code successfully saved!"
        );
    } else {
        // Send an error response
        $response = array(
            "status" => "error",
            "message" => "Invalid input data"
        );
    }

    // Send the response in JSON format
    header('Content-Type: application/json');
    echo json_encode($response);
}
?>
