// The author disclaims copyright to this source code.
#ifndef __CONF_H
#define __CONF_H


typedef long cell;
#define CELL_FMT "%ld"

// Enable colors in the terminal
// #define __ECHO_COLOR

// Uncomment for a minimal build with only mandatory extensions
// Build with:
//   make EXTRA_CFLAGS=-D__MINIMAL_BUILD EXT_SRC=
// #define __MINIMAL_BUILD

// Unsafe mode: disable stack underflow/overflow check
// #define UNSAFE_MODE

// Uncomment to display name <-> hash relationship
// #define __SHOW_MISSING_HASH

// Enable hashed names: reduce memory size but break words/see/disasemble
#define __KEEP_ENTRY_NAMES

#ifndef __MINIMAL_BUILD

// Enable register variable
#define __USE_REGISTER

// Enable dictionary clash check
#define __CHECK_DICT

#ifdef __CHECK_DICT
#define __LIVE_CHECK_DICT
#endif /* __CHECK_DICT */

// Enable exception
#define __EXCEPTION

// Use extensions
#define __USE_EXTENSIONS

// Extended math
#define __EXTENDED_MATH

#endif /* __MINIMAL_BUILD */


#define FSTACK_SIZE 30
typedef double number_t;

// MP_MATH
#define __MP_MATH

#define MPSTACK_SIZE 30

// Threads
#define __THREADS

#define MAX_THREAD 10
#define MAX_LOCK 10

// NETWORK
#define __NETWORK

// SDL
// uncomment the line below if you want to include SDL words
// #define __SDL

// Various sizes
#define STACK_SIZE 30
#define R_STACK_SIZE 30

#define DICT_SIZE 1000

#define HEAP_SIZE 10000

#define TIB_SIZE 32

// Number of bits in a char
#define CF_CHAR_BIT 8

struct state;

#endif /* __CONF_H */
