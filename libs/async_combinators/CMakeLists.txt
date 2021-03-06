# Copyright (c) 2019 The STE||AR-Group
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(async_combinators_headers
  hpx/async_combinators/split_future.hpp
  hpx/async_combinators/wait_all.hpp
  hpx/async_combinators/wait_any.hpp
  hpx/async_combinators/wait_each.hpp
  hpx/async_combinators/wait_some.hpp
  hpx/async_combinators/when_all.hpp
  hpx/async_combinators/when_any.hpp
  hpx/async_combinators/when_each.hpp
  hpx/async_combinators/when_some.hpp)

set(async_combinators_compat_headers
  hpx/lcos/split_future.hpp
  hpx/lcos/wait_all.hpp
  hpx/lcos/wait_any.hpp
  hpx/lcos/wait_each.hpp
  hpx/lcos/wait_some.hpp
  hpx/lcos/when_all.hpp
  hpx/lcos/when_any.hpp
  hpx/lcos/when_each.hpp
  hpx/lcos/when_some.hpp)

include(HPX_AddModule)
add_hpx_module(async_combinators
  COMPATIBILITY_HEADERS ON
  DEPRECATION_WARNINGS
  FORCE_LINKING_GEN
  GLOBAL_HEADER_GEN ON
  HEADERS ${async_combinators_headers}
  COMPAT_HEADERS ${async_combinators_compat_headers}
  DEPENDENCIES
    hpx_allocator_support
    hpx_assertion
    hpx_basic_execution
    hpx_config
    hpx_datastructures
    hpx_errors
    hpx_functional
    hpx_iterator_support
    hpx_memory
    hpx_preprocessor
    hpx_threading
    hpx_type_support
    hpx_util
  CMAKE_SUBDIRS examples tests
)
