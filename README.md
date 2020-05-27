# SGX-Cache Trace Generator
This is a memory trace generator for the [SGX-Cache simulator](https://github.com/atskae/sgxc). This trace generator uses [OpenSGX](https://github.com/sslab-gatech/opensgx) as its base. The generated trace file can be used as an input to the SGX-Cache simulator.

This trace generator was tested on Ubuntu 18.04.4 LTS (from `lsb_release -a` command).

## Building the Trace Generator
Original build instructions from [here](https://github.com/sslab-gatech/opensgx) reproduced below:
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
1. Write an OpenSGX program in the `opensgx-cache/user/test` directory (instructions on writing OpenSGX programs can be found [here](https://github.com/sslab-gatech/opensgx))
2. Compile the program by running `make -C user`
3. Change into the correct directory by running `cd opensgx-cache/user`
4. Run the trace generator by running `./test.sh test/your-program`, where `your-program` is the OpenSGX program that was compiled in Step 1. A trace file will be generated in the `opensgx-cache/user/test/` directory as `your-program.out`

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

### Other Modified Files
These files were modified due to build errors from the original OpenSGX
* Files with `Unescaped left brace in regex is illegal here in regex;` errors
    * Added an escape character `\` before every left brace
    * Error found in `opensgx/qemu/scripts/texi2pod.pl`
* Files with `dereferencing pointer to incomplete type ‘struct ucontext’` errors
    * Replaced all instances of `struct ucontext *uc = puc` with `ucontext_t *uc = puc`
    * Error found in `/opensgx/qemu/user-exec.c`
    * [Solution source](https://lore.kernel.org/patchwork/patch/851352/)
