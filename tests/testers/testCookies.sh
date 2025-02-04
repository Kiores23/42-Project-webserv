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

printf "$TITLE_FORMAT" "COOKIES"

printf "${SUBTITLE_FORMAT}" "COOKIE title"
cmd="curl --silent --include -X POST --location '$ADDRESS/create-cookie.php' \
--header 'Cookie: PHPSESSID=1' \
-H 'Content-Type: application/x-www-form-urlencoded' \
-d 'username=test1&title=test' | grep -qi 'Set-Cookie: title=test'"
isTestPassed "${cmd}"

printf "${SUBTITLE_FORMAT}" "COOKIE test_second_cookie"
cmd="curl --silent --include -X POST --location '$ADDRESS/create-cookie.php' \
--header 'Cookie: PHPSESSID=2' \
-H 'Content-Type: application/x-www-form-urlencoded' \
-d 'username=test2&title=test' | grep -qi 'Set-Cookie: test_second_cookie=it_works'"
isTestPassed "${cmd}"

rm test-html/sessions.json
