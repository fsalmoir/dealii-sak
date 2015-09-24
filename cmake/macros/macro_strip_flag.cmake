## ---------------------------------------------------------------------
##
## Copyright (C) 2014 - 2014 by the deal2lkit authors
##
## This file is part of the deal2lkit library.
##
## The deal2lkit library is free software; you can use it, redistribute
## it, and/or modify it under the terms of the GNU Lesser General
## Public License as published by the Free Software Foundation; either
## version 2.1 of the License, or (at your option) any later version.
## The full text of the license can be found in the file LICENSE at
## the top level of the deal2lkit distribution.
##
## ---------------------------------------------------------------------

#
# Remove all occurences of "${flag}" in the string variable.
#
# Usage:
#     STRIP_FLAG(variable flag)
#

MACRO(STRIP_FLAG _variable _flag)
  STRING(REPLACE " " "  " ${_variable} "${${_variable}}")
  SET(${_variable} " ${${_variable}} ")
  STRING(REPLACE " " "  " _flag2 "${_flag}")
  STRING(REPLACE " ${_flag2} " " " ${_variable} "${${_variable}}")
  STRING(REPLACE "  " " " ${_variable} "${${_variable}}")
  STRING(STRIP "${${_variable}}" ${_variable})
ENDMACRO()
