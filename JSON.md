https://dev.mysql.com/doc/connector-cpp/en/connector-cpp-examples-results.html

https://github.com/nlohmann/json

# Login

## Client Sends
```
“{
"packet-type": “LOGIN”,
"user-name": “nerd”,
"login-hash": "SHA256 81407JFHASFU1hAsdf892#$HA",
}”
```

## Server Responds
```
“{
"user-type": “ADMIN" | "STUDENT" | "DNE”,
"user-id": integer,
}”
```
