# Copyright (c) 2007-2012 Hartmut Kaiser
#
# SPDX-License-Identifier: BSL-1.0
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# Add component for this example
add_subdirectory(cancelable_action)

# Add example itself
set(sources cancelable_action_client.cpp)

source_group("Source Files" FILES ${sources})

# add example executable
add_hpx_executable(cancelable_action_client
    INTERNAL_FLAGS
    SOURCES ${sources}
    DEPENDENCIES cancelable_action_component
    FOLDER "Examples/CancelableAction")

add_hpx_example_target_dependencies("cancelable_action" cancelable_action_client)

if(HPX_WITH_TESTS AND HPX_WITH_TESTS_EXAMPLES)
  add_hpx_example_test("cancelable_action" cancelable_action_client THREADS_PER_LOCALITY 4)
endif()
