#ifndef __BITSUTIL_H__
#define __BITSUTIL_H__

#ifndef BITHANDLING_MACRO
#ifndef BITHANDLING_INLINE
#ifndef BITHANDLING_INLINE_MACRO
#ifndef BITHANDLING_CPP
#error Must specify a bit handling API: BITHANDLING_MACRO, BITHANDLING_INLINE, BITHANDLING_INLINE_MACRO or BITHANDLING_CPP
#endif
#endif
#endif
#endif

#ifdef BITHANDLING_MACRO
#include "bitsMacro.h"
#elif defined BITHANDLING_INLINE
#include "bitsInline.h"
#elif defined BITHANDLING_INLINE_MACRO
#include "bitsInlineMacro.h"
#elif defined BITHANDLING_CPP
#include "bitsInlineCpp.h"
#endif

#endif
