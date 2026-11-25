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

/* Row normalization for the 8, 16 and 32 bit coefficient types: multiply a
 * row by the inverse of its leading coefficient, so that it starts with 1.
 *
 * Template, included once by each of la_ff_8.c, la_ff_16.c and la_ff_32.c
 * with these macros defined (and undefined again at the end of this file):
 *   LA_NORM_CF_T       the coefficient type (cf8_t, cf16_t, cf32_t)
 *   LA_NORM_FF(name)   the name of the instance (name##_ff_8, ...)
 *   LA_NORM_INV(a, p)  the inverse of a modulo the prime p
 *
 * The coefficients and the inverse are below p < 2^32, so their product is
 * computed exactly in 64 bits for every width. The 64 bit field has its own
 * normalization in la_ff_64.c. */

/* Normalize the dense row of length len. */
static inline LA_NORM_CF_T *LA_NORM_FF(normalize_dense_matrix_row)(
        LA_NORM_CF_T *row,
        const hm_t len,
        const uint32_t fc
        )
{
    len_t i;

    const hm_t os       = len % UNROLL;
    const uint64_t p    = fc;
    const uint64_t inv  = LA_NORM_INV(row[0], fc);

    for (i = 1; i < os; ++i) {
        row[i]  = (LA_NORM_CF_T)(((uint64_t)row[i] * inv) % p);
    }
    /* we need to set i to os since os < 1 is possible */
    for (i = os; i < len; i += UNROLL) {
        row[i]    = (LA_NORM_CF_T)(((uint64_t)row[i] * inv) % p);
        row[i+1]  = (LA_NORM_CF_T)(((uint64_t)row[i+1] * inv) % p);
        row[i+2]  = (LA_NORM_CF_T)(((uint64_t)row[i+2] * inv) % p);
        row[i+3]  = (LA_NORM_CF_T)(((uint64_t)row[i+3] * inv) % p);
    }
    row[0]  = 1;

    return row;
}

/* Normalize the sparse coefficient array of length len, whose first os
 * entries are handled before the unrolled loop. */
static inline LA_NORM_CF_T *LA_NORM_FF(normalize_sparse_matrix_row)(
        LA_NORM_CF_T *row,
        const len_t os,
        const len_t len,
        const uint32_t fc
        )
{
    len_t i;

    const uint64_t p    = fc;
    const uint64_t inv  = LA_NORM_INV(row[0], fc);

    for (i = 0; i < os; ++i) {
        row[i]  = (LA_NORM_CF_T)(((uint64_t)row[i] * inv) % p);
    }
    /* we need to set i to os since os < 1 is possible */
    for (i = os; i < len; i += UNROLL) {
        row[i]    = (LA_NORM_CF_T)(((uint64_t)row[i] * inv) % p);
        row[i+1]  = (LA_NORM_CF_T)(((uint64_t)row[i+1] * inv) % p);
        row[i+2]  = (LA_NORM_CF_T)(((uint64_t)row[i+2] * inv) % p);
        row[i+3]  = (LA_NORM_CF_T)(((uint64_t)row[i+3] * inv) % p);
    }
    row[0]  = 1;

    return row;
}

#undef LA_NORM_CF_T
#undef LA_NORM_FF
#undef LA_NORM_INV
