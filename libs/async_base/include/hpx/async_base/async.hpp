//  Copyright (c) 2007-2017 Hartmut Kaiser
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_ASYNC_BASE_ASYNC_APR_16_20012_0225PM)
#define HPX_ASYNC_BASE_ASYNC_APR_16_20012_0225PM

#include <hpx/config.hpp>
#include <hpx/type_support/decay.hpp>

#include <utility>

namespace hpx { namespace detail {
    // dispatch point used for async implementations
    template <typename Func, typename Enable = void>
    struct async_dispatch;
}}    // namespace hpx::detail

namespace hpx {
    template <typename F, typename... Ts>
    HPX_FORCEINLINE auto async(F&& f, Ts&&... ts)
        -> decltype(detail::async_dispatch<typename util::decay<F>::type>::call(
            std::forward<F>(f), std::forward<Ts>(ts)...))
    {
        return detail::async_dispatch<typename util::decay<F>::type>::call(
            std::forward<F>(f), std::forward<Ts>(ts)...);
    }
}    // namespace hpx

#endif
