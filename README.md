<p align="center">
  <img width="512" height=auto src="https://github.com/BanceDev/tinyvm/blob/main/tinyvm.png">
  <br/>
  <img src="https://img.shields.io/github/contributors/bancedev/tinyvm" alt="contributors">
  <img src="https://img.shields.io/github/license/bancedev/tinyvm" alt="license">
  <img src="https://img.shields.io/github/forks/bancedev/tinyvm" alt="forks">
</p>

---

# TinyVM

TinyVM is an implementaion of a very minimal computer as a way to learn about computer architecture and how instruction sets for CPUs can be emulated. The TinyCPU being virtualized has only 6 registers, 4Kb of ROM memory, and 4 instructions. The entire VM is written in just over 100 lines of C code and is all in a single file. I hope this VM is able to teach people a little about computer architecture, assembly languages, and high level CPU design.

## Compiling/Installation

```
git clone https://github.com/BanceDev/tinyvm.git
cd tinyvm
gcc -o tinyvm tinyvm.c
```

## License

This project is licensed under the GPLv3 License.

For more details, see the [LICENSE](./LICENSE) file.
