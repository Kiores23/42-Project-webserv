#!/usr/bin/php-cgi

<?php
require_once 'session-helper.php';

parse_str($_SERVER['QUERY_STRING'], $queryString) ?? '';
$username = $queryString['username'] ?? '';
$response = ['cantSubmit' => true];
$response['message'] = "";
if (isUsernameValid($username)) {
    $response['cantSubmit'] = usernameAlreadyTaken($username);
    if ($response['cantSubmit'])
        $response['message'] = "Username is already taken.";
}
else {
    $response['message'] = "Invalid username.";
}

header('Content-Type: application/json');
echo json_encode($response);
?>