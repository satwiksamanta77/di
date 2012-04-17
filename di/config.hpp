//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef QDEPS_CONFIG_HPP
#define QDEPS_CONFIG_HPP

#include <boost/mpl/assert.hpp>
#include <boost/config.hpp>

#if !defined(QDEPS_CTOR_CFG_VA_ARGS) && !defined(QDEPS_CTOR_CFG_BRACKET)
# define QDEPS_CTOR_CFG_VA_ARGS
#endif

#if !defined(QDEPS_FUNCTION_ARITY_LIMIT_SIZE)
# define QDEPS_FUNCTION_ARITY_LIMIT_SIZE 10
#endif

#if !defined(QDEPS_CTOR_UNIQUE_NAME)
# define QDEPS_CTOR_UNIQUE_NAME ctor__
#endif

#ifndef QDEPS_STATIC_ASSERT
# define QDEPS_STATIC_ASSERT(cond, expr, types) BOOST_MPL_ASSERT_MSG(cond, expr, types)
#endif

#if 0
#if defined(BOOST_HAS_RVALUE_REFS) && defined(BOOST_HAS_VARIADIC_TMPL)
#endif
#endif

namespace di
{

/**
 * Example of changing default behavior using specialized class
 *
 * @code
 * #include <QDesp/Config.hpp>
 *
 * namespace di
 * {
 * template<>
 * struct Defaults<Back::Detail::Policy, di::Specialized>
 * {
 *     typedef Back::Policy<...> type;
 * };
 * } // namespace di
 * @endcode
 */
class Specialized { };
template<typename, typename = Specialized> struct Defaults;

} // namespace di

#endif

