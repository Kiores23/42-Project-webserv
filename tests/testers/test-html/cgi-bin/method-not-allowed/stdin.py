#!/usr/bin/python3
import sys
import select

timeout = 3  # Timeout en secondes
ready, _, _ = select.select([sys.stdin], [], [], timeout)
print("Content-type: text/html\r\n")
print("<html>")
print("<head>")
print("<title>CGI Test Python - stdin.py</title>")
print("</head>")
print("<body>")
print("<h1>CGI Test Python - stdin.py</h1><p>")
if ready:
    input_data = sys.stdin.read()
    if input_data == "":
        print("Server has no data to send")
    else:
        print("Server sent data: ", input_data)
else:
    print("Server has sent no data received within", timeout, "seconds.")
print("<p></body>")
print("</html>")
