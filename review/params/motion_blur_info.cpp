// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/motion_blur_info.hpp>

#include<limits>

#include<ramen/assert.hpp>

namespace ramen
{

namespace
{

template<class T>
inline T abs( T a)
{
    return ( a > T(0)) ? a : -a;
}

template <class T>
inline T lerpfactor( T m, T a, T b)
{
    //
    // Return how far m is between a and b, that is return t such that
    // if:
    //     t = lerpfactor(m, a, b);
    // then:
    //     m = lerp(a, b, t);
    //
    // If a==b, return 0.
    //

    T d = b - a;
    T n = m - a;

    if( abs( d) > T(1) || abs( n) < std::numeric_limits<T>::max() * abs( d))
        return n / d;

    return T(0);
}

} // unnamed

bool motion_blur_info_t::operator==( const motion_blur_info_t& other) const
{
    return samples == other.samples &&
            shutter == other.shutter &&
            shutter_offset == other.shutter_offset &&
            filter == other.filter;
}

bool motion_blur_info_t::operator!=( const motion_blur_info_t& other) const { return !( *this == other);}

motion_blur_info_t::loop_data_t::loop_data_t( float time, int samples, float shutter, float offset, motion_blur_info_t::filter_type f)
{
    if( shutter != 0)
        num_samples = 2 * samples + 1;
    else
        num_samples = 1;

    start_time = time - ( shutter / 2.0f) + ( offset * shutter / 2.0f);
    end_time = time + ( shutter / 2.0f) + ( offset * shutter / 2.0f);
    center_time = ( end_time + start_time) / 2.0f;
    time_step = ( end_time - start_time) / num_samples;
    filter = f;
}

float motion_blur_info_t::loop_data_t::weight_for_time( float t) const
{
    switch( filter)
    {
    case box_filter:
        return 1.0f;
    break;

    case triangle_filter:
    {
        float x = abs( t - center_time);
        float b = (end_time - center_time) + ( time_step / 2.0);
        return lerpfactor( x, b, 0.0f);
    }
    break;

    case cubic_filter:
    {
        float x = abs( t - center_time);
        float b = (end_time - center_time) + ( time_step / 2.0);
        float t = lerpfactor( x, 0.0f, b);
        return ( 2.0f * t * t * t) - ( 3.0f * t * t) + 1.0f;
    }
    break;

    default:
        RAMEN_ASSERT( 0 && "Motion blur, unknown filter type");
        return 1.0f; // to make MSCV happy
    }
}

core::string8_t motion_blur_info_t::filter_to_string( motion_blur_info_t::filter_type f) const
{
    switch( f)
    {
        case box_filter:
            return "box_filter";

        case triangle_filter:
            return "triangle_filter";

        case cubic_filter:
            return "cubic_filter";
    }
}

motion_blur_info_t::filter_type motion_blur_info_t::string_to_filter( const core::string8_t& s) const
{
    if( s == "box_filter")
        return box_filter;

    if( s == "triangle_filter")
        return triangle_filter;

    if( s == "cubic_filter")
        return cubic_filter;

    RAMEN_ASSERT( 0 && "Unknown filter in motion_blur_info_t");
}

/*
void operator>>( const YAML::Node& in, motion_blur_info_t& mb)
{
    int version;
    in[0] >> version;
    in[1] >> mb.samples;
    in[2] >> mb.shutter;
    in[3] >> mb.shutter_offset;

    std::string s;
    in[4] >> s;
    mb.filter = mb.string_to_filter( s);
}

YAML::Emitter& operator<<( YAML::Emitter& out, const motion_blur_info_t& mb)
{
    out << YAML::Flow << YAML::BeginSeq
        << 1 // version
        << mb.samples
        << mb.shutter
        << mb.shutter_offset
        << mb.filter_to_string( mb.filter);
    out << YAML::EndSeq;
    return out;
}
*/

} // ramen