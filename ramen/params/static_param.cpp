// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/params/static_param.hpp>

#include<ramen/params/static_param_command.hpp>

namespace ramen
{

static_param_t::static_param_t() : param_t() { set_static( true);}

static_param_t::static_param_t( const static_param_t& other) : param_t( other) {}

core::auto_ptr_t<undo::command_t> static_param_t::do_create_command()
{
    return core::auto_ptr_t<undo::command_t>( new static_param_command_t( *param_set(), id()));
}

} // namespace
