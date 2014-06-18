#ifndef _LWQQ_CONFIG_H__
#define _LWQQ_CONFIG_H__

#define WITH_LIBEV
/* #undef WITH_LIBUV */
#define WITH_SQLITE
/* #undef WITHOUT_ASYNC */
#define WITH_MOZJS
#define MOZJS_185
/* #undef MOZJS_17 */

#define LWQQ_VERSION ""

//use a single thread to poll message
#define USE_MSG_THREAD 1

//detach thread without join it
#define DETACH_THREAD 

//enable additional debug function
#define USE_DEBUG 0

//write discu into database
#define DISCU_READ_DB 1

// when continuly poll_msg_lost happend {?} times, send relink
// default : 5
#define RETRY_BEFORE_RELINK 3

// set poll_msg timeout {?} seconds
// default : 90
#define POLL_MSG_TIMEOUT 75

#endif /* __CONFIG_H__ */

