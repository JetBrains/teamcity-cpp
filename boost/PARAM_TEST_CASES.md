If you have tests with test parameters,
look at http://jetbrains.net/tracker/issue/TW-7043 for quick solution.

It's proposed to replace standard parameterized_test.hpp with modified one - it
creates a unique name for each test from the range if you use
BOOST_NAMED_PARAM_TEST_CASE macro.

By default a per-param type sequence is used to create name,
e.g. test(#1), test(#2), ... but you can alter name generation by
providing your our compose_test_case_name() function, or by providing
to_string overloads. Note that any of your custom overloads should reside
in the boost::unit_test namespace.

Thanks Kirill Kovalenko for the workaround.

parameterized_test.hpp
----------------------

#pragma once

#include <sstream>
#include <boost/test/parameterized_test.hpp>

#define BOOST_NAMED_PARAM_TEST_CASE( function, begin, end )     \
    boost::unit_test::make_named_test_case( function,           \
    BOOST_TEST_STRINGIZE( function ),                           \
    (begin), (end) )                                            \

namespace boost {

namespace unit_test {

template<typename ParamType>
inline std::string to_string(const ParamType& t)
{
    static unsigned int counter = 0;
    std::stringstream ss;
    ss << "#" << ++counter;
    return ss.str();
}

template<typename ParamType>
inline std::string compose_test_case_name(boost::unit_test::const_string name, const ParamType& t)
{
    std::stringstream ss;
    ss << name << "(\"" << to_string(t) << "\")";
    return ss.str();
}

namespace ut_detail {

template<typename ParamType, typename ParamIter>
class named_param_test_case_generator : public test_unit_generator {
public:
    named_param_test_case_generator(
        callback1<ParamType> const&  test_func,
        const_string                 tc_name, 
        ParamIter                    par_begin,
        ParamIter                    par_end )
        : m_test_func( test_func )
        , m_tc_name( ut_detail::normalize_test_case_name( tc_name ) )
        , m_par_begin( par_begin )
        , m_par_end( par_end )
    {}

    boost::unit_test::test_unit* next() const
    {
        if( m_par_begin == m_par_end )
            return (test_unit*)0;

        boost::unit_test::ut_detail::test_func_with_bound_param<ParamType> bound_test_func( m_test_func, *m_par_begin );
        test_unit* res = new test_case( compose_test_case_name(m_tc_name, *m_par_begin), bound_test_func );

        ++m_par_begin;

        return res;
    }

private:
    // Data members
    callback1<ParamType>    m_test_func;
    std::string             m_tc_name;
    mutable ParamIter       m_par_begin;
    ParamIter               m_par_end;
};

} // ut_detail

template<typename ParamType, typename ParamIter>
inline ut_detail::named_param_test_case_generator<ParamType,ParamIter>
make_named_test_case( callback1<ParamType> const& test_func,
                const_string   tc_name, 
                ParamIter      par_begin,
                ParamIter      par_end )
{
    return ut_detail::named_param_test_case_generator<ParamType,ParamIter>( test_func, tc_name, par_begin, par_end );
}

//____________________________________________________________________________//

template<typename ParamType, typename ParamIter>
inline ut_detail::named_param_test_case_generator<
    BOOST_DEDUCED_TYPENAME remove_const<BOOST_DEDUCED_TYPENAME remove_reference<ParamType>::type>::type,ParamIter>
make_named_test_case( void (*test_func)( ParamType ),
                const_string  tc_name, 
                ParamIter     par_begin,
                ParamIter     par_end )
{
    typedef BOOST_DEDUCED_TYPENAME remove_const<BOOST_DEDUCED_TYPENAME remove_reference<ParamType>::type>::type param_value_type;
    return ut_detail::named_param_test_case_generator<param_value_type,ParamIter>( test_func, tc_name, par_begin, par_end );
}

//____________________________________________________________________________//

template<typename UserTestCase,typename ParamType, typename ParamIter>
inline ut_detail::named_param_test_case_generator<
    BOOST_DEDUCED_TYPENAME remove_const<BOOST_DEDUCED_TYPENAME remove_reference<ParamType>::type>::type,ParamIter>
make_named_test_case( void (UserTestCase::*test_method )( ParamType ),
                const_string                           tc_name,
                boost::shared_ptr<UserTestCase> const& user_test_case,
                ParamIter                              par_begin,
                ParamIter                              par_end )
{
    typedef BOOST_DEDUCED_TYPENAME remove_const<BOOST_DEDUCED_TYPENAME remove_reference<ParamType>::type>::type param_value_type;
    return ut_detail::named_param_test_case_generator<param_value_type,ParamIter>( 
               ut_detail::user_param_tc_method_invoker<UserTestCase,ParamType>( user_test_case, test_method ), 
               tc_name,
               par_begin,
               par_end );
}

} // namespace unit_test

} // boost
