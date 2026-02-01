/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------

This file makes use of a C preprocessor technique called "X-macros".
There are typically two ways to create these:

    1. By defining a macro which sequentially invokes its argument, typically
       named `X`, to be instantiated at a later time when its `X` is defined.

       #define MY_ENUM_VALUES(X) X(FOO) X(BAR) X(BAZ)
       // ...
       enum {
       #define X(Name)  MY_ENUM_##Name,
       MY_ENUM_VALUES
       #undef X
       };

    2. By creating a header file which sequentially invokes a macro similar
       to the previous example.

       (See this file for an example, I don't need to inline it.)

The second option of using a header file comes with a few benefits over
defining a macro and instantiating it:

    -  The header file can itself contain preprocessor statements. This allows
       you to define default implementations of the "X" macro if it was not
       provided at time of inclusion. It also allows undefining the "X" macro
       at the end of the source file to avoid poluting names or triggering
       macro redefinition warnings.

    -  The file can be kept out-of-band with other source text and still
       included when necessary. For single header libraries this is a con
       rather than a pro, but as soon as you have a second source file that
       is no longer a problem to worry about.

    -  You can more easily manage multiple "X" macros within the header, and
       even have them depend on each other by default.

       #if !defined(MY_ENUM_BASE)
       #  define MY_ENUM_BASE(Name)
       #endif
       // the user can define `MY_ENUM_BASE` and not have to define
       // `MY_ENUM_SPECIAL` unless they specifically want the extra data.
       #if !defined(MY_ENUM_SPECIAL)
       #  define MY_ENUM_SPECIAL(Name, ExtraData)  MY_ENUM_BASE(Name)
       #endif

       Doing this with a `#define`d "X" macro is more difficult and less
       ergonomic at the instantiation site.

----------------------------------------------------------------------------- */

#ifndef TK
#  define TK(Id)
#endif


TK(ENDL)
TK(DIRECTIVE)
TK(LABEL_GLOBAL)
TK(LABEL_LOCAL)
TK(MNEMONIC)
TK(GPR)
TK(C0R)
TK(IMMEDIATE)
TK(BYTE_STRING)


#undef TK
