// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/render/context_guard.hpp>

#include<boost/bind.hpp>

#include<ramen/nodes/composition_node.hpp>
#include<ramen/nodes/graph_algorithm.hpp>

namespace ramen
{

namespace render
{

context_guard_t::context_guard_t( const context_t& context, node_t *n)
{
    RAMEN_ASSERT( context.composition_node);

    context_ = context;
    n_ = n;

    if( n_)
        save( n_);
    else
    {
        for( composition_node_t::const_node_iterator it( context_.composition_node->nodes().begin());
             it != context_.composition_node->nodes().end(); ++it)
        {
            const node_t *n = &(*it);

            if( n->num_outputs() == 0)
                save( n);
        }
    }
}

context_guard_t::~context_guard_t()
{
    context_.composition_node->set_frame( context_.frame);

    if( n_)
        restore( n_);
    else
    {
        for( composition_node_t::node_iterator it( context_.composition_node->nodes().begin());
             it != context_.composition_node->nodes().end(); ++it)
        {
            node_t *n = &(*it);

            if( n->num_outputs() == 0)
                restore( n);
        }
    }
}

void context_guard_t::save( const node_t *n)
{
    saved_info_t s;
    s.roi = n->interest();
    saved_[n] = s;
}

void context_guard_t::restore( node_t *n)
{
    depth_first_inputs_search( *n, boost::bind( &node_t::calc_format_fun, _1, context_));
    depth_first_inputs_search( *n, boost::bind( &node_t::calc_bounds_fun, _1, context_));
    depth_first_inputs_search( *n, boost::bind( &node_t::clear_interest_fun, _1));

    n->set_interest( saved_[n].roi);

    breadth_first_inputs_apply( *n, boost::bind( &node_t::calc_inputs_interest_fun, _1, context_));
    depth_first_inputs_search(  *n, boost::bind( &node_t::calc_defined_fun, _1, context_));
    depth_first_inputs_search(  *n, boost::bind( &node_t::subsample_areas_fun, _1, context_));
    depth_first_inputs_search( *n, boost::bind( &node_t::clear_hash, _1));
    depth_first_inputs_search( *n, boost::bind( &node_t::calc_hash_str, _1, context_));
}

} // render
} // ramen