/* 
 * File:   main.cpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 15:57
 */

#include "operator_proxy.hpp"
#include "operand.hpp"
#include "operators.hpp"

#include <iostream>
#include <cmath>




struct floating_point_context_tag{};

namespace pop
{
    auto operator==( void(float,float) , pop::default_operator_tag )
    {
        return []( float x , float y )
        {
            std::cout << "Using default\n";
            
            return x == y;
        };
    }
}

auto operator==( void(float,float) , floating_point_context_tag )
{
    return []( float x , float y )
    {
        std::cout << "Using floating-point\n";

        return std::abs( x - y ) < 0.00001f;
    };
}


int main()
{
    using pop::trigger::_;
    
    auto op = _( 1.0f ) == 1.0f;
    
    std::cout << std::boolalpha << op() << std::endl;
    std::cout << std::boolalpha << op( floating_point_context_tag{} ) << std::endl;
    std::cout << std::boolalpha << (_(1.0f) == 2.0f)( floating_point_context_tag{} ) << std::endl;
}

