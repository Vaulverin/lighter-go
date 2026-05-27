#if defined(__linux__)
#define _GNU_SOURCE
#include "signal_stack.h"

#include <signal.h>
#include <stddef.h>
#include <sys/mman.h>
#include <unistd.h>

#ifndef MAP_STACK
#define MAP_STACK 0
#endif

#define LIGHTER_SIGALTSTACK_SIZE (1024U * 1024U)

static __thread void* lighter_sigaltstack_mapping;
static __thread size_t lighter_sigaltstack_mapping_size;

void lighter_ensure_large_sigaltstack(void) {
    stack_t current;
    if (sigaltstack(NULL, &current) == 0) {
        if ((current.ss_flags & SS_DISABLE) == 0 && current.ss_size >= LIGHTER_SIGALTSTACK_SIZE) {
            return;
        }
    }

    long page_size_long = sysconf(_SC_PAGESIZE);
    size_t page_size = page_size_long > 0 ? (size_t)page_size_long : 4096U;
    size_t total_size = page_size + LIGHTER_SIGALTSTACK_SIZE;
    void* mapping = mmap(NULL, total_size, PROT_READ | PROT_WRITE,
                         MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (mapping == MAP_FAILED) {
        return;
    }

    if (mprotect(mapping, page_size, PROT_NONE) != 0) {
        munmap(mapping, total_size);
        return;
    }

    stack_t next;
    next.ss_sp = (char*)mapping + page_size;
    next.ss_size = LIGHTER_SIGALTSTACK_SIZE;
    next.ss_flags = 0;
    if (sigaltstack(&next, NULL) != 0) {
        munmap(mapping, total_size);
        return;
    }

    if (lighter_sigaltstack_mapping != NULL) {
        munmap(lighter_sigaltstack_mapping, lighter_sigaltstack_mapping_size);
    }
    lighter_sigaltstack_mapping = mapping;
    lighter_sigaltstack_mapping_size = total_size;
}
#else
#include "signal_stack.h"

void lighter_ensure_large_sigaltstack(void) {
}
#endif
