execute_commands() {
    ./testCGI.sh
    ./testCookies.sh
    ./testHttpCodes.sh
    ./testHttpMethods.sh
    ./testMultiplePorts.sh
    ./testRedirections.sh
    ./testServerNames.sh
    ./testSiege.sh
    ./testUploadDelete.sh
}

if [ $# -eq 0 ]; then
    execute_commands | grep "❌"
  else
    execute_commands
fi
