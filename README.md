Polyop
======

Overridable universal operator overloading for C++14


## What is Polyop?

C++ supports operator overloading, allowing the users to specify custom semantics for that operators when they are used together with user defined types. For example:

``` cpp

struct foo{};
     
std::ostream& operator<<( std::ostream& os , const foo& )
{
    return os << "a foo!";
}
```

That specification works using function overloads, so **that semantics are fixed and unique**. Of course all C++ basic and library types have their own operator overloads, for example `1 + 1` is allowed since there is a `operator+(int,int)` overload.

But, **what if that default operator semantics don't meet our requirements?** Thats where Polyop kicks in.   

Consider the common floating-point comparison issue:

``` cpp
float a = 2.0f , b = 3.0f

assert( a + a - b == 1.0f );
```

Floating-point arithmetic suffers from precission errors due to mantissa rounding on certain operations (Primarily additions and subtractions), so in the example above the assertion could fail even it that condition seems trivial. In fact C/C++ compilers never optimize floating-point arithmetic expressions since floating-point operations are not commutative, associative, nor pure (A function is considered pure if an application always return the same result for the same arguments. Thats not entirely true for floating-point ops).

So, as you can see, **the default semantics of the equality operator don't work well on some contexts**. You could write a fp-aware comparison function ala Java, like this:

``` cpp
bool compare( float x , float y )
{
    return std::abs( x - y ) < 0.00001f;
}
```

But that may break C++ readability, since in one situation you use `compare()` and `==` in others.  
**What Polyop does is to provide a way to customize the default behavior of C++ operators for different contexts**:

``` cpp
assert( pop::wrap( a + a - b ) == 1.0f );
```

Now the assertion never fails since the user provided a fp-aware semantic for `operator==()` using Polyop.

## How it works?

Since C++ operators work through overloading, we need a custom datatype to change the semantics of an operator. If you want to change the semantics of an existing type with its own operator overload you have to wrap it in a custom type.
The function `pop::wrap()` takes an operand (lvalue or rvalue) and efficiently stores it to be used as an operand of a Polyop operator. In other words, `pop::wrap()` triggers the usage of a Polyop operator instead of the default C++ one.

``` cpp
int a , b;

a == b;            //Calls operator==(int,int)
cpp::wrap(a) == b; //Calls custom Polyop operator==
```

Polyop operators are evaluated lazily by default, and they are manipulable entities, allowing you to manipulate the expression before the call is even applied, do partial operator application, or even store the operator expression:

``` cpp
int a , b;

auto comp = pop::wrap( a ) == b; //The comparison is not executed but stored in comp.
bool r1 = comp(); //Call the comparison.
auto expression = __ == __;          //Store a naked comparison expression
auto partial_call = expression( a ); //Pass the first argumment to the expression
bool r2 = partial_call( b );         //Pass the last argumment to the expression (Then calling the operator).

bool lex_result = (pop::wrap( a ) == b ).context( lexicographical ); /Applies a "lexicographical" comparison context.

```

### Ok, how it *really* works?

Using the `pop::operand` template Polyop wraps all the binary operators which request for a Polyop operator call, that is, any operator which one of its
operands is a `pop::operand` instance. Then a proxy is generated storing the call signature and the call arguments. Is that proxy what a call like `pop::wrap(1) + 2` returns.
Then the proxy is called using the specified context (`pop::default_operator` by default) or an implicit call is done due to a implicit conversion from the operator expression to
the result type.

The user context customization works through *operator dispatchers*. An operator dispatcher is a particular overload of the required operator which specifies the action to be done 
when the corresponding Polyop operator is called:

``` cpp

auto operator==(void(float,float) , float_context_tag )
{
    return []( float x , float y )
    {
        return /* some floating-point aware comparison */;
    };
}
```  
The idea behind operator dispatchers is to provide an alternative syntax to operator overloading, similar to the original, but which allows to specify context information. 

The first argument (Which only is used to carry semantic/overloading info) specifies the operator signature. In the example, `operator==(float,float)` (Note the return type is ignored).  
The second argument specifies in what context the operator should be applied. By default contexts are specified by type tags, and the context resolution done though overload
resolution. This approach has the advantage that the compiler is capable of inline all the Polyop machinery and only generate the code which really does the work (The body of
the operator dispatcher action in this case). But exactly because that reason **operator dispatchers should be declared/defined on the same namespace of their context tags**,
to allow the compiler to find the overload via ADL.

## Performance 

Polyop operators are designed to act as high-level syntactic sugar, with no runtime performance hits at all. All the Polyop machinery is erased at compile-time with minimal compiler optimizations (Common day to day inlining), generating only the code provided by the user. [Here](http://goo.gl/eF4zyp) is an example of a compilation with minimal optimization enabled (`-O1`) of this program:

``` cpp
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

```

There is not output code for `pop::operand`, `pop::operator_proxy`, nor reference wrappers. The only code generated was the two Polyop operator dispatchers specified in the example, but with their body empty. Erasing something that depends on a function pointer is very hard for the compiler, since the pointer erases a lot of useful compile-time info. On the other hand, note how it successfully identified what the dispatcher does and its body was completely inlined:

``` asm 
pop::operator==(void (*)(float, float), pop::default_operator_tag):
	movl	$0, %eax
	ret
operator==(void (*)(float, float), floating_point_context_tag):
	movl	$0, %eax
	ret
```
