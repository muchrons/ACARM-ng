/*
 * CallName.h
 *
 */
#ifndef INCLUDE_COMMONS_CALLNAME_H_FILE
#define INCLUDE_COMMONS_CALLNAME_H_FILE

/* public header */

// for gcc use pretty function, otherwise just standard __func__
#ifdef __GNUC__
#  define CALLNAME __PRETTY_FUNCTION__
#else
#  define CALLNAME __func__
#endif

#endif
