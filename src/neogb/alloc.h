/* This file is part of msolve.
 *
 * msolve is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * msolve is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with msolve.  If not, see <https://www.gnu.org/licenses/> */

/* Checked allocation for the neogb engine.
 *
 * gb_malloc, gb_calloc and gb_realloc have the same signatures and semantics
 * as malloc, calloc and realloc, except that a failed allocation of a nonzero
 * size does not return NULL: it reports the request and its call site on
 * stderr and terminates the process with GB_EXIT_OUT_OF_MEMORY.  Without this,
 * an exhausted address space (for example under `ulimit -v`) surfaces as a
 * segmentation fault at some later dereference, far from its cause.
 *
 * Requests of size zero are passed through unchanged, so code that relies on
 * malloc(0) or realloc(p, 0) behaves exactly as before. */

#ifndef GB_ALLOC_H
#define GB_ALLOC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Exit status for allocation failure, distinct from the engine's other
 * error exits (which use 1) so that callers can tell the two apart. */
#define GB_EXIT_OUT_OF_MEMORY 75

static void gb_out_of_memory_(
        const size_t nmemb,
        const size_t size,
        const char *file,
        const int line
        )
{
    fprintf(stderr,
            "msolve: out of memory: allocation of %zu x %zu bytes failed (%s:%d)\n",
            nmemb, size, file, line);
    exit(GB_EXIT_OUT_OF_MEMORY);
}

static inline void *gb_malloc_(
        const size_t size,
        const char *file,
        const int line
        )
{
    void *p = malloc(size);
    if (p == NULL && size != 0) {
        gb_out_of_memory_(1, size, file, line);
    }
    return p;
}

static inline void *gb_calloc_(
        const size_t nmemb,
        const size_t size,
        const char *file,
        const int line
        )
{
    void *p = calloc(nmemb, size);
    if (p == NULL && nmemb != 0 && size != 0) {
        gb_out_of_memory_(nmemb, size, file, line);
    }
    return p;
}

static inline void *gb_realloc_(
        void *ptr,
        const size_t size,
        const char *file,
        const int line
        )
{
    void *p = realloc(ptr, size);
    if (p == NULL && size != 0) {
        gb_out_of_memory_(1, size, file, line);
    }
    return p;
}

/* Fails the same way for a NULL result of an allocator that reports failure
 * by returning NULL (aligned allocation, for instance). */
static inline void *gb_check_alloc_(
        void *p,
        const size_t size,
        const char *file,
        const int line
        )
{
    if (p == NULL && size != 0) {
        gb_out_of_memory_(1, size, file, line);
    }
    return p;
}

#define gb_malloc(size)          gb_malloc_((size), __FILE__, __LINE__)
#define gb_calloc(nmemb, size)   gb_calloc_((nmemb), (size), __FILE__, __LINE__)
#define gb_realloc(ptr, size)    gb_realloc_((ptr), (size), __FILE__, __LINE__)
#define gb_check_alloc(p, size)  gb_check_alloc_((p), (size), __FILE__, __LINE__)

#endif
