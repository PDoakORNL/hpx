//  Copyright (c) 2016-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_THREADS_RUN_AS_OS_THREAD_MAR_12_2016_0220PM)
#define HPX_THREADS_RUN_AS_OS_THREAD_MAR_12_2016_0220PM

#include <hpx/config.hpp>
#include <hpx/assertion.hpp>
#include <hpx/execution/executors/execution.hpp>
#include <hpx/execution/executors/service_executors.hpp>
#include <hpx/runtime/threads_fwd.hpp>
#include <hpx/functional/deferred_call.hpp>
#include <hpx/functional/result_of.hpp>

#include <type_traits>
#include <utility>

namespace hpx { namespace threads
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename F, typename ... Ts>
    hpx::future<typename util::invoke_result<F, Ts...>::type>
    run_as_os_thread(F && f, Ts &&... vs)
    {
        HPX_ASSERT(get_self_ptr() != nullptr);

        parallel::execution::io_pool_executor executor;
        auto result = parallel::execution::async_execute(executor,
            std::forward<F>(f), std::forward<Ts>(vs)...);
        return result;
    }
}}

#endif
