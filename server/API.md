# API Documentation

This details the fields of the JSON requests used to communicate
between client and server.

## Login

### Client Sends
```
“{
"request-type": “LOGIN”,
"user-name": “nerd”,
"login-hash": "SHA256 81407JFHASFU1hAsdf892#$HA",
}”
```

### Server Responds
```
“{
"user-type": “ADMIN" | "STUDENT" | "DNE”,
"user-id": integer,
}”
```
