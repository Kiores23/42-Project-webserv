#!/bin/bash

TITLE_FORMAT="\n\n==================== %s ====================\n"
SUBTITLE_FORMAT="\n---------- %s ----------\n"

function isTestPassed()
{
      eval "$1"
      if [ $? -eq 0 ]; then
          echo "✅ Test passed: $1"
      else
          echo "❌ Test failed: $1"
      fi
}

PORT=8085
# Script execution
if ! lsof -i:${PORT} > /dev/null 2>&1; then
    echo "❌ webserv is not listening to port ${PORT}, try again."
    exit 1
fi

printf "$TITLE_FORMAT" "SERVER NAMES"
printf "$SUBTITLE_FORMAT" "server_name amery elombard avan;"
cmd="curl --silent --include --resolve amery:$PORT:127.0.0.1 http://amery:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve avan:$PORT:127.0.0.1 http://avan:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve elombard:$PORT:127.0.0.1 http://elombard:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve aavan:$PORT:127.0.0.1 http://aavan:$PORT | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve coucou:$PORT:127.0.0.1 http://coucou:$PORT | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

PORT=8091
printf "$SUBTITLE_FORMAT" "server_name amery;"
cmd="curl --silent --include --resolve amery:$PORT:127.0.0.1 http://amery:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve avan:$PORT:127.0.0.1 http://avan:$PORT | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve elombard:$PORT:127.0.0.1 http://elombard:$PORT | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

PORT=8090
printf "$SUBTITLE_FORMAT" "server_name redirection.test;"
cmd="curl --silent --include --resolve redirection.test:$PORT:127.0.0.1 http://redirection.test:$PORT | grep -qi 'HTTP/1.1 301'"
isTestPassed "$cmd"

PORT=8086
printf "$SUBTITLE_FORMAT" "server_name google.com test.google.com salut.google.com"
cmd="curl --silent --include --resolve google.com:$PORT:127.0.0.1 http://google.com:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve test.google.com:$PORT:127.0.0.1 http://test.google.com:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

cmd="curl --silent --include --resolve salut.google.com:$PORT:127.0.0.1 http://salut.google.com:$PORT | grep -qi 'HTTP/1.1 200'"
isTestPassed "$cmd"

