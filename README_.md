# Hello_NuMaker-M032BTAI

## TL;DR

This repository demonstrates how to develop projects for the `NuMaker-M032BTAI` using `gcc-arm-none-eabi` with a `makefile`.

We use `pyOCD` for flashing and debugging because `OpenOCD` does not fully support the `NuMaker-M032BTAI`.

The example used is [SampleCode/NuMaker-M03xBT/BLE/Demo/Central/TRSP_UART_Central](https://github.com/OpenNuvoton/M031BSP/tree/master/SampleCode/NuMaker-M03xBT/BLE/Demo/Central/TRSP_UART_Central).

Use `make help` to quickly see available commands.

## Requirements

Before proceeding with the commands, ensure you have the following installed:

- Required
  - `make`
  - `gcc-arm-none-eabi 13.2.Rel1+` or `docker`
- Optional
  - To install `pyOCD`:
    1. Install `Python 3.7+` and `pip`
    1. Use `make install` to install `pyOCD` and required setup
  - To build documentation:
    1. Install `node.js 18+` and `npm`
    1. Install `pandoc` and `tinytex`
    1. Use `make docs-install` to install required extensions

### Windows

```powershell
choco install -y make gcc-arm-embedded python3 nodejs-lts pandoc tinytex
```

### Linux / macOS

```bash
# Linux
sudo apt update && sudo apt install -y curl git
```

```bash
# macOS
brew install coreutils curl git
```

```bash
git clone https://github.com/asdf-vm/asdf.git ~/.asdf --branch v0.14.0
echo ". $HOME/.asdf/asdf.sh" >> ~/.bashrc
echo ". $HOME/.asdf/completions/asdf.bash" >> ~/.bashrc
source ~/.bashrc
```

```bash
asdf plugin add make
asdf plugin add gcc-arm-none-eabi
asdf plugin add python
asdf plugin add nodejs
asdf plugin add pandoc
asdf plugin add tinytex
```

```bash
asdf install make latest
asdf install gcc-arm-none-eabi latest
asdf install python latest
asdf install nodejs 18.18.0
asdf install pandoc latest
asdf install tinytex latest
```

```bash
asdf global make latest
asdf global gcc-arm-none-eabi latest
asdf global python latest
asdf global nodejs 18.18.0
asdf global pandoc latest
asdf global tinytex latest
```

## Quick Start

- Compile

```bash
make
```

- Recompile

```bash
make clean && make
```

```bash
make rebuild
```

- Flash

```bash
make flash
```

- Recompile and Flash

```bash
make upgrade
```

- Generate Documentation

```bash
make docs
```

## Debugging with `pyOCD`

To debug the firmware manually, use the following commands:

### Starting the GDB Server

Start the GDB server with `pyOCD`:

```bash
pyocd gdbserver -t M032BTAIAAN --elf Source\build\TRSP_UART_Central.elf
```

### Connecting GDB to the Target

In the GDB command line, connect to the target and load the firmware:

```bash
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
```

## More Information

- [OpenNuvoton/M031BSP](https://github.com/OpenNuvoton/M031BSP)
