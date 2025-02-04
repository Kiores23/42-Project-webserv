#!/bin/bash

# Declarations
testsRoot="./test-html"
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

runMethodRightsTests()
{
      local method=$1

        printf "${SUBTITLE_FORMAT}" "${method^^} /test-methods/only-${method,,}-allowed"
        cmd="(! (curl --silent --include -X ${method^^} '$ADDRESS/test-methods/only-${method,,}-allowed' \
| grep -qi 'HTTP/1.1 405'))"
        isTestPassed "${cmd}"

        printf "${SUBTITLE_FORMAT}" "${method^^} /test-methods/only-${method,,}-disallowed"
        cmd="curl --silent --include -X ${method^^} '$ADDRESS/test-methods/only-${method,,}-disallowed' \
| grep -qi 'HTTP/1.1 405'"
        isTestPassed "${cmd}"
}

runGetTests() {
      local method="GET"
      printf "$TITLE_FORMAT" "METHOD: ${method}"

        printf "${SUBTITLE_FORMAT}" "${method} /"
        cmd="curl --silent --include -X ${method} '$ADDRESS' | grep -qi 'HTTP/1.1 200'"
        isTestPassed "${cmd}"
      runMethodRightsTests ${method};
}

runPostTests() {
      local method="POST"
      printf "$TITLE_FORMAT" "METHOD: ${method}"

      runMethodRightsTests ${method};
}

runDeleteTests() {
      local method="DELETE"
      printf "$TITLE_FORMAT" "METHOD: ${method}"

      touch ${testsRoot}/test-methods/only-delete-allowed
      runMethodRightsTests ${method};

      touch ${testsRoot}/test-methods/only-delete-allowed
      printf "${SUBTITLE_FORMAT}" "${method^^} /test-methods/only-${method,,}-allowed"
      cmd="(curl --silent --include -X ${method^^} '$ADDRESS/test-methods/only-${method,,}-allowed' \
| grep -qi 'HTTP/1.1 204')"
      isTestPassed "${cmd}"
      touch ${testsRoot}/test-methods/only-delete-allowed
}

runHeadTests() {
      local method="HEAD"
      printf "$TITLE_FORMAT" "METHOD: ${method}"

        printf "${SUBTITLE_FORMAT}" "${method} /"
        cmd="curl --silent --include -X ${method} '$ADDRESS' | grep -qi 'HTTP/1.1 200'"
        isTestPassed "${cmd}"
      runMethodRightsTests ${method};
}

runGetTests
runPostTests
runDeleteTests
runHeadTests
