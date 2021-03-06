/*
 * Copyright (C) 2019 Intel Corporation.  All rights reserved.
 * SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Enclave_t.h"
#include "wasm_export.h"
#include "libc_wasi_benchmark.h"

#define GLOBAL_HEAP_SIZE (60 * 1024 * 1024)
#define HEAP_SIZE (1024 * 1024 * 4)
#define STACK_SIZE (1024 * 1024 * 5)

typedef long int clock_t;

static wasm_module_inst_t module_inst = NULL;
static wasm_exec_env_t exec_env = NULL;

#define LOOKUP_WASM_FUNC(func_name) \
    wasm_function_inst_t ptr_##func_name = NULL; \
    if(!(ptr_##func_name = wasm_runtime_lookup_function(module_inst, #func_name, NULL))){ \
        ocall_print("The "#func_name" wasm function is not found.\n"); \
        return; \
    }

#define CALL_WASM_FUNC(func_ptr) \
    start = enclave_clock(); \
    if (!wasm_runtime_call_wasm(exec_env, ptr_##func_ptr, func_argc, func_argv)){ \
        snprintf(output_buff, sizeof(output_buff), "call wasm function "#func_ptr" failed. %s\n", wasm_runtime_get_exception(module_inst)); \
        ocall_print(output_buff); \
        return; \
    } \
    clock_t t_##func_ptr = enclave_clock() - start;


extern "C" {
    typedef void (*os_print_function_t)(const char* message);
    extern void os_set_print_function(os_print_function_t pf);

    void enclave_print(const char *message)
    {
        ocall_print(message);
    }
    
    typedef int (*os_clock_gettime_t)(clockid_t clk_id, struct timespec *ts);
    extern void os_set_clock_gettime(os_clock_gettime_t pf);

    int enclave_clock_gettime(clockid_t clk_id, struct timespec *ts)
    {
        int retval;
        ocall_clock_gettime(&retval, clk_id, ts);
        return retval;
    }

    typedef clock_t (*os_clock_t)();
    void os_set_clock(os_clock_t pf);

    clock_t enclave_clock(void)
    {
        clock_t time;
        ocall_clock(&time);
        return time;
    }
}


void perform_benchmark(int database_type)
{
    char output_buff[300];
    clock_t start;

    // Resolve benchmark functions
    LOOKUP_WASM_FUNC(speedtest1_initialize);
    LOOKUP_WASM_FUNC(speedtest1_launch100)
    LOOKUP_WASM_FUNC(speedtest1_launch110)
    LOOKUP_WASM_FUNC(speedtest1_launch120)
    LOOKUP_WASM_FUNC(speedtest1_launch130)
    LOOKUP_WASM_FUNC(speedtest1_launch140)
    LOOKUP_WASM_FUNC(speedtest1_launch142)
    LOOKUP_WASM_FUNC(speedtest1_launch145)
    LOOKUP_WASM_FUNC(speedtest1_launch160)
    LOOKUP_WASM_FUNC(speedtest1_launch161)
    LOOKUP_WASM_FUNC(speedtest1_launch170)
    LOOKUP_WASM_FUNC(speedtest1_launch180)
    LOOKUP_WASM_FUNC(speedtest1_launch190)
    LOOKUP_WASM_FUNC(speedtest1_launch210)
    LOOKUP_WASM_FUNC(speedtest1_launch230)
    LOOKUP_WASM_FUNC(speedtest1_launch240)
    LOOKUP_WASM_FUNC(speedtest1_launch250)
    LOOKUP_WASM_FUNC(speedtest1_launch260)
    LOOKUP_WASM_FUNC(speedtest1_launch270)
    LOOKUP_WASM_FUNC(speedtest1_launch280)
    LOOKUP_WASM_FUNC(speedtest1_launch290)
    LOOKUP_WASM_FUNC(speedtest1_launch300)
    LOOKUP_WASM_FUNC(speedtest1_launch320)
    LOOKUP_WASM_FUNC(speedtest1_launch400)
    LOOKUP_WASM_FUNC(speedtest1_launch410)
    LOOKUP_WASM_FUNC(speedtest1_launch500)
    LOOKUP_WASM_FUNC(speedtest1_launch510)
    LOOKUP_WASM_FUNC(speedtest1_launch520)
    LOOKUP_WASM_FUNC(speedtest1_launch980)
    LOOKUP_WASM_FUNC(speedtest1_launch990)

    // Call the functions to perform the benchmark
    // Declare the buffer for the arguments passing
    unsigned int func_argc = 2;
    unsigned int func_argv[2];

    // Initialize the speedtest1
    func_argc = 1;
    func_argv[0] = database_type;
    CALL_WASM_FUNC(speedtest1_initialize);

    // Execute the speed tests
    func_argc = 0;

    CALL_WASM_FUNC(speedtest1_launch100)
    CALL_WASM_FUNC(speedtest1_launch110)
    CALL_WASM_FUNC(speedtest1_launch120)
    CALL_WASM_FUNC(speedtest1_launch130)
    CALL_WASM_FUNC(speedtest1_launch140)
    CALL_WASM_FUNC(speedtest1_launch142)
    CALL_WASM_FUNC(speedtest1_launch145)
    CALL_WASM_FUNC(speedtest1_launch160)
    CALL_WASM_FUNC(speedtest1_launch161)
    CALL_WASM_FUNC(speedtest1_launch170)
    CALL_WASM_FUNC(speedtest1_launch180)
    CALL_WASM_FUNC(speedtest1_launch190)
    CALL_WASM_FUNC(speedtest1_launch210)
    CALL_WASM_FUNC(speedtest1_launch230)
    CALL_WASM_FUNC(speedtest1_launch240)
    CALL_WASM_FUNC(speedtest1_launch250)
    CALL_WASM_FUNC(speedtest1_launch260)
    CALL_WASM_FUNC(speedtest1_launch270)
    CALL_WASM_FUNC(speedtest1_launch280)
    CALL_WASM_FUNC(speedtest1_launch290)
    CALL_WASM_FUNC(speedtest1_launch300)
    CALL_WASM_FUNC(speedtest1_launch320)
    CALL_WASM_FUNC(speedtest1_launch400)
    CALL_WASM_FUNC(speedtest1_launch410)
    CALL_WASM_FUNC(speedtest1_launch500)
    CALL_WASM_FUNC(speedtest1_launch510)
    CALL_WASM_FUNC(speedtest1_launch520)
    CALL_WASM_FUNC(speedtest1_launch980)
    CALL_WASM_FUNC(speedtest1_launch990)

    snprintf(output_buff, sizeof(output_buff), "%d, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld, %ld\n",
        database_type, t_speedtest1_launch100, t_speedtest1_launch110, t_speedtest1_launch120, t_speedtest1_launch130, t_speedtest1_launch140, t_speedtest1_launch142, t_speedtest1_launch145,
        t_speedtest1_launch160, t_speedtest1_launch161, t_speedtest1_launch170, t_speedtest1_launch180, t_speedtest1_launch190, t_speedtest1_launch210,t_speedtest1_launch230, 
        t_speedtest1_launch240, t_speedtest1_launch250, t_speedtest1_launch260, t_speedtest1_launch270, t_speedtest1_launch280, t_speedtest1_launch290, t_speedtest1_launch300, 
        t_speedtest1_launch320, t_speedtest1_launch400, t_speedtest1_launch410, t_speedtest1_launch500, t_speedtest1_launch510, t_speedtest1_launch520, t_speedtest1_launch980,
        t_speedtest1_launch990);
    ocall_print(output_buff);
}

void ecall_benchmark(uint8_t* wasm_buffer, size_t wasm_buffer_len, int database_type)
{
    os_set_print_function(enclave_print);
    os_set_clock_gettime(enclave_clock_gettime);
    os_set_clock(enclave_clock);
    
    uint8_t *wasm_file_buf = NULL;
    int wasm_file_size;
    wasm_module_t wasm_module = NULL;
    char error_buf[128];

    RuntimeInitArgs init_args;
    memset(&init_args, 0, sizeof(RuntimeInitArgs));

    void* global_heap_buf = malloc(GLOBAL_HEAP_SIZE);
    memset(global_heap_buf, 0, GLOBAL_HEAP_SIZE);

    init_args.mem_alloc_type = Alloc_With_Pool;
    init_args.mem_alloc_option.pool.heap_buf = global_heap_buf;
    init_args.mem_alloc_option.pool.heap_size = GLOBAL_HEAP_SIZE;

    /* initialize runtime environment */
    if (!wasm_runtime_full_init(&init_args)) {
        ocall_print("Init runtime environment failed.");
        ocall_print("\n");
        return;
    }

    // Set empty argc and argv
    char *wasm_argv[] = { NULL };
    uint32_t wasm_argc = 0;

    // Define the directories that the WASI calls can interact with
    const char *dir_list[] = { ".", NULL };
    uint32_t dir_list_size = 1;

    // Define the environment variables that the WASI calls can query
    const char *env_list[] = { NULL };
    uint32_t env_list_size = 0;

    /* load WASM byte buffer from the argument */
    wasm_file_buf = wasm_buffer;
    wasm_file_size = wasm_buffer_len;

    /* load WASM module */
    if (!(wasm_module = wasm_runtime_load(wasm_file_buf, wasm_file_size,
                                          error_buf, sizeof(error_buf)))) {
        ocall_print(error_buf);
        ocall_print("\n");
        goto fail1;
    }

    wasm_runtime_set_wasi_args(wasm_module, dir_list, dir_list_size, NULL, 0,
                               env_list, env_list_size, wasm_argv, wasm_argc);

    /* instantiate the module */
    if (!(module_inst = wasm_runtime_instantiate(wasm_module,
                                                      STACK_SIZE,
                                                      HEAP_SIZE,
                                                      error_buf,
                                                      sizeof(error_buf)))) {
        ocall_print(error_buf);
        ocall_print("\n");
        goto fail2;
    }
    
    exec_env = wasm_runtime_create_exec_env(module_inst, STACK_SIZE);
    if(!exec_env) {
        ocall_print("Create wasm execution environment failed.\n");
        goto fail2;
    }

    /* execute the main function of wasm app */
    wasm_application_execute_main(module_inst, wasm_argc, wasm_argv);

    const char *exception;
    if ((exception = wasm_runtime_get_exception(module_inst))) {
        ocall_print(exception);
        ocall_print("\n");
    }

    // Perform the benchmark
    perform_benchmark(database_type);

    /* destroy the module instance */
    wasm_runtime_deinstantiate(module_inst);

fail2:
    /* unload the module */
    wasm_runtime_unload(wasm_module);

fail1:
    /* destroy runtime environment */
    wasm_runtime_destroy();

    free(global_heap_buf);
}