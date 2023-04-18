<?php
    ini_set('display_errors', '1');
    ini_set('display_startup_errors', '1');
    error_reporting(E_ALL);
    session_start();
    // header("Cache-Control: no-cache, must-revalidate");    
    header('Location: public/index.html');
    phpinfo();
    exit;
?>
