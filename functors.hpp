/* 
 * File:   functors.hpp
 * Author: manu343726
 *
 * Created on 10 de julio de 2014, 17:31
 */

#ifndef FUNCTORS_HPP
#define	FUNCTORS_HPP

namespace pop
{
    namespace operators
    {
#define REMOVE_PARENS(...) __VA_ARGS__
        
#define BINARY_OPERATOR_FTOR( op , name ) template<typename SIGNATURE>                              \
                                          struct name;                                              \
                                                                                                    \
                                          template<typename R , typename... ARGS>                   \
                                          struct name<R(ARGS...)>                                   \
                                          {                                                         \
                                              name( const name&) = default;                         \
                                                                                                    \
                                              template<typename CONTEXT>                            \
                                              auto operator()( CONTEXT rhs ) const                  \
                                              {                                                     \
                                                  return [](ARGS...){} REMOVE_PARENS op rhs;        \
                                              }                                                     \
                                          };
        
        BINARY_OPERATOR_FTOR((==),equal);
        BINARY_OPERATOR_FTOR((!=),not_equal);
        BINARY_OPERATOR_FTOR((>) ,bigger_than);
        BINARY_OPERATOR_FTOR((<) ,less_than);
        BINARY_OPERATOR_FTOR((>=),bigger_or_equal);
        BINARY_OPERATOR_FTOR((<=),less_or_equal);
        
        BINARY_OPERATOR_FTOR((+),add);
        BINARY_OPERATOR_FTOR((-),sub);
        BINARY_OPERATOR_FTOR((*),mul);
        BINARY_OPERATOR_FTOR((/),div);
        
        BINARY_OPERATOR_FTOR((&&),logical_and);
        BINARY_OPERATOR_FTOR((||),logical_or);
        
        BINARY_OPERATOR_FTOR((&),bitwise_and);
        BINARY_OPERATOR_FTOR((|),bitwise_or);
        BINARY_OPERATOR_FTOR((^),bitwise_xor);
        BINARY_OPERATOR_FTOR((<<),leftshift);
        BINARY_OPERATOR_FTOR((>>),rightshift);
        
        BINARY_OPERATOR_FTOR((,),comma);
        
    }
}

#endif	/* FUNCTORS_HPP */

