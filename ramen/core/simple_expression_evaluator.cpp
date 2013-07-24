// 2011 Esteban Tovagliari.
// Mostly based on calc3 spirit example. Original copyright follows.

/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  A calculator example demonstrating the grammar and semantic actions
//  using phoenix to do the actual expression evaluation. The parser is
//  essentially an "interpreter" that evaluates expressions on the fly.
//
//  [ JDG June 29, 2002 ]   spirit1
//  [ JDG March 5, 2007 ]   spirit2
//
///////////////////////////////////////////////////////////////////////////////

#include<ramen/core/simple_expression_evaluator.hpp>

#include<boost/spirit/include/qi.hpp>
#include<boost/spirit/include/phoenix_operator.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

namespace ramen
{
namespace core
{

struct simple_expression_evaluator_t::impl
{
    template <typename Iterator>
    struct calc_grammar : qi::grammar<Iterator, double(), ascii::space_type>
    {
        calc_grammar() : calc_grammar::base_type( expression)
        {
            using qi::_val;
            using qi::_1;
            using qi::double_;

            expression =
                term                            [_val = _1]
                >> *(   ('+' >> term            [_val += _1])
                    |   ('-' >> term            [_val -= _1])
                    )
                ;

            term =
                factor                          [_val = _1]
                >> *(   ('*' >> factor          [_val *= _1])
                    |   ('/' >> factor          [_val /= _1])
                    )
                ;

            factor =
                double_                         [_val = _1]
                |   '(' >> expression           [_val = _1] >> ')'
                |   ('-' >> factor              [_val = -_1])
                |   ('+' >> factor              [_val = _1])
                ;
        }

        qi::rule<Iterator, double(), ascii::space_type> expression, term, factor;
    };

    boost::optional<double> parse_and_eval( const core::string8_t& s) const
    {
        using boost::spirit::ascii::space;

        core::string8_t::const_iterator iter = s.begin();
        core::string8_t::const_iterator end = s.end();
        double result;
        bool r = phrase_parse( iter, end, calc, space, result);

        if( r && iter == end)
            return result;
        else
            return boost::optional<double>();
    }

    calc_grammar<core::string8_t::const_iterator> calc;
};

simple_expression_evaluator_t::simple_expression_evaluator_t()	{ pimpl_ = new impl();}
simple_expression_evaluator_t::~simple_expression_evaluator_t()	{ delete pimpl_;}

boost::optional<double> simple_expression_evaluator_t::operator()( const core::string8_t& s) const
{
    return pimpl_->parse_and_eval( s);
}

} // core
} // ramen
