<?php
// Start session
session_start();

// Return back previous code input from the previous session, if it exists
if (isset($_COOKIE['codeInput'])) {
    $cookieValue = $_COOKIE['codeInput'];
    // Code to use the cookie value
    echo $cookieValue;
} else {
    echo "not_found";
}
?>
