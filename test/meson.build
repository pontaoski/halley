# SPDX-FileCopyrightText: 2024 Janet Blackquill <uhhadd@gmail.com>
#
# SPDX-License-Identifier: MIT

test_sources = [
  'test.cpp',
]
if cc.get_id() == 'msvc'
  test_cpp_args = ['/std:c++14']
else
  test_cpp_args = ['-std=c++14']
endif
test_exe = executable(
  'test_exe',
  test_sources,
  dependencies: [gtest_dep],
  include_directories: [halley_private_include],
  cpp_args: test_cpp_args,
  link_with: halley.get_shared_lib()
)
test('halley test', test_exe)
