/**
 * likely.hpp
 *
 * implementation of macros that help compiler to optimize code
 * for usual cases (i.e. when we know condition will pass/fail most
 * of the times).
 *
 * for example following code tells compiler that it is most probable that
 * this condition will fail (since it is just an error handling).
 *
 * <code>
 * if( UNLIKELY(paramPtr==NULL) )
 *   return -1;
 * </code>
 *
 * in case of no support for this is present, fallback to regular expression
 * is provided.
 *
 * trick found on interesting publication regarding memory and caching:
 * http://people.redhat.com/drepper/cpumemory.pdf
 *
 */
#ifndef INCLUDE_SYSTEM_LIKELY_HPP_FILE
#define INCLUDE_SYSTEM_LIKELY_HPP_FILE

/* public header */

#if __GNUC__ >= 3

// optimization can be done
#define UNLIKELY(expr) __builtin_expect(!!(expr), false)
#define LIKELY(expr)   __builtin_expect(!!(expr), true )

#else

#warning "__builtin_expect() call is probably not present - falling back to pure expression"
// in case of no heuristic can be made, just use expression itself
#define UNLIKELY(expr) (!!(expr))
#define LIKELY(expr)   (!!(expr))

#endif // gcc>=3.0

#endif // header guard
