#!/bin/bash

# Declarations
HOST="localhost"
PORT=8085
ADDRESS="http://$HOST:$PORT"
TITLE_FORMAT="\n\n==================== %s ====================\n"
SUBTITLE_FORMAT="\n---------- %s ----------\n"

function isTestPassed()
{
      eval "$1"
      if [ $? -eq 0 ]; then
          echo "✅ Test passed: $1"
          return 1;
      else
          echo "❌ Test failed: $1"
          return 0;
      fi
}

# Script execution
if ! lsof -i:${PORT} > /dev/null 2>&1; then
    echo "❌ webserv is not listening to port ${PORT}, try again."
    exit 1
fi

# Give rights to tests CGI scripts to be executed
chmod -R 777 test-html/cgi-bin

runCGITests() {
    local lang=$1
    local cgi_path="$ADDRESS/cgi-bin"
    local cgi_lang_path="${cgi_path}/${lang}"

    printf "$TITLE_FORMAT" "CGI: ${lang^^}"

    printf "${SUBTITLE_FORMAT}" "GET: CHECK RIGHTS"
    cmd="! (curl --silent -X GET '${cgi_lang_path}/helloworld.${lang}' | grep -qi '#!')"
    if isTestPassed "${cmd}"; then
      echo "❌ Can't test CGI for language ${lang}, the language extension is not allowed. \
Please add it in the configuration."
      return;
    fi

    printf "${SUBTITLE_FORMAT}" "GET: HELLO WORLD"
    cmd="(curl --silent -X GET '${cgi_lang_path}/helloworld.${lang}' | grep -qi 'hello world') \
&& ! (curl --silent -X GET '${cgi_lang_path}/helloworld.${lang}' | grep -qi 'Content-type')"
    isTestPassed "${cmd}"

    printf "${SUBTITLE_FORMAT}" "GET: STDIN WITH NO DATA"
    cmd="curl --silent --include -X GET '${cgi_lang_path}/stdin.${lang}' | grep -qi 'Server has no data to send'"
    isTestPassed "${cmd}"

    printf "${SUBTITLE_FORMAT}" "POST: STDIN WITH DATA"
    cmd="curl --silent --include -X POST -H 'Content-Type: text/plain' \
--data 'Ceci est un message pour le CGI' \
'${cgi_lang_path}/stdin.${lang}' | grep -qi 'HTTP/1.1 200'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X POST -H 'Content-Type: text/plain' \
--data 'Ceci est un message pour le CGI' \
'${cgi_lang_path}/stdin.${lang}' | grep -qi 'Ceci est un message pour le CGI'"
    isTestPassed "${cmd}"

    printf "${SUBTITLE_FORMAT}" "GET: ENVIRONMENT"
    cmd="curl --silent --include -X GET \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'webserv'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X GET \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'HTTP/1.1 200'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X GET \
'${cgi_lang_path}/environment.${lang}' | grep -qi '${PORT}'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X GET \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'GET'"
    isTestPassed "${cmd}"

    printf "${SUBTITLE_FORMAT}" "POST: ENVIRONMENT"
    cmd="curl --silent --include -X POST \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'webserv'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X POST \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'HTTP/1.1 200'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X POST \
'${cgi_lang_path}/environment.${lang}' | grep -qi '${PORT}'"
    isTestPassed "${cmd}"

    cmd="curl --silent --include -X POST \
'${cgi_lang_path}/environment.${lang}' | grep -qi 'POST'"
    isTestPassed "${cmd}"

    printf "${SUBTITLE_FORMAT}" "POST: NOT ALLOWED"
    cmd="curl --silent --include -X POST \
'${cgi_path}/method-not-allowed/stdin.${lang}' | grep -qi 'HTTP/1.1 405'"
    isTestPassed "${cmd}"
}

runCGITests py # Python
runCGITests pl # Perl
runCGITests php # PHP
