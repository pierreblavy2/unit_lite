# Features 
* Check at compile time unit consistency
* Have zero run time overhead (but slower compilation)
* Easily print numbers with their underlying unit
* Handles explicit conversions between multiples of a unit, like meters to centimeters.
* Requires c++14 support
* ![LGPL](https://img.shields.io/github/license/pierreblavy2/unit_lite)

# Introduction

Unit lite is an alternative to boost units that handle compile-time checking of unit consistency, multiples of units and unit pretty print. This library is designed to be more user friendly than the boost one with easy to use syntax, faster compilation, and easier to read error messages. 

This library is published under the LGPL 3.0 license. 




# How to use
## Installation

* Download the latest stable version on github, and store it in your library folder
* Add it to the include path. (in gcc : -I/path/unit_lite)
* Compile with c++14 or any latter version (in gcc : -std=c++14)
* #include <unit_lite/unit_lite.hpp>

## Define a unit system

Units can be defined anywhere, and your can spread their definition across your code. We recommend you to organise them in namespaces to avoid confusion.


```cpp
#include <unit_lite/unit_lite.hpp>

//declare base units
namespace myunits{
  UL_DECLARE_UNIT(meter   ,double,  "m")     //length, in meter
  UL_DECLARE_UNIT(second  ,double,  "sec")   //duration in second
  UL_DECLARE_UNIT(nb_boys, integer, "boys")  //number of boys
  UL_DECLARE_UNIT_N (banana, integer);       //shortcut for UL_DECLARE_UNIT(banana, integer,"banana")
}

//compose units
namespace myunits{
 UL_COMPOSE_UNIT(m_per_s, meter/second); //meters per second
}
```


## UL_DECLARE_UNIT(name,type,print_str)
This macro defines
* name_tag : a struct that makes template magic works, that triggers readable error messages, and that holds print and multiple infromation
* name_quantity, the type of this units
* name : a quantity of 1 name_quantity

## UL_DECLARE_UNIT_N(name,type,print_str)
This macro is a shortcut that does the same thing as UL_DECLARE_UNIT(name,type,"name")

## UL_COMPOSE_UNIT(name,formula)
This macro defines
* name_quantity the type of the result of the formula
* name : a quantity of 1 name_quantity


## UL_IMPORT_UNIT(full_name)
This macro import the unit from one namespace into an other, according to the following example.

```cpp
namespace myunits{
  UL_DECLARE_UNIT_N(meter   ,double)     //length, in meter
}


namespace an_other_namespace{
  UL_IMPORT_UNIT(::myunits::meter)
  //define meter_tag, meter and meter_quantity here.
}
```


## Quantity algebra
The commons arithmetic operators are defined to work with units, so you don't have to do anything special to make common algebra works.

```cpp
#include <unit_lite/unit_lite.hpp>

//declare base units
namespace myunits{
  UL_DECLARE_UNIT(meter   ,double,"m")     //length, in meter
  UL_DECLARE_UNIT(second  ,double,"s")     //duration in second
}

//compute things
void f(){
  using namespace myunits;

  //define quantities
  meter_quantity m1 = 2.0 * meter; //[1]
  meter_quantity m2 = meter_quantity::from_value(10.0); //[2]
  auto           m3 = meter_quantity::from_value(10.0); //[3]
  auto           s  = second_quantity::from_value(5.0); //[4]
  meter_quantity m4; m4.value=5.5; //[5]

  //compute
  auto m_per_s = (m1+m2+m3)/s;  //[6]
  double d     = m_per_s.value; //[7]

  //conversion from dimensionless
  auto x = m1/m2;  //x is a dimensionless quantity
  double d2=x ;    //conversion to double works because x is dimensionless
  //double error=m1; //as m1 has a dimension, this conversion fails, instead use:
  double ok=m1.value; //but, you can always extract the underlying type with .value

}
```


## Create quantities
The canonical way to define a quantity is to use the static from_value(x) function of the wished quantity (see [2-4]). This function expects an argument of the same type as the underlying value type of your quantity.

The other way is to use arithmetic (see [1]) by multiplying a qualtity of one by a scalar. Note that the scalar has to be exactly the same type as the underlying type. One common error is to write meter_quantity m1 = 2 * meter; . As meter has an underlying type of double, this code will crash. To fix it write 2.0 (i.e., a double) instead of 2 (i.e., an int).



## Get and set the underlying value
As each quantity has a .value member, you can use it to read (see [7]) or change (see [5]) the underlying value of an existing quantity. When you use .value, no unit consistency check is performed.



## Conversion
Conversion between units of different kinds (like meters to seconds) always trigger an error [1].

Conversion from a raw type (like double) to a quantity always always triggers an error [2]. To fix it you can set the .value member, use from_value, or multiply by the correct unit [3-5].

Conversion from a quantity to the underlying type works implicitly only for dimensionless units [6-7].

```cpp
meter_quantity m1 = 1.0 * meter; 
meter_quantity m2 = 2.0 * meter; 
auto           s =  5.0 * second;

s=m1;      // [1] Error : metters to seconds     
m2 = 5.0;  // [2] Error : raw type (i.e. double), to unit       

m2 = 5.0 * meter;                     //[3] OK
m2 = meter_quantity::from_value(5.0); //[4] OK
m2.value = 5.0;                       //[5] OK

double d1 = m1/m2; //[6] OK    : dimensionless
double d2 = m1;    //[7] ERROR , use double d2=m1.value;
```




## Multiples of units
### The nanometers + kilometers problem
Let's suppose that some magic that makes the meter+centimeter arithmetic works, and that all quantities are using double as the underlying value type. Then, we can write the following code :

```cpp
auto a = 10.0*meter;
auto b = 5.0 *centimeter;
auto c = a+b; //expected c=10.05*meter or maybe 1005.0*centimeter
```

Such code looks perfectly legal, and we may think that such automatic conversions are a good way to save time, but it's a trap. Let's have a look at the following example
```cpp
auto f(
  const nanometer_quantity &x, 
  const kilometer_quantity &y
){
  return x+y;
}
```

When we write f, we cannot know in which type we expect the result, as if we call the function on large distances we expect a result in kilometers, so the double don't overflow, and if we use it on small distances, we expect a result in nanometers so we keep precision. Therefore, there is no automatic conversion in unit_lite.

Instead you can perform an explicit conversion with the .to member function.

### Defining a multiple of a unit, and explicit conversion

```cpp
#include <unit_lite/unit_lite.hpp>

namespace myunits{
UL_DECLARE_UNIT(m,double);
UL_MULTIPLE(cm, m,1,100 ,"cm");  //
UL_MULTIPLE(mm,cm,1,10  ,"mm");  // equivalent to UL_MULTIPLE(mm,m,1,1000 , "mm");
}

#include <iostream>
int main() {
	using namespace myunits;
	auto b = 5.0*pow<-3>(mm);
	auto c = b.to<decltype( 1.0/(cm*cm*cm) )>() ;
	std::cout << c; //0.005 cm^-3

	//FAIL conversion between dimensionless
	//auto d=cm/cm;
	//auto e=cm/cm;
	//auto f = d.to<decltype(e)>();
}

```

The UL_MULTIPLE(new_unit, old_unit,ratio_num,ratio_den) macro allow you to define the new unit as a multiple of the old_unit with a conversion ratio from old_unit to new_unit of ratio_num/ratio_den.

Note that, when you call UL_MULTIPLE, you can use any previously defined multiple as old_unit. If you do so, the compiler will automatically compute a ratio related to the base unit (here m), Therefore UL_MULTIPLE(mm,cm,1,10) is the same as UL_MULTIPLE(mm,m,1,1000), with one extra multiplication at compile time, but no extra run-time overhead.

Conversion from or to dimensionless units is not allowed, as there is no automatic way to compute the conversion ratio.

If you use a custom value type in your quantities, you can specialize the way the ratio is multiplied with your units to make conversions. To do so, write a specialization of the multiply_by_ratio_t struct. You can get the ratio numerator with ratio_t::num and the denominator with ratio_t::den. Such values are compile time integers.

```cpp
namespace unit_lite{
  template<std::intmax_t N, std::intmax_t D>
  struct multiply_by_ratio_t<  std::ratio<N,D>, YOUR_VALUE_TYPE >{
    typedef typename std::ratio<N,D>::type ratio_t;
    static YOUR_VALUE_TYPE run(const YOUR_VALUE_TYPE & source){
	YOUR CODE
    }
  };
}
```


# Read errors messages
The units tempalte magic is handled by a class called Compose_unit. If this terms appears in your error messages, you're certainly doing something wrong with your units. Check your log for a static assert, this line will explain you what's wrong. Then crawl up the log to get the lines related to Compose_unit, so you can get details on which types triggered the error.

This class stores units as two types of tuple : the firs one stores Tags, the second one Ratios. Therefore a unit of metters per second is : std::tuple<meter_tag,second_tag>, std::tuple<std::ratio<1,1>, std::ratio<-1,1> >

Here is an example, where we try to define a variable crash in meters from a variable in meters per seconds².

```cpp
#include <unit_lite/unit_lite.hpp>

//declare base units
namespace myunits{
  UL_DECLARE_UNIT(meter   ,double , "m")     //length, in meter
  UL_DECLARE_UNIT(second  ,double , "s")     //duration in second
}

//compute things
void f(){
  using namespace myunits;

  //define quantities
  auto m = meter_quantity ::from_value(10.0);
  auto s = second_quantity::from_value(5.0);

  auto m_per_s2 = m/pow<2>(s);

  meter_quantity crash = m_per_s2;
}
```

This code produces the following error, that can be translated to "cannot assign meter per seconds² to meters".

```
In file included from /zdata/workspace/cpp-test/src/lib/unit_lite/unit_lite.hpp:6:0,
                 from ../src/cpp-test.cpp:10:
/zdata/workspace/cpp-test/src/lib/unit_lite/Quantity.hpp : dans l'instantiation de ‘unit_lite::Quantity<unit_lite::Compose_unit<T1, R1>, V1>::Quantity(const unit_lite::Quantity<X, Y>&) [with X = unit_lite::Compose_unit<std::tuple<myunits::meter_tag, myunits::second_tag>, std::tuple<std::ratio<1l, 1l>, std::ratio<-2l, 1l> > >; Y = double; T = std::tuple<myunits::meter_tag>; R = std::tuple<std::ratio<1l, 1l> >; V = double]’ :
../src/cpp-test.cpp:32:26:   required from here
/zdata/workspace/cpp-test/src/lib/unit_lite/Quantity.hpp:49:4: erreur: static assertion failed: cannot assign heterogeneous units
    static_assert(same_unit<unit_t, unit_q >::value,"cannot assign heterogeneous units" );
    ^
```
