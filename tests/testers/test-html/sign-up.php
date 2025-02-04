#!/usr/bin/php-cgi

<!DOCTYPE html>
<html lang='en'>

<head>
    <meta charset="utf-8">
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>
    <meta name="robots" content="noindex, nofollow">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <title>Sign in to Webserv</title>
    <link rel="icon" href="resources/favicon.ico"/>
    <link href="resources/sign-up.css" rel="stylesheet"/>
    <style>
        #kc-form-buttons input[type="submit"]:disabled {
            background-color: #008789;
            color: #dcdcdc;
        }
    </style>
</head>
<body>
<div id="kc-content">
    <div id="kc-content-wrapper">
        <div id="kc-form">
            <img id="logo" src="resources/42_logo.png"/>
            <div id="kc-form-wrapper">
                <form id="kc-form-login" action="/create-cookie.php" method="POST" enctype="application/x-www-form-urlencoded">
                    <div>
                        <p id="usernameFeedback"><p>
                        <input tabindex="1" id="username" name="username" type="text"
                               autofocus autocomplete="off"
                               placeholder="Login" />
                    </div>
                    <div>
                        <input tabindex="2" id="password" name="title" type="text"
                               autocomplete="off"
                               placeholder="Cool title (optional)" />
                    </div>
                    <div id="kc-form-buttons">
                        <input tabindex="4" id="submitButton" type="submit" value="Sign In"/>
                    </div>
                </form>
            </div>
        </div>
    </div>
</div>

<script>
    document.getElementById('username').addEventListener('blur', function() {
        var username = this.value;
        var feedbackElement = document.getElementById('usernameFeedback');
        var submitButton = document.getElementById('submitButton');
        fetch('/check_username.php?username=' + encodeURIComponent(username))
            .then(response => response.json())
            .then(data => {
                console.log('data: ' + JSON.stringify(data));
                if(data.cantSubmit) {
                   feedbackElement.textContent = data.message;
                    submitButton.disabled = true;
                } else {
                    feedbackElement.textContent = '';
                    submitButton.disabled = false;
                }
            })
            .catch(error => console.error('Error:', error));
    });
</script>
</body>
</html>
