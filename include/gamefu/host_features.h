/* -----------------------------------------------------------------------------
Part of the GameFU Station Project under the GNU General Public License v2.0.
See the LICENSE file and LICENSES directory for more information.
SPDX-License-Identifier: GPL-2.0-only
----------------------------------------------------------------------------- */


#ifndef __GAMEFU__HOST_FEATURES_H__
#define __GAMEFU__HOST_FEATURES_H__


#if defined(_WIN32)
#  if !defined(_CRT_SECURE_NO_WARNINGS)
#    define _CRT_SECURE_NO_WARNINGS
#  endif /* _CRT_SECURE_NO_WARNINGS */

#  if !defined(_CRT_NONSTDC_NO_DEPRECATE)
#    define _CRT_NONSTDC_NO_DEPRECATE  1
#  endif /* _CRT_NONSTDC_NO_DEPRECATE */

#  if !define(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif /* WIN32_LEAN_AND_MEAN */
#endif


#if defined(__linux__)
#  if !defined(_BSD_SOURCE)
#    define _BSD_SOURCE
#  endif /* _BSD_SOURCE */

#  if !defined(_SVID_SOURCE)
#    define _SVID_SOURCE
#  endif /* _SVID_SOURCE */

#  if !defined(_DEFAULT_SOURCE)
#    define _DEFAULT_SOURCE
#  endif /* _DEFAULT_SOURCE */

#  if !defined(_GNU_SOURCE)
#    define _GNU_SOURCE
#  endif /* _GNU_SOURCE */

#  if defined(_POSIX_C_SOURCE) && (_POSIX_C_SOURCE - 0 < 200809L)
#    undef _POSIX_C_SOURCE
#  endif /* _POSIX_C_SOURCE */
#  if !defined(_POSIX_C_SOURCE)
#    define _POSIX_C_SOURCE  200809L
#  endif /* _POSIX_C_SOURCE */
#endif


#endif /* __GAMEFU__HOST_FEATURES_H__ */
