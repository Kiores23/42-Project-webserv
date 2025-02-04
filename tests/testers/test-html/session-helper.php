#!/usr/bin/php-cgi

<?php
$filePath = './sessions.json';

function isUsernameValid($username) {
    return !empty($username);
}

function usernameAlreadyTaken($username)
{
    global $filePath;
    $isInSessionFile = false;
    if (file_exists($filePath) && !empty($username)) {
        $fileContent = file_get_contents($filePath);
        if (empty($fileContent))
            $jsonData = [];
        else
            $jsonData = json_decode(file_get_contents($filePath), true);
        $isInSessionFile = in_array($username, $jsonData);
    }
    return ($isInSessionFile);
}

function addUser($username)
{
    global $filePath;
    $jsonData = file_exists($filePath) ? json_decode(file_get_contents($filePath), true) : [];
    $jsonData[session_id()] = $username;
    file_put_contents($filePath, json_encode($jsonData, JSON_PRETTY_PRINT));
}

function getUser()
{
    global $filePath;
    if (file_exists($filePath)) {
        $jsonData = json_decode(file_get_contents($filePath), true);
        $currentSessionId = session_id();
        if (isset($jsonData[$currentSessionId])) {
            return ($jsonData[$currentSessionId]);
        }
    }
    return "";
}

function findCookieValue($cgiCookies, $requestedKey)
{
    $cookies = explode('; ', $cgiCookies);
    $cookieData = [];
    foreach ($cookies as $cookie) {
       $parts = explode('=', $cookie, 2);
        if(count($parts) === 2) {
            list($key, $value) = $parts;
            $cookieData[$key] = $value;
        }
    }
    if (isset($cookieData[$requestedKey])) {
        return $cookieData[$requestedKey];
    }
    return "";
}

function getTitle()
{
    $title = "Cookie connnecté";
    $sessionData = [];
    $cgiHttpCookie = $_SERVER['HTTP_COOKIE'];
    if (isset($cgiHttpCookie)) {
        $cookieTitle = findCookieValue($cgiHttpCookie, 'title');
        if (!empty($cookieTitle)) {
            $title = urldecode($cookieTitle);
        }
    }
    return $title;
}

?>