# SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
#
# SPDX-License-Identifier: MIT

halley_sources = [
  'system.c',
  'memory_management_unit.c',
]

halley_public_headers = [
  'inc/system.h',
  'inc/memory_allocation.h',
]

halley_include = include_directories('inc')
halley_private_include = include_directories('.')

if cc.get_id() == 'msvc'
  halley_c_args = ['/std:c11']
else
  halley_c_args = ['-std=c99']
endif

halley = both_libraries(
  'halley',
  halley_sources,
  include_directories: halley_include,
  install: true,
  c_args: halley_c_args,
  vs_module_defs: 'halley.def'
)

install_headers(halley_public_headers)
