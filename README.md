# IPC demo app

Minimal demo of building an application for an IP camera with a Linux
cross-compilation environment.

The repository intentionally mirrors the production build flow in a reduced
form:

1. The build environment provides a cross-compiler.
2. Docker calls the cross-compiler.
3. The build produces a target Linux binary.

The demo application is a tiny HTTP server. It listens on port `8080` by
default and returns `HTTP 200 OK` for every accepted TCP connection.

## Build

Build the demo app:

```sh
make
```

By default, Docker uses the standard Ubuntu ARM cross compiler:

```text
arm-linux-gnueabi-gcc
```

The output file is:

```text
build/http_200
```

To build with a local toolchain archive, keep the archive in the repository
directory and pass it as `TOOLCHAIN`:

```sh
make TOOLCHAIN=toolchain.tgz
```

To only build the Docker image:

```sh
make docker-build
```

To copy the binary from the image:

```sh
make docker-artifact
```

The default toolchain prefix is `arm-linux-`. For a toolchain with another
prefix, pass it explicitly:

```sh
make TOOLCHAIN=toolchain.tgz TOOLCHAIN_PREFIX=vendor-linux-
```

To find the prefix, list compiler binaries in the archive:

```sh
tar -tf toolchain.tgz | grep '/bin/.*gcc$'
```

For example, if the compiler is `bin/vendor-linux-gcc`, use
`TOOLCHAIN_PREFIX=vendor-linux-`.

## Run On A Camera

Copy `build/http_200` to the camera and run it:

```sh
./http_200 8080
```

Then request any path:

```sh
curl http://CAMERA_IP:8080/
```

The expected response is:

```text
OK
```
