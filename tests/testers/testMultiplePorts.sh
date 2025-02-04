#!/bin/bash

# Declarations
HOST="localhost"
PORT=8085
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

printf "$TITLE_FORMAT" "MULTIPLE PORTS"

printf "${SUBTITLE_FORMAT}" "PORT: 8084"
cmd="curl --silent --include 'localhost:8084' | grep -qi 'HTTP/1.1 200'"
isTestPassed "${cmd}"

printf "${SUBTITLE_FORMAT}" "PORT: 8085"
cmd="curl --silent --include 'localhost:8085' | grep -qi 'HTTP/1.1 200'"
isTestPassed "${cmd}"

printf "${SUBTITLE_FORMAT}" "PORT: 8086"
cmd="curl --silent --include 'localhost:8086' | grep -qi 'HTTP/1.1 200'"
isTestPassed "${cmd}"

printf "${SUBTITLE_FORMAT}" "PORT: 8087"
cmd="curl --silent --include 'localhost:8087' | grep -qi 'HTTP/1.1 200'"
isTestPassed "${cmd}"
