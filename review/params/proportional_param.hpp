// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PROPORTIONAL_PARAM_HPP
#define	RAMEN_PROPORTIONAL_PARAM_HPP

#include<ramen/params/numeric_param.hpp>

#include<ramen/math/vector3.hpp>

#include<QPointer>
#include<QToolButton>

namespace ramen
{

class RAMEN_API proportional_param_t : public numeric_param_t
{
    Q_OBJECT

public:

    proportional_param_t();

    bool proportional() const	    { return flags() & proportional_bit;}
    void set_proportional( bool p);

    bool proportional_checked() const;

private Q_SLOTS:

    void proportional_toggle( bool state);

protected:

    proportional_param_t( const proportional_param_t& other);
    void operator=( const proportional_param_t& other);

    void create_proportional_button( QWidget *parent, int height);

    QPointer<QToolButton> prop_button_;

    static math::vector3f_t proportional_factor;
};

} // namespace

#endif