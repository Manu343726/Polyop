/* 
 * File:   operators.hpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 17:49
 */

#ifndef OPERATORS_HPP
#define	OPERATORS_HPP

#include "functors.hpp"
#include "operator_proxy.hpp"

#include <functional>

namespace pop
{
#define REMOVE_PARENS(...) __VA_ARGS__
        
#define DEFAULT_BINARY_OPERATOR( op , functor )                                                                               \
        template<typename LHS , typename RHS>                                                                                 \
        auto operator REMOVE_PARENS op( const pop::operand<LHS>& lhs , const pop::operand<RHS>& rhs ) ->                                                   \
            decltype( pop::make_proxy( std::bind( pop::operators::functor{} , std::cref(lhs.get()) , std::cref(rhs.get()) ) ) ) \
        {                                                                                                                     \
            return pop::make_proxy( std::bind( pop::operators::functor{} , std::cref(lhs.get()) , std::cref(rhs.get()) ) );     \
        }    
   
    template<typename LHS , typename RHS>                                                                                 
    auto operator==( const pop::operand<LHS>& lhs , const pop::operand<RHS>& rhs ) ->                                                   
        decltype( pop::make_proxy( std::bind( pop::operators::equal{} , std::cref(lhs.get()) , std::cref(rhs.get()) ) ) ) 
    {                                                                                                                     
        return pop::make_proxy( std::bind( pop::operators::equal{} , std::cref(lhs.get()) , std::cref(rhs.get()) ) );     
    } 
    
    //DEFAULT_BINARY_OPERATOR((==),equal);
    DEFAULT_BINARY_OPERATOR((!=),not_equal);
    DEFAULT_BINARY_OPERATOR((>) ,bigger_than);
    DEFAULT_BINARY_OPERATOR((<) ,less_than);
    DEFAULT_BINARY_OPERATOR((>=),bigger_or_equal);
    DEFAULT_BINARY_OPERATOR((<=),less_or_equal);

    DEFAULT_BINARY_OPERATOR((+),add);
    DEFAULT_BINARY_OPERATOR((-),sub);
    DEFAULT_BINARY_OPERATOR((*),mul);
    DEFAULT_BINARY_OPERATOR((/),div);

    DEFAULT_BINARY_OPERATOR((&&),logical_and);
    DEFAULT_BINARY_OPERATOR((||),logical_or);

    DEFAULT_BINARY_OPERATOR((&),bitwise_and);
    DEFAULT_BINARY_OPERATOR((|),bitwise_or);
    DEFAULT_BINARY_OPERATOR((^),bitwise_xor);
    DEFAULT_BINARY_OPERATOR((<<),leftshift);
    DEFAULT_BINARY_OPERATOR((>>),rightshift);

    DEFAULT_BINARY_OPERATOR((,),comma);
}

#endif	/* OPERATORS_HPP */

