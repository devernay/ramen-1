// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMBO_GROUP_PARAM_HPP
#define	RAMEN_COMBO_GROUP_PARAM_HPP

#include<ramen/params/composite_param.hpp>

#include<QPointer>

class QComboBox;
class QStackedWidget;

namespace ramen
{

class RAMEN_API combo_group_param_t : public composite_param_t
{
    Q_OBJECT

public:

    combo_group_param_t();

    void set_default_value( int x);

    void set_value( int x, change_reason reason = user_edited);

protected:

    combo_group_param_t( const combo_group_param_t& other);
    void operator=( const combo_group_param_t& other);

private:

    virtual param_t *do_clone() const { return new combo_group_param_t( *this);}

    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    virtual core::auto_ptr_t<undo::command_t> do_create_command();

    //virtual void do_read( const serialization::yaml_node_t& node);
    //virtual void do_write( serialization::yaml_oarchive_t& out) const;

    virtual QWidget *do_create_widgets();

    QPointer<QComboBox> menu_;
    QPointer<QStackedWidget> stack_;

private Q_SLOTS:

    void item_picked( int index);
};

} // namespace

#endif
