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
        template<typename T , typename is_rvalue , typename is_const = std::false_type>
        struct operand
        {
        public:
            template<typename... ARGS>
            operand( ARGS&&... args ) : 
                _op_ref{ std::forward<ARGS>( args )... }
            {}

            const T& get() const
            {
                return _op_ref.get();
            }
            
            explicit operator T&() const
            {
                return get();
            }
        private:
            std::reference_wrapper<T> _op_ref;
        };
        
        template<typename T , typename is_const>
        struct operand<T,std::true_type,is_const>
        {
        public:
            template<typename... ARGS>
            operand( ARGS&&... args ) : 
                _op{ std::forward<ARGS>( args )... }
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
                                       std::is_rvalue_reference<T>,
                                       std::is_const<T>>;
    
    template<typename T>
    pop::operand<T> wrap( T&& op )
    {
        return { op };
    }
    
    template<typename T , typename... ARGS>
    pop::operand<T> make_op( ARGS&&... args )
    {
        return { args... };
    }
}

#endif	/* OPERAND_HPP */

