### Launch all execution tests (webserv must be started with tests/configuration-files/tester.conf)
```
./tester.sh {any_argument_to_show_program_output}
```

### Launch all tests (webserv must be stopped)
```
./testConfig.sh
```

### Use siege to stress test the server


```
siege --concurrent=1 --time=10s -b http://127.0.0.1:8085/
```

```
siege --concurrent=10 --time=10s -b http://127.0.0.1:8085/
```

```
siege --concurrent=100 --time=10s -b http://127.0.0.1:8085/
```

```
siege --concurrent=255 --time=10s -b http://127.0.0.1:8085/
```

`--concurrent/-c : set the concurrent number of users`  
`--time/-t : specifies the amount of time each user should run`  
`--benchmark/-b : benchmark mode, there is no delay between iterations`  
`--get : show the header transaction`


## Optional
### See a request and its response using siege
```
siege --get "http://127.0.0.1:8085/"
```