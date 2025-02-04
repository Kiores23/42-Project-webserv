#!/usr/bin/python3

# https://fr.m.wikibooks.org/wiki/Programmation_Python/L%27interface_CGI#Un_script_CGI_pour_le_traitement_des_donn%C3%A9es

import cgi
import os

print ("Content-type: text/html\r\n")
print("<html>")
print("<head>")
print("<title>CGI Test Python - get_word.py</title>")
print("</head>")
print("<body>")
print ("<h1>CGI Test Python - get_word.py</h1>")
print(os.environ)
# Check if running in a CGI context
if 'REQUEST_METHOD' in os.environ:
    form = cgi.FieldStorage()
    print( form.keys() )
    if "word" in form:
        word = form.getvalue('word')
    else:
        word = "WORD NOT DEFINED"
else:
    word = "NOT RUNNING IN CGI CONTEXT"

print("<h1>Word : %s</h1>" % word)
