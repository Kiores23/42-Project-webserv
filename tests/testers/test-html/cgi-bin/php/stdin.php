#!/usr/bin/php-cgi

<?php
$timeout = 3;

$startTime = time();
$inputData = '';

$inputData = file_get_contents("php://input");

echo "<html><head><title>CGI Test PHP - stdin.php</title></head><body>\n";
echo "<h1>CGI Test PHP - stdin.php</h1><p>\n";

if ($inputData === "") {
    echo "Server has no data to send\n";
} else {
    echo "Server sent data: " . htmlspecialchars($inputData) . "\n";
}

if ($inputData === '' && ((time() - $startTime) >= $timeout)) {
    echo "Server sent no data received within $timeout seconds.\n";
}

echo "</p></body></html>\n";
?>