# Hello_NuMaker-M032BTAI

## Prerequisites

Before proceeding with the commands, ensure you have the following
installed:

- `make`
- `gcc-arm-none-eabi` or `docker`
- `Python 3.7+` and `pip` (for flashing and debugging)

## Quick Commands

The following quick commands are available for convenience:

- `make upgrade`: Executes `make clean`, `make`, and `make flash`
  sequentially.

## Building the Firmware

### Using Makefile

To build the firmware using Makefile, run:

``` bash
make clean && make
```

## Flashing the Firmware

We use [pyOCD](https://pypi.org/project/pyocd/) for cross-platform
compatibility and to support the latest versions.
[OpenOCD](https://github.com/OpenNuvoton/OpenOCD-Nuvoton) is not used in
this process.

### Installing pyOCD

First, install `pyOCD` using `pip`:

``` bash
python -m pip install -U pyocd
```

### Installing the Required Pack

Next, install the pack for the target MCU:

``` bash
pyocd pack install M032BTAIAAN
```

### Flashing the MCU

Finally, flash the MCU with the following command:

``` bash
make flash
```

## Debugging the Firmware

To debug the firmware manually, use the following commands:

### Starting the GDB Server

Start the GDB server with `pyOCD`:

``` bash
pyocd gdbserver -t M032BTAIAAN --elf Source\build\TRSP_UART_Central.elf
```

### Connecting GDB to the Target

In the GDB command line, connect to the target and load the firmware:

``` bash
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
```

## Documentation

To build the documentation, run:

``` bash
make docs
```
