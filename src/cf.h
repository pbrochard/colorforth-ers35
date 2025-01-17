// The author disclaims copyright to this source code.
#ifndef __COLORFORTH_H
#define __COLORFORTH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "conf.h"
#include "color.h"

typedef uint32_t hash_t;
typedef hash_t opcode_t;

// terminal input buffer
struct tib
{
  char buf[TIB_SIZE];
  size_t len;
};

enum entry_mode {
  CORE,
  CALL,
};

struct entry;
struct entry
{
  opcode_t opcode;
  enum entry_mode mode;
#ifdef __KEEP_ENTRY_NAMES
  char *name;
  size_t name_len;
#endif /* __KEEP_ENTRY_NAMES */
  // offset to find code in the heap
  cell offset;
};

struct stack
{
  // stack
  cell *cells;
  // stack position
  int sp;
  int lim;
};

#ifdef __EXTENDED_MATH
struct fstack
{
  // stack
  number_t *cells;
  // stack position
  int sp;
  int lim;
};
#endif /* __EXTENDED_MATH */


#ifdef __MP_MATH

#include <gmp.h>

struct mpstack
{
  // circular stack
  mpz_t *cells;
  // stack position
  int sp;
  int lim;
};

#endif /* __MP_MATH */

struct dictionary
{
  struct entry *entries;
  cell latest;
};

struct state
{
  void (*color)(struct state *s);
  struct stack *stack;
  struct stack *r_stack;
  struct tib tib;
  struct dictionary dict;

  uint8_t *heap;
  cell here;

  cell pc;

  char base;

  char done;
  char echo_on;

  // streams
  char *str_stream;
  FILE *file_stream;

  // extended math extension
#ifdef __EXTENDED_MATH
  struct fstack fstack;
#endif

  // mp math extension
#ifdef __MP_MATH
  struct mpstack mpstack;
#endif
};

struct prefix_map
{
  char c;
  void (*fn)(struct state *state);
  char *color;
};

extern struct prefix_map prefix_map[];

enum errors {
  STATE_ERROR                =   0,
  STACK_ERROR                =   1,
  STACK_INIT_ERROR           =   2,
  RSTACK_ERROR               =   3,
  RSTACK_INIT_ERROR          =   4,
  DICT_ERROR                 =   5,
  HEAP_ERROR                 =   7,

  PRIMITIVE_ERROR            =  11,
  DEFINE_ERROR               =  12,
  DUPLICATE_HASH_ERROR       =  13,

  CALLOC_ERROR               = 100,
  F_STACK_ERROR              = 120,

  F_LITERAL_ERROR            = 121,

  MP_STACK_INIT_ERROR        = 110,
  MP_LITERAL_ERROR           = 111,

  THREAD_CLONE_STATE_ERROR   = 130,
  THREAD_STACK_ERROR         = 131,
  THREAD_INIT_STACK_ERROR    = 132,
  THREAD_RSTACK_ERROR        = 133,
  THREAD_INIT_RSTACK_ERROR   = 134
};

extern hash_t hash(char *str);

extern void cf_print_cell(struct state *state, cell cell);

extern void push(struct stack *stack, const cell n);
extern cell pop(struct stack *stack);

extern cell find_entry(struct state *state, struct dictionary *dict);
//extern struct entry* find_entry_by_code(struct dictionary *dict, struct code *code);
//extern struct entry* find_entry_by_fn(struct dictionary *dict, struct code *code);
extern void unknow_word (struct state *s);

extern void compile_code(struct state *s, cell opcode, cell value);

extern void quit(struct state *state);
extern struct state* colorforth_newstate(void);
extern void free_state(struct state* state);
extern void reset_state(struct state *s);

extern void parse_colorforth(struct state *state, int c);
extern void parse_from_string(struct state *s, char *str);
extern void parse_space(struct state *s);
extern void clear_tib (struct state *s);

extern void init_lib(struct state *state);

extern void *cf_calloc(struct state *state, size_t nmemb, size_t size, unsigned char id);


#define CELLS s->stack->cells
// SP is the current cell in CELLS
#define SP s->stack->sp
#define LIM s->stack->lim

#define R_CELLS s->r_stack->cells
// R_SP is the current cell in R_CELLS
#define R_SP s->r_stack->sp
#define R_LIM s->r_stack->lim

#define PC s->pc

#define HEAP(offset, type) *((type *) (s->heap + (offset)))
#define PUT(value, type) { HEAP(s->here, type) = (type) (value); }
#define STORE(value, type) { PUT(value, type); s->here += sizeof(type); }

#define ENTRY(index) (&s->dict.entries[(index)])

#ifndef UNSAFE_MODE
#define ENSURE_STACK_MIN_GEN(x, sp, msg) if (sp < x - 1) { cf_printf(NULL, msg); show_stacktrace(s); return; }
#define ENSURE_STACK_MAX_GEN(x, sp, lim, msg) if (sp > lim - x) { cf_printf(NULL, msg);  show_stacktrace(s);  return; }

#define ENSURE_STACK_MIN(x) ENSURE_STACK_MIN_GEN(x, SP, "\nES<!")
#define ENSURE_STACK_MAX(x) ENSURE_STACK_MAX_GEN(x, SP, LIM, "\nES>!")

#define ENSURE_R_STACK_MIN(x) ENSURE_STACK_MIN_GEN(x, R_SP, "\nERS<!")
#define ENSURE_R_STACK_MAX(x) ENSURE_STACK_MAX_GEN(x, R_SP, R_LIM, "\nERS>!")
#else
#define ENSURE_STACK_MIN(x)
#define ENSURE_STACK_MAX(x)

#define ENSURE_R_STACK_MIN(x)
#define ENSURE_R_STACK_MAX(x)
#endif

#define POP() CELLS[SP]; SP -= 1
#define PUSH(x) SP += 1; CELLS[SP] = (x)

#define R_POP() R_CELLS[R_SP]; R_SP -= 1
#define R_PUSH(x) R_SP += 1; R_CELLS[R_SP] = (x)

#define POP1() ENSURE_STACK_MIN(1); cell p1=POP()
#define POP2() ENSURE_STACK_MIN(2); cell p1=CELLS[SP], p2=CELLS[SP-1]; SP-=2
#define POP3() ENSURE_STACK_MIN(3); cell p1=CELLS[SP], p2=CELLS[SP-1], p3=CELLS[SP-2]; SP-=3
#define POP4() ENSURE_STACK_MIN(4); cell p1=CELLS[SP], p2=CELLS[SP-1], p3=CELLS[SP-2], p4=CELLS[SP-3]; SP-=4

#define PUSH1(p1) ENSURE_STACK_MAX(1); PUSH(p1)
#define PUSH2(p1, p2) ENSURE_STACK_MAX(2); SP+=2; CELLS[SP-1]=p1;  CELLS[SP]=p2
#define PUSH3(p1, p2, p3) ENSURE_STACK_MAX(3); SP+=3; CELLS[SP-2]=p1; CELLS[SP-1]=p2; CELLS[SP]=p3

#define define_register_OP(N) OP_##N##_LOAD, OP_##N##_STORE, OP_##N##_ADD, \
    OP_##N##_INC, OP_##N##_DEC, OP_##N##_R_PUSH, OP_##N##_R_POP

#define define_register(N)                                                     \
  case OP_REG_##N##_STORE: { ENSURE_STACK_MIN(1);  N = POP(); break; }         \
  case OP_REG_##N##_LOAD: { ENSURE_STACK_MAX(1);   PUSH(N); break; }    \
  case OP_REG_##N##_ADD: { ENSURE_STACK_MIN(1);    N += POP(); break; } \
  case OP_REG_##N##_INC: { N += 1; break; }                             \
  case OP_REG_##N##_DEC: { N -= 1; break; }                             \
  case OP_REG_R_TO_##N##_ : { ENSURE_R_STACK_MIN(1); N = R_POP(); break; } \
  case OP_REG_##N##_TO_R: { ENSURE_R_STACK_MAX(1); R_PUSH(N); break; }

#define NON_NULL(x) ((x) ? (x) : "")

#ifdef __cplusplus
}
#endif

#endif /* __COLORFORTH_H */
