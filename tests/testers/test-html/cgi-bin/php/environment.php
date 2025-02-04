#!/usr/bin/php-cgi

<!DOCTYPE html>
<html>
<head>
    <title>CGI Test PHP 1 - ENV</title>
</head>
<body>
    <h1>CGI Test PHP 1 - ENV</h1>

    <?php foreach ($_SERVER as $key => $value): ?>
        <p><?= str_pad(htmlspecialchars($key), 20) ?> = <?= htmlspecialchars($value) ?></p>
    <?php endforeach; ?>

</body>
</html>
