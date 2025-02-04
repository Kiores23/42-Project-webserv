#!/bin/bash

# Declarations
HOST="127.0.0.1"
PORT=8085
ADDRESS="http://$HOST:$PORT"
TITLE_FORMAT="\n\n==================== %s ====================\n"
SUBTITLE_FORMAT="\n---------- %s ----------\n"
resultFile="siege_output.txt"

function isValidJSON() {
    if jq empty "$1" > /dev/null 2>&1; then
        echo "true"
    else
        echo "false"
    fi
}


function siegeTest()
{
    eval "$1"

    cat $resultFile
    if [[ $(isValidJSON ${resultFile}) == "true" ]]; then
        availability=$(jq -r '.availability' ${resultFile} | xargs)
    else
        availability=$(grep "Availability" ${resultFile} | awk '{print $2}' | tr -d '%')
    fi

    rm ${resultFile}

    if (( $(echo "$availability >= 99" | bc -l) )); then
          echo "✅ Test passed: $1: Availability: ${availability}"
          return 1;
      else
          echo "❌ Test failed: $1: Availability: ${availability}"
          return 0;
    fi
}

# Script execution
if ! lsof -i:${PORT} > /dev/null 2>&1; then
    echo "❌ webserv is not listening to port ${PORT}, try again."
    exit 1
fi

printf "$TITLE_FORMAT" "SIEGE"

resultFile="siege_output.txt"
seconds=3
numberUsers=1

# Using siege for the first time to avoid messages
siege --concurrent=1 --time=1s -b http://127.0.0.1:8085/ > /dev/null 2>&1

printf "${SUBTITLE_FORMAT}" "${seconds} seconds, ${numberUsers} concurrent users"
cmd="siege --concurrent=${numberUsers} --time=${seconds}s -b ${ADDRESS}/ > ${resultFile} 2>&1"
cat ${resultFile}
siegeTest "${cmd}"

numberUsers=10
printf "${SUBTITLE_FORMAT}" "${seconds} seconds, ${numberUsers} concurrent users"
cmd="siege --concurrent=${numberUsers} --time=${seconds}s -b ${ADDRESS}/ > ${resultFile} 2>&1"
siegeTest "${cmd}"

numberUsers=100
printf "${SUBTITLE_FORMAT}" "${seconds} seconds, ${numberUsers} concurrent users"
cmd="siege --concurrent=${numberUsers} --time=${seconds}s -b ${ADDRESS}/ > ${resultFile} 2>&1"
siegeTest "${cmd}"

numberUsers=255
printf "${SUBTITLE_FORMAT}" "${seconds} seconds, ${numberUsers} concurrent users"
cmd="siege --concurrent=${numberUsers} --time=${seconds}s -b ${ADDRESS}/ > ${resultFile} 2>&1"
siegeTest "${cmd}"
