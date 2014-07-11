/* 
 * File:   operator_proxy.hpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 16:53
 */

#ifndef OPERATOR_PROXY_HPP
#define	OPERATOR_PROXY_HPP

#include "operand.hpp"

#include <functional>
#include <iostream>

namespace pop
{
    struct default_operator_tag{};
    extern const default_operator_tag default_operator;
    
    template<typename OP>
    struct operator_proxy
    {
        template<typename... ARGS>
        operator_proxy( ARGS&&... args ) :
            _operator{ std::forward<ARGS>( args )... }
        {}
            
        template<typename TAG>
        typename std::result_of<OP()>::type operator()( TAG tag )
        {
            std::cout << "Calling operator...\n";
               
            return _operator();
        }
        
        typename std::result_of<OP()>::type operator()( pop::default_operator_tag = pop::default_operator )
        {
            std::cout << "Calling operator...\n";
               
            return _operator();
        }
        
        operator typename std::result_of<OP(pop::default_operator_tag)>::type()
        {
            return (*this)();
        }
    private:
        OP _operator;
    };
    
    template<typename OP>
    pop::operator_proxy<typename std::decay<OP>::type> make_proxy( OP&& operator_ )
    {
        return { operator_ };
    } 
}


#endif	/* OPERATOR_PROXY_HPP */

