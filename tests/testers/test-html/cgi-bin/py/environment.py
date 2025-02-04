#!/usr/bin/python3

import os

print("Content-type: text/html\r\n")
print("<html>")
print("<head>")
print("<title>CGI Test Python 1 - ENV</title>")
print("</head>")
print("<body>")
print("<h1>CGI Test Python 1 - ENV</h1>")
for param in os.environ.keys():
    print("<p>%20s = %s</p>" % (param, os.environ[param]))
print("</body>")
print("</html>")
