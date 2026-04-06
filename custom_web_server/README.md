# Custom Web Server (C)

A minimal multi-threaded HTTP server written in C. It accepts TCP connections, parses the first line of an HTTP request, and serves static files from the `www/` directory.

## Features

- Listens on port **8080**
- Fixed-size worker thread pool (**10** threads)
- Basic HTTP request-line parsing (method, path, version)
- Static file serving from `./www` (defaults `/` -> `/index.html`)
- Simple MIME type detection for: `.html`, `.css`, `.js`, `.png`, `.jpg`

## Project layout

- `main.c` -> server startup + accept loop
- `network.c/h` -> socket setup (`bind`, `listen`, `accept`)
- `thread_pool.c/h` -> job queue + worker threads
- `parser.c/h` -> parse the HTTP request line
- `file_io.c/h` -> serve a file from `./www` to the client
- `www/` -> place your static site files here (e.g. `index.html`)

## Build

### Linux / WSL (recommended on Windows)

From the `custom_web_server/` directory:

```sh
make
```

Clean build artifacts:

```sh
make clean
```

### Native Windows

This code uses POSIX sockets/headers and `pthread`, so it is easiest to build in **WSL**.

If you want to build without WSL, you'll need a POSIX-like toolchain (e.g. MSYS2/MinGW) and may need code changes depending on your environment.

## Run

```sh
./server
```

You should see logs like:

- `[NETWORK] Server successfully bound to port 8080`
- `[THREAD POOL] Successfully spawned 10 worker threads.`

Then visit:

- http://localhost:8080/

## Serving content

Files are served from `./www` with the request path appended.

Examples:

- `GET /` -> serves `./www/index.html`
- `GET /style.css` -> serves `./www/style.css`

If `www/` is empty (as in a fresh clone), requests will return `404 Not Found`. Add an `index.html` (and any other assets) under `www/`.

## Quick test

After adding `www/index.html`:

```sh
curl -i http://localhost:8080/
```

## Notes / limitations

- Only the request line is parsed; headers/body are ignored.
- The server will attempt to serve a file even for non-`GET` methods.
- Paths are simply concatenated onto `./www`; there is no robust path normalization (e.g. `..` handling).
