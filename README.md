# SGX-Cache Trace Generator
This is a memory trace generator for the [SGX-Cache simulator](https://github.com/atskae/sgxc). This trace generator uses [OpenSGX](https://github.com/sslab-gatech/opensgx) as its base. The generated trace file can be used as an input to the SGX-Cache simulator.

This trace generator was tested on Ubuntu 18.04.4 LTS (from `lsb_release -a` command).

## Building the Trace Generator
The original build instructions (for Ubuntu only) from [here](https://github.com/sslab-gatech/opensgx) are reworded below:
1. Install the following on Ubuntu:
```
$ apt-get build-dep qemu
$ apt-get install libelf-dev
```
2. Build `qemu` by first `cd`ing into `opensgx/qemu`, then run `./configure-arch`. Afterwards, run `make -j $(nproc)`
3. `cd` back to main directory `opensgx/` by running `cd ../`
4. Compile the SGX library by running `make -C libsgx`
5. Compile the user-level code by running `make -C user`

## Generating Traces
### Compile an OpenSGX Program
1. Write an OpenSGX program in the `opensgx/user/test` directory (instructions on writing OpenSGX programs can be found [here](https://github.com/sslab-gatech/opensgx)). In this example, we will compile and run the `aes.c` program in the provided directory `opensgx/user/test/openssl/`
2. `cd` back to the main directory `opensgx/`, then compile the program by running `make -C user`

### Generate the Program Trace
1. After compiling the program, change into the `user/` directory by running `cd opensgx/user`
2. Run the trace generator by running `./test.sh test/your-program`, where `your-program` is the OpenSGX program that was written in Step 1. A trace file will be generated in the `opensgx/user/test/` directory as `your-program.out`.

For example, to run the `aes` program, run `./test.sh test/openssl/aes`. The message `Writing to file test/openssl/aes.out` will be printed to the console. This will generate the trace file `test/openssl/aes.out`. Generating the `aes.out` trace takes approximately 3 minutes.

Running the command `head test/openssl/aes.out` will display the first few lines of the generated `aes.out` trace:
```
60.521000 0 0x4000a3f090 2
0.014000 0 0x4000a3f093 2
0.125000 0 0x4000a3fea0 2
0.002000 0 0x4000a3fea1 2
0.001000 0 0x4000a3fea4 2
0.001000 0 0x4000a3fea6 2
```
Each line has the format `<timestamp> <enclave mode> <memory address> <type>`, where `<type>` indicates whether the memory reference was an instruction fetch (`2`), load (`0`), or store (`1`).

## Modified Files from the Original OpenSGX
* Files in `/opensgx/qemu/target-i386`
    * `sgx_helper.c`
        * Added two new functions, `write_to_trace()` and `helper_access_icache()`
        * Modified `helper_mem_access()`, which captures `LOADS`/`STORE` instructions, to call the added function `write_to_trace()`
    * `translate.c`
        * Added new function `wrapper_access_icache()`
        * Modified `disas_insn()` to call the added function `wrapper_access_icache()`
    * `helper.h`
        * Registered the new helper function as `DEF_HELPER_3(access_icache, void, env, int, tl)`
    * `cpu.h`
        * Modified the struture `CPUX86State` to hold trace file name and timestamp information
* `/opensgx/qemu/linux-user/main.c`
    * Modifed `main()` to open a trace file prior to the `cpu_loop(env)` call toward the end of the `main()` function
* `opensgx/qemu/tcg/tcg-plugin.c`
    * Modifed `tcg_plugin_after_gen_tb()` to save the trace file name
    * Modified `cpus_stopped()` to close the trace file.
* `.gitignore`
    * Added `user/test/openssl/*.out` to ignore the generated traces files when commiting to the repository

### Other Modified Files
These files were modified due to build errors from the original OpenSGX
* Files with `Unescaped left brace in regex is illegal here in regex;` errors
    * Added an escape character `\` before every left brace
    * Error found in `opensgx/qemu/scripts/texi2pod.pl`
* Files with `dereferencing pointer to incomplete type ‘struct ucontext’` errors
    * Replaced all instances of `struct ucontext *uc = puc` with `ucontext_t *uc = puc`
    * Error found in `/opensgx/qemu/user-exec.c`
    * [Solution source](https://lore.kernel.org/patchwork/patch/851352/)

## Added Files
Provided are sample OpenSGX programs of different crypto algorithms. These programs were added in the `opensgx/user/test/openssl` directory.
