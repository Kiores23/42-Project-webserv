#!/usr/bin/php-cgi

<?php
 require_once 'session-helper.php';

   enum State {
        case Error;
        case Success;
        case NotAllowed;
    }
    $state = State::Success;
    $stdin = file_get_contents('php://input');
    parse_str($stdin, $cgiContent);
    if (!isset($cgiContent))
    {
        $state = State::NotAllowed;
    }
    else
    {
        if (isset($cgiContent['username']) && isset($cgiContent['title'])) {
            $username = $cgiContent['username'];
            $title = $cgiContent['title'];
        }
            else
                $state = State::NotAllowed;
    }
    if ($state == State::NotAllowed || !isUsernameValid($username) || usernameAlreadyTaken($username)) {
        $state = State::NotAllowed;
    }
   if ($state == State::Success)
   {
        session_start();
        if (session_status() == PHP_SESSION_ACTIVE)
        {
            addUser($username);
            setcookie("title", $title);
            setcookie("test_second_cookie", "it_works");
        }
    }
?>

<!DOCTYPE html>
<html lang='en'>
<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <meta name="robots" content="noindex, nofollow">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>Create cookie Webserv</title>
    <link rel="icon" href="https://auth.42.fr/auth/resources/yyzrk/login/students/img/favicon.ico"/>
    <link href="https://auth.42.fr/auth/resources/yyzrk/login/students/css/styles.css?v=1.2" rel="stylesheet"/>
    <style>
        body {
            text-align: center;
            width: 100vw;
            height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
     }

    .title-message {
        font-size: 3em;
    }

    .link-home {
        font-size: 1.6em;
        padding: .5em;
        color: #fff;
        background-color: #00babc;
    }
    </style>
</head>
<body>
<div class="content">
    <h1 class="title-message">
        <?php if ($state == State::Success) { ?>
            Votre session <?= $username ?> a bien été créé !
        <?php } elseif ($state == State::Error) { ?>
            Une erreur est survenue, veuillez rééssayer.
        <?php } else { ?>
            Vous n'êtes pas autorisé à utiliser cette page.
        <?php } ?>
    </h1>
    <a class="link-home" href="/">
        <?php if ($state == State::Success) { ?>
            Accéder au profil
        <?php } else { ?>
            Retourner à l'accueil
        <?php } ?>
        </a>
</div>
</body>
</html>
