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

using namespace std::placeholders;

#define REMOVE_PARENS(...) __VA_ARGS__

#define POLYOP_DEFAULT_OPERATORS( op , functor ) \
template<typename LHS , typename RHS>

        
#define BINARY_OP_WRAPPER( op , functor )                                                                                 \
        template<typename LHS , typename RHS , typename CONTEXT>                                                                                   \
        auto operator REMOVE_PARENS op( const pop::operand<LHS>& lhs , const pop::operand<RHS>& rhs )                           \
        {                                                                                                                       \
            return pop::make_proxy( pop::operators::functor<void(LHS,RHS)>{} , lhs , rhs );     \
        }                                                                                                                       \
                                                                                                                                \
        template<typename LHS , typename RHS>                                                                                   \
        auto operator REMOVE_PARENS op( const pop::operand<LHS>& lhs , const RHS& rhs )                                         \
        {                                                                                                                       \
            return pop::make_proxy( pop::operators::functor<void(LHS,RHS)>{} , lhs , rhs );     \
        }                                                                                                                       \
                                                                                                                                \
        template<typename LHS , typename RHS>                                                                                   \
        auto operator REMOVE_PARENS op( const LHS& lhs , const pop::operand<RHS>& rhs )                                         \
        {                                                                                                                       \
            return pop::make_proxy( pop::operators::functor<void(LHS,RHS)>{} , lhs , rhs );     \
        }
                                                                                                                                
    
BINARY_OP_WRAPPER((==),equal);
BINARY_OP_WRAPPER((!=),not_equal);
BINARY_OP_WRAPPER((>) ,bigger_than);
BINARY_OP_WRAPPER((<) ,less_than);
BINARY_OP_WRAPPER((>=),bigger_or_equal);
BINARY_OP_WRAPPER((<=),less_or_equal);

BINARY_OP_WRAPPER((+),add);
BINARY_OP_WRAPPER((-),sub);
BINARY_OP_WRAPPER((*),mul);
BINARY_OP_WRAPPER((/),div);

BINARY_OP_WRAPPER((&&),logical_and);
BINARY_OP_WRAPPER((||),logical_or);

BINARY_OP_WRAPPER((&),bitwise_and);
BINARY_OP_WRAPPER((|),bitwise_or);
BINARY_OP_WRAPPER((^),bitwise_xor);
BINARY_OP_WRAPPER((<<),leftshift);
BINARY_OP_WRAPPER((>>),rightshift);

BINARY_OP_WRAPPER((,),comma);


#endif	/* OPERATORS_HPP */

