/* 
 * File:   operand.hpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 15:58
 */

#ifndef OPERAND_HPP
#define	OPERAND_HPP

#include <type_traits>
#include <utility>
#include <functional>

namespace pop
{
    namespace impl
    {
        template<typename T , typename is_lvalue>
        struct operand
        {
        public:
            operand( const T& r ) : 
                _op_ref{ r }
            {}
                
            operand( const operand& ) = default;

            const T& get() const
            {
                return _op_ref.get();
            }
            
            explicit operator T&() const
            {
                return get();
            }
        private:
            std::reference_wrapper<const T> _op_ref;
        };
        
        template<typename T>
        struct operand<T,std::false_type>
        {
        public:
            operand( T&& op ) : _op{ std::move( op ) }
            {}

            T& get()
            {
                return _op;
            }
            
            const T& get() const
            {
                return _op;
            }
            
            explicit operator const T&() const
            {
                return get();
            }
            
            explicit operator T() const
            {
                return get();
            }
        private:
            T _op;
        };
    }
    
    template<typename T>
    using operand = pop::impl::operand<typename std::decay<T>::type,
                                       std::is_lvalue_reference<T>>;
    
    template<typename T>
    pop::operand<T> wrap( T&& op )
    {
        return { std::forward<T>( op ) };
    }
    
    template<typename T , typename... ARGS>
    pop::operand<T> make_op( ARGS&&... args )
    {
        return { args... };
    }
    
    namespace trigger
    {
        template<typename T>
        pop::operand<T> _( T&& op )
        {
            return pop::wrap( std::forward<T>( op ) );
        }
    }
}

#endif	/* OPERAND_HPP */

