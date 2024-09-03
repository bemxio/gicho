# Gicho
*Gicho* (기초, "basic" in Korean) is a simple operating system running in 16-bit real mode. It is written in C and Assembly, made for learning purposes and for fun.

It has a CLI interface that supports basic low-level commands like `peek`, `poke`, etc. It can also run external code from a disk.

## Building
To build Gicho, you will need a couple of prerequisites:
- A Linux host - on Windows, you can use [WSL](https://docs.microsoft.com/en-us/windows/wsl/install) or a virtual machine.
- [`gcc-ia16`](https://github.com/tkchia/gcc-ia16) - a GCC cross-compiler for the kernel. If you use Arch Linux, you can install it from the [AUR](https://aur.archlinux.org/packages/gcc-ia16), otherwise you'll probably need to build it from source.
- [NASM](https://www.nasm.us) - an assembler for the bootloader. You should be able to install it from your package manager.
- [QEMU](https://www.qemu.org) (optional) - an emulator to run the OS. It should also be available in your package manager.

Once you have all the prerequisites, simply clone the repository, and then run `make` in the root directory of the project. This will create a `gicho.bin` file in the `build` directory.

If you installed QEMU, you can launch Gicho by running `make run`.

## Contributing
Contributions are welcome! If you want to contribute, whether it's just a simple question or a whole pull request, feel free to do so.

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
