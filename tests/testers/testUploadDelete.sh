#!/bin/bash

PORT=8085
TITLE_FORMAT="\n\n==================== %s ====================\n"
SUBTITLE_FORMAT="\n---------- %s ----------\n"
ROOT_UPLOAD="localhost:$PORT/upload_files"

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

printf "$TITLE_FORMAT" "UPLOAD/DELETE"

printf "${SUBTITLE_FORMAT}" "UPLOAD FILES"
cmd="curl --silent --include -X POST -F "file=@./testUploadFile.txt" '$ROOT_UPLOAD' | grep -qi 'HTTP/1.1 201'"
isTestPassed "$cmd"

cmd="curl --silent --include -X POST -F 'file1=@./testUploadFile.txt' -F 'file2=@./testUploadFile.txt' '$ROOT_UPLOAD' | grep -qi 'HTTP/1.1 201'"
isTestPassed "$cmd"


printf "${SUBTITLE_FORMAT}" "DELETE FILES"
cmd="curl --silent --include -X DELETE '$ROOT_UPLOAD/testUploadFile.txt' | grep -qi 'HTTP/1.1 204'"
isTestPassed "$cmd"

cmd="curl --silent --include -X DELETE '$ROOT_UPLOAD/testUploadFile(1).txt' | grep -qi 'HTTP/1.1 204'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "DELETE FORBIDDEN TO DELETION FILE"

chmod 000 './test-html/upload_files/testUploadFile(2).txt'

cmd="curl --silent --include -X DELETE '$ROOT_UPLOAD/testUploadFile(2).txt' | grep -qi 'HTTP/1.1 403'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "DELETE NON EXISTING FILE"
cmd="curl --silent --include -X HEAD '$ROOT_UPLOAD/bob' | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "CHECK DELETED FILES ARE REMOVED"
cmd="curl --silent --include -X HEAD '$ROOT_UPLOAD/testUploadFile.txt' | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

cmd="curl --silent --include -X HEAD '$ROOT_UPLOAD/testUploadFile(1).txt' | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "CHECK FORBIDDEN TO DELETION FILE IS STILL HERE"
cmd="curl --silent --include -X HEAD '$ROOT_UPLOAD/testUploadFile(2).txt' | grep -qi 'HTTP/1.1 403'"
isTestPassed "$cmd"

printf "${SUBTITLE_FORMAT}" "ADD RIGHTS TO FORBIDDEN TO DELETION FILE AND DELETE IT"

chmod 777 './test-html/upload_files/testUploadFile(2).txt'

cmd="curl --silent --include -X DELETE '$ROOT_UPLOAD/testUploadFile(2).txt' | grep -qi 'HTTP/1.1 204'"
isTestPassed "$cmd"

cmd="curl --silent --include -X HEAD '$ROOT_UPLOAD/testUploadFile(2).txt' | grep -qi 'HTTP/1.1 404'"
isTestPassed "$cmd"