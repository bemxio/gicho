# Gicho
*Gicho* (기초, "basic" in Korean) is a simple operating system running in 16-bit real mode. Written in C and Assembly, made primarily for learning purposes and for fun.

It has a CLI interface that supports basic low-level commands like `peek` and `poke`. It can also call BIOS interrupts, read and write data from/to a disk, as well as execute other programs.

## Building
To build Gicho, you will need a couple of prerequisites:
- **A Linux host system** - while it is theoretically possible to set up the environment on other OSes, it is not supported. On Windows, you can use [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).
- **[`gcc-ia16`](https://github.com/tkchia/gcc-ia16)** - if you're on an Arch-based distro, you can install it from the [AUR](https://aur.archlinux.org/packages/gcc-ia16). Otherwise, you'll probably need to build it from source.
- **[GNU make](https://www.gnu.org/software/make)** - you should be able to install it from your package manager, e.g. `apt install make` for Debian-based distros.
- **[NASM](https://www.nasm.us)** - it should also be available in your package manager.
- **[QEMU](https://www.qemu.org) (optional)** - in case you want to run or debug the OS, it's recommended to use QEMU. Like with Make and NASM, the package manager should have it.

1. Get the source code. You can either download the ZIP archive by pressing the *Code > Download ZIP* button, or clone the repository with [Git](https://git-scm.com) by running `git clone https://github.com/bemxio/gicho`.
2. Ensure all variables in the [`Makefile`](Makefile) are correct. You can change GCC, NASM, and QEMU paths if they are different on your system, as well as the path of the disk image.
3. Run `make` in the root directory of the project. This will compile the kernel and the bootloader, link them together, and create an image file you can run.

## Running
### QEMU
The [`Makefile`](Makefile) has a target for running the OS in QEMU. If you installed it earlier, just run `make run` and it should start Gicho up in a new window.

In case you want to start it up manually, you can execute the following command:
```sh
qemu-system-i386 -drive file=<image path>,format=raw
```
with `<image path>` being the path to the disk image. By default, it's `build/gicho.img`.

### VirtualBox
It is also possible to run Gicho in VirtualBox. To do so, create a new VM, set whatever settings you want, then go to the *Storage* tab in the settings, add a floppy controller and attach the disk image to it. Start the VM and it should boot right up.

### Real Hardware
Gicho is a hit-or-miss when it comes to running on real hardware. **It is unable to run on UEFI PC's with no legacy boot support.** If you have a computer that supports it, you can write the image file onto a USB drive using tools like [`dd`](https://www.gnu.org/software/coreutils/dd) or [balenaEtcher](https://www.balena.io/etcher), and then boot from it.

Here's an example of how to write the image using `dd`:
```sh
dd if=<image path> of=/dev/sdX status=progress
```
where `<image path>` is the path to the disk image and `/dev/sdX` is the USB drive. **Make sure you have specified the correct device, as this command will overwrite data on it.**

## Usage
Gicho has 9 available commands:
- `help` - displays a list of available commands.
- `echo [text]` - prints the specified text to the screen.
- `clear` - clears the screen.
- `peek <address>` - reads a byte at a specified address in memory.
- `poke <address> <value>` - writes a byte to the specified memory address.
- `int <interrupt> [registers]` - calls a BIOS interrupt with optional register values, in the format of space-separated assignments (e.g. `ah=0 al=0x03 bx=0x0449`).
- `read <drive number> <sector amount> [offset] [memory address]` - reads sectors from a disk into memory. By default, the offset is 0 and the memory address is `0x7e00`.
- `write <drive number> <sector amount> [memory address] [offset]` - writes sectors from memory to a disk. THe defaults are the same as for `read`.
- `run [address]`- executes a program at the specified address. By default, it's `0x7e00`.

The image file contains two sample programs, `hello.asm` and `rainbow.asm`. You can run them by first reading them into memory by running `read 0x80 1 18`, then executing one of them with the `run` command for `hello.asm` and `run 0x7e29` for `rainbow.asm`.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Contributions are welcome! If you want to contribute, whether it's an issue you've encountered or a pull request with new features, feel free to do so.