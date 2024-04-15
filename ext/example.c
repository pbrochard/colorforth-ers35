// The author disclaims copyright to this source code.

/**********************************************************************************
 *   PREFIX DEF
 *********************************************************************************/
#ifdef __SECTION_PREFIX_DEF

case '$': { ...; return; }

#endif /* __SECTION_PREFIX_DEF */


/**********************************************************************************
 *   HASH DEF
 *********************************************************************************/
#ifdef __SECTION_HASH_DEF

#define OP_...                     (opcode_t) 0x...                // ...

#endif /* __SECTION_HASH_DEF */


/**********************************************************************************
 *   WORD DEF
 *********************************************************************************/
#ifdef __SECTION_WORD_DEF

define_primitive(s, ENTRY_NAME("..."), OP_....);

#endif /* __SECTION_WORD_DEF */


/**********************************************************************************
 *   SWITCH DEF
 *********************************************************************************/
#ifdef __SECTION_SWITCH

case OP_...: {
  POP1();
  ...
  break;
}

#endif /* __SECTION_SWITCH */


/**********************************************************************************
 *   FUNCTION DEF
 *********************************************************************************/
#ifdef __SECTION_FUNCTION

void
....(struct state *s, ....)
{
  ....
}

#endif /* __SECTION_FUNCTION */
