//
// Copyright (c) 2012 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#if !BOOST_PP_IS_ITERATING

    #ifndef QDEPS_BACK_DETAIL_CREATOR_HPP
    #define QDEPS_BACK_DETAIL_CREATOR_HPP

    #include <boost/preprocessor/iteration/iterate.hpp>
    #include <boost/preprocessor/repetition/repeat.hpp>
    #include <boost/preprocessor/punctuation/comma_if.hpp>
    #include <boost/type_traits/is_base_of.hpp>
    #include <boost/utility/enable_if.hpp>
    #include <boost/mpl/size.hpp>
    #include <boost/mpl/at.hpp>
    #include <boost/mpl/push_back.hpp>
    #include "di/Back/Aux/Utility.hpp"
    #include "di/Back/Aux/Dependency.hpp"
    #include "di/Back/Detail/Binder.hpp"
    #include "di/Back/Detail/Converter.hpp"
    #include "di/Back/Scopes/PerRequest.hpp"
    #include "di/Config.hpp"

    #define BOOST_PP_ITERATION_PARAMS_1 (3, (0, QDEPS_FUNCTION_ARITY_LIMIT_SIZE, "di/Back/Detail/Creator.hpp"))

    namespace di
    {
    namespace Back
    {
    namespace Detail
    {

    template
    <
        typename TDeps,
        typename TPool,
        template<typename, typename, typename = TDeps, typename = Aux::Dependency<Scopes::PerRequest, boost::mpl::_1, boost::mpl::_2> > class TBinder = Binder,
        template<typename, typename> class TConverter = Converter
    >
    class CreatorImpl
    {
    public:
        template<typename T, typename TCallStack, typename TEntries>
        static typename TBinder<T, TCallStack>::type::template ResultType<TPool>::type execute(TEntries& p_entries, const TPool& p_pool)
        {
            typedef typename TBinder<T, TCallStack>::type ToBeCreated;
            typedef typename Aux::UpdateCallStack<TCallStack, ToBeCreated>::type CallStack;
            return executeImpl<ToBeCreated, CallStack, TEntries>(p_entries, p_pool);
        }

    private:
        #include BOOST_PP_ITERATE()

        template<typename TDependency, typename TEntries>
        static typename boost::enable_if<boost::is_base_of<TDependency, TEntries>, TDependency&>::type acquire(TEntries& p_entries)
        {
            return static_cast<TDependency&>(p_entries);
        }

        template<typename TDependency, typename TEntries>
        static typename boost::disable_if<boost::is_base_of<TDependency, TEntries>, TDependency>::type acquire(TEntries&)
        {
            return TDependency();
        }
    };

    template<typename TDeps, typename TPool> struct Creator : CreatorImpl<TDeps, TPool> { };

    } // namespace Detail
    } // namespace Back
    } // namespace di

    #endif

#else

    template<typename TDependency, typename TCallStack, typename TEntries>
    static typename Aux::EnableIfCtorSize<TDependency, BOOST_PP_ITERATION(), typename TDependency::template ResultType<TPool>::type>::type
    executeImpl(TEntries& p_entries, const TPool& p_pool)
    {
        #define QDEPS_CREATOR_EXECUTE(z, n, _) BOOST_PP_COMMA_IF(n)                                         \
             TConverter<typename TDependency::Scope, typename Aux::AtCtor<TDependency, n>::type>::execute(  \
                execute<typename Aux::AtCtor<TDependency, n>::type, TCallStack>(p_entries, p_pool))

        return acquire<TDependency>(p_entries).create(
            p_pool BOOST_PP_COMMA_IF(BOOST_PP_ITERATION()) BOOST_PP_REPEAT(BOOST_PP_ITERATION(), QDEPS_CREATOR_EXECUTE, ~));

        #undef QDEPS_CREATOR_EXECUTE
    }

#endif

