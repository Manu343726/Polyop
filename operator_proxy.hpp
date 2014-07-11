/* 
 * File:   operator_proxy.hpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 16:53
 */

#ifndef OPERATOR_PROXY_HPP
#define	OPERATOR_PROXY_HPP

#include "operand.hpp"
#include "Turbo/type_traits.hpp"
#include "Turbo/utils/assert.hpp"

#include <functional>
#include <iostream>

namespace pop
{
    struct default_operator_tag{};
    extern const default_operator_tag default_operator;
    
    template<typename OP , typename LHS , typename RHS>
    struct operator_proxy
    {
        template<typename OP_DISPATCH , typename LHS_ , typename RHS_>
        operator_proxy( OP_DISPATCH&& op , LHS_&& lhs , RHS_&& rhs ) :
            _operator_dispatcher( op ),
            _lhs{ lhs },
            _rhs{ rhs }
        {}
            
        template<typename CONTEXT_TAG>
        auto operator()( CONTEXT_TAG tag ) const
        {
            auto op = _operator_dispatcher( tag );
            
            return op( _lhs.get() , _rhs.get() );
        }
        
        auto operator()() const
        {
            return (*this)( pop::default_operator );
        }
        
        template< typename CONTEXT_TAG>
        auto context( CONTEXT_TAG tag )
        {
            return (*this)( tag );
        }
        
        operator auto()
        {
            return (*this)();
        }
    private:
        OP _operator_dispatcher;
        pop::operand<LHS> _lhs;
        pop::operand<RHS> _rhs;
    };
    
    template<typename OP , typename LHS , typename RHS>
    pop::operator_proxy<typename std::decay<OP>::type,
                        typename std::decay<LHS>::type,
                        typename std::decay<RHS>::type> 
    make_proxy( OP&& operator_ , const pop::operand<LHS>& lhs , const pop::operand<RHS>& rhs )
    {
        return { operator_ , lhs , rhs };
    } 
    
    template<typename OP , typename LHS , typename RHS>
    pop::operator_proxy<typename std::decay<OP>::type,
                        typename std::decay<LHS>::type,
                        RHS> 
    make_proxy( OP&& operator_ , const pop::operand<LHS>& lhs , const RHS& rhs )
    {
        return { operator_ , lhs , rhs };
    } 
    
    template<typename OP , typename LHS , typename RHS>
    pop::operator_proxy<typename std::decay<OP>::type,
                        LHS,
                        typename std::decay<RHS>::type> 
    make_proxy( OP&& operator_ , const LHS& lhs , const pop::operand<RHS>& rhs )
    {
        return { operator_ , lhs , rhs };
    }
}


#endif	/* OPERATOR_PROXY_HPP */

