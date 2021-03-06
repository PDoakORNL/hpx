//  Copyright (c) 2007-2016 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  SPDX-License-Identifier: BSL-1.0
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/config.hpp>
#include <hpx/async/apply.hpp>
#include <hpx/async.hpp>
#include <hpx/runtime/actions/manage_object_action.hpp>
#include <hpx/runtime/agas/interface.hpp>
#include <hpx/async/applier/detail/apply_colocated.hpp>
#include <hpx/runtime/components/stubs/runtime_support.hpp>
#include <hpx/runtime/get_colocation_id.hpp>
#include <hpx/runtime/launch_policy.hpp>
#include <hpx/runtime/naming/name.hpp>
#include <hpx/performance_counters/counters.hpp>
#include <hpx/runtime_configuration/ini.hpp>
#include <hpx/runtime.hpp>
#include <hpx/traits/component_supports_migration.hpp>
#include <hpx/traits/action_was_object_migrated.hpp>

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

namespace hpx { namespace components { namespace stubs
{
    lcos::future<int>
    runtime_support::load_components_async(naming::id_type const& gid)
    {
        typedef server::runtime_support::load_components_action action_type;
        return hpx::async<action_type>(gid);
    }

    int runtime_support::load_components(naming::id_type const& gid)
    {
        return load_components_async(gid).get();
    }

    lcos::future<void>
    runtime_support::call_startup_functions_async(naming::id_type const& gid,
        bool pre_startup)
    {
        typedef server::runtime_support::call_startup_functions_action action_type;
        return hpx::async<action_type>(gid, pre_startup);
    }

    void runtime_support::call_startup_functions(naming::id_type const& gid,
        bool pre_startup)
    {
        call_startup_functions_async(gid, pre_startup).get();
    }

    /// \brief Shutdown the given runtime system
    lcos::future<void>
    runtime_support::shutdown_async(naming::id_type const& targetgid,
        double timeout)
    {
        // Create a promise directly and execute the required action.
        // This action has implemented special response handling as the
        // back-parcel is sent explicitly (and synchronously).
        typedef server::runtime_support::shutdown_action action_type;

        lcos::promise<void> value;
        auto f = value.get_future();

        // We need to make it unmanaged to avoid late refcnt requests
        id_type gid(value.get_id().get_gid(), id_type::unmanaged);
        hpx::apply<action_type>(targetgid, timeout, gid);

        return f;
    }

    void runtime_support::shutdown(naming::id_type const& targetgid,
        double timeout)
    {
        // The following get yields control while the action above
        // is executed and the result is returned to the future
        shutdown_async(targetgid, timeout).get();
    }

    /// \brief Shutdown the runtime systems of all localities
    void runtime_support::shutdown_all(naming::id_type const& targetgid,
        double timeout)
    {
        hpx::apply<server::runtime_support::shutdown_all_action>(
            targetgid, timeout);
    }

    void runtime_support::shutdown_all(double timeout)
    {
        hpx::apply<server::runtime_support::shutdown_all_action>(
            hpx::naming::id_type(
                hpx::applier::get_applier().get_runtime_support_raw_gid(),
                hpx::naming::id_type::unmanaged), timeout);
    }

    ///////////////////////////////////////////////////////////////////////
    /// \brief Retrieve configuration information
    /// \brief Terminate the given runtime system
    lcos::future<void>
    runtime_support::terminate_async(naming::id_type const& targetgid)
    {
        // Create a future directly and execute the required action.
        // This action has implemented special response handling as the
        // back-parcel is sent explicitly (and synchronously).
        typedef server::runtime_support::terminate_action action_type;

        lcos::promise<void> value;
        auto f = value.get_future();

        hpx::apply<action_type>(targetgid, value.get_id());
        return f;
    }

    void runtime_support::terminate(naming::id_type const& targetgid)
    {
        // The following get yields control while the action above
        // is executed and the result is returned to the future
        terminate_async(targetgid).get();
    }

    /// \brief Terminate the runtime systems of all localities
    void runtime_support::terminate_all(naming::id_type const& targetgid)
    {
        hpx::apply<server::runtime_support::terminate_all_action>(
            targetgid);
    }

    void runtime_support::terminate_all()
    {
        hpx::apply<server::runtime_support::terminate_all_action>(
            hpx::naming::id_type(
                hpx::applier::get_applier().get_runtime_support_raw_gid(),
                hpx::naming::id_type::unmanaged));
    }

    ///////////////////////////////////////////////////////////////////////
    void runtime_support::garbage_collect_non_blocking(
        naming::id_type const& targetgid)
    {
        typedef server::runtime_support::garbage_collect_action
            action_type;
        hpx::apply<action_type>(targetgid);
    }

    lcos::future<void> runtime_support::garbage_collect_async(
        naming::id_type const& targetgid)
    {
        typedef server::runtime_support::garbage_collect_action
            action_type;
        return hpx::async<action_type>(targetgid);
    }

    void runtime_support::garbage_collect(naming::id_type const& targetgid)
    {
        typedef server::runtime_support::garbage_collect_action
            action_type;
        hpx::async<action_type>(targetgid).get();
    }

    ///////////////////////////////////////////////////////////////////////
    lcos::future<naming::id_type>
    runtime_support::create_performance_counter_async(naming::id_type targetgid,
        performance_counters::counter_info const& info)
    {
        if (!naming::is_locality(targetgid))
        {
            HPX_THROW_EXCEPTION(bad_parameter,
                "stubs::runtime_support::create_performance_counter_async",
                "The id passed as the first argument is not representing"
                    " a locality");
            return make_ready_future(naming::invalid_id);
        }

        typedef server::runtime_support::create_performance_counter_action
            action_type;
        return hpx::async<action_type>(targetgid, info);
    }

    naming::id_type
    runtime_support::create_performance_counter(naming::id_type targetgid,
        performance_counters::counter_info const& info,
        error_code& ec)
    {
        return create_performance_counter_async(targetgid, info).get(ec);
    }

    ///////////////////////////////////////////////////////////////////////
    /// \brief Retrieve configuration information
    lcos::future<util::section> runtime_support::get_config_async(
        naming::id_type const& targetgid)
    {
        // Create a future, execute the required action,
        // we simply return the initialized future, the caller needs
        // to call get() on the return value to obtain the result
        typedef server::runtime_support::get_config_action action_type;
        return hpx::async<action_type>(targetgid);
    }

    void runtime_support::get_config(naming::id_type const& targetgid,
        util::section& ini)
    {
        // The following get yields control while the action above
        // is executed and the result is returned to the future
        ini = get_config_async(targetgid).get();
    }

    ///////////////////////////////////////////////////////////////////////
    void runtime_support::remove_from_connection_cache_async(
        naming::id_type const& target, naming::gid_type const& gid,
        parcelset::endpoints_type const& endpoints)
    {
        typedef server::runtime_support::remove_from_connection_cache_action
            action_type;
        hpx::apply<action_type>(target, gid, endpoints);
    }
}}}

