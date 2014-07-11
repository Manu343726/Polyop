Polyop
======

Overridable universal operators overloading for C++11


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

But, **what if that default operator semantics don't meet our requeriments?** Thats where Polyop kicks in.   

Consider the common floating-point comparison issue:

``` cpp
float a = 2.0f , b = 3.0f

assert( a + a - b == 1.0f );
```

Floating-point arithmetic suffers from precission errors due to mantissa rounding on certain operations (Primarily additions and substractions), so in the example above the assertion could fail even it that condition seems trivial. In fact C/C++ compilers never optimize floating-point arithmetic expressions since floating-point operations are not conmutative, associative, nor pure (A function is considered pure if an aplication always return the same result for the same argumments. Thats not entirely true for FP).

So, as you can see, **the default semantics of the equality operator don't work well on some contexts**. You could write a fp-aware comparison function ala Java, like this:

``` cpp
bool compare( float x , float y )
{
    return std::abs( x - y ) < 0.00001f;
}
```

But that may break C++ readability, since in one situation you use `compare()` and `==` in others.  
**What Polyop does is to provide a way to customize the default behaviour of C++ operators for different contexts**:

``` cpp
assert( pop::wrap( a + a - b ) == 1.0f );
```

Now the assertion never fails since the user provided a fp-aware semantic for `operator==()` using Polyop.

## How it works?

Since C++ operators work through overloading, we need a custom daratype to change the semantics of an operator. If you want to change the semantics of an exsiting type with its own operator overload you have to wrap it in a custom type.
The function `pop::wrap()` takes an operand (lvalue or rvalue) and efficiently stores it to be used as an operand of a Polyop operator. In other words, `pop::wrap()` triggers the usage of a Polyop operator instead of the default C++ one.

``` cpp
int a , b;

a == b;            //Calls operator==(int,int)
cpp::wrap(a) == b; //Calls custom Polyop operator== ( operator==( pop::operand<int> , int ) )
```

Polyop operators aer evaluated lazily by default, and they are manipulable entities, allowing you to manipulate the expression before the call is even applied, do partial operator application, or even store the operator expression:

``` cpp
int a , b;

auto comp = pop::wrap( a ) == b; //The comparison is not executed but stored in comp.
bool r1 = comp(); //Call the comparison.
auto expression = __ == __;          //Store a naked comparison expression
auto partial_call = expression( a ); //Pass the first argumment to the expression
bool r2 = partial_call( b );         //Pass the last argumment to the expression (Then calling the operator).

bool lex_result = (pop::wrap( a ) == b ).context( lexicographical ); /Applies a "lexicographical" comparison context.

```
