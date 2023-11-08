# Hello_NuMaker-M032BTAI

## Building

### Build Binaries with Docker

- Get the docker image from [Docker Hub](https://hub.docker.com/r/jafee201153/arm-none-eabi-gcc) and run the following commands:

```shell
docker pull jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04
```

- Start the container:

```shell
# Mac/Linux
docker run --name arm-none-eabi-gcc-container --rm -v ${PWD}:/share -d -it jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04 sh
```

```bash
# Windows
docker run --name arm-none-eabi-gcc-container --rm -v "%cd%":/share -d -it jafee201153/arm-none-eabi-gcc:13.2-ubuntu-20.04 sh
```

- Build the binaries:

```shell
docker exec arm-none-eabi-gcc-container sh -c "cd /share && make clean && make"
```

- Stop the container:

```shell
docker stop arm-none-eabi-gcc-container
```

///

> Hint: or just run `build.bat` on Windows, `build.sh` on Mac/Linux.

## Upgrading

```bash
copy Source\build\TRSP_UART_Central.bin F:\
```
