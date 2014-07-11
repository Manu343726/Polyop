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



int main()
{
    auto op = pop::wrap( 1.0f ) == pop::wrap( 1.0f );
    
    std::cout << std::boolalpha << op();
}

