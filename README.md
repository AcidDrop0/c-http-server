
# C HTTP Server

A simple HTTP server written in C that listens locally (default port 6767).  
This project was built for learning purposes — mainly to understand sockets, threading, and basic HTTP request handling.

---

## How to build

Compile using:

```bash
gcc main.c http_utils.c handlers.c -lpthread -o server
```

> Adjust file paths depending on your folder structure.

---

## How to run

```bash
./server --directory /your/path/to/files/
```

**Important:**  
Start **and** end the directory path with `/`  
Example:
```
./server --directory /home/you/files/
```

---

## What the server can do

### 1) Get the user-agent

```bash
curl -v http://localhost:6767/user-agent
```
The server will return the user agent string that curl sends.

### 2) Echo a message

```bash
curl -v http://localhost:6767/echo/hello
```
Returns back `hello` as the response body.

### 3) Download a file from the directory

```bash
curl -v http://localhost:6767/files/example.txt
```

OR in browser:  
`http://localhost:6767/files/example.txt`  
(Your browser will download it)

### 4) Upload a file (POST)

```bash
curl -v -X POST --data-binary "@file.txt" http://localhost:6767/files/file.txt
```

This writes `file.txt` into the server’s specified directory — as long as the file size is under 2MB (text files only for now).

---

## Notes

- This project is intentionally very simple
- There’s no proper security implemented yet — treat this as experimental / learning code
- File sending is currently done with a naïve string formatting approach — proper streaming is planned for future improvements

---

## License

No license specified yet — feel free to explore but don’t use this in production.
