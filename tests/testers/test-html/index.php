#!/usr/bin/php-cgi

<?php
require_once 'session-helper.php';

session_start();

$title = getTitle();
$username = getUser();
if (!empty($username)) {
    include 'profile.php';
}
else {
    include 'sign-up.php';
}
?>