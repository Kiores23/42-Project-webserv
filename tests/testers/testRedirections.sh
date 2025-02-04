#!/bin/bash

PORT=8085
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

# Script execution
if ! lsof -i:${PORT} > /dev/null 2>&1; then
    echo "❌ webserv is not listening to port ${PORT}, try again."
    exit 1
fi

printf "$TITLE_FORMAT" "REDIRECTIONS"

printf "${SUBTITLE_FORMAT}" "301 REDIRECTION"
cmd="curl --silent --include 'localhost:$PORT/redir/test_301' | grep -qi 'HTTP/1.1 301'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "308 REDIRECTION"
cmd="curl --silent --include 'localhost:$PORT/redir/test_308' | grep -qi 'HTTP/1.1 308'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "308 TO 301 REDIRECTION"
cmd="curl --location --max-redirs 1 --silent --include 'localhost:$PORT/redir/test_chain' \
| grep -qi 'HTTP/1.1 301'"
isTestPassed "$cmd"

cmd="curl --location --max-redirs 1 --silent --include 'localhost:$PORT/redir/test_chain' \
| grep -qi 'HTTP/1.1 308'"
isTestPassed "$cmd"
