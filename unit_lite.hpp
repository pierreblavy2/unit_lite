//This program (i.e. the unit_lite library) is free software:
//you can redistribute it and/or modify it under the terms of
//the GNU General Public License as published by the Free
//Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program. If not, see the two following links
//https://www.gnu.org/licenses/
//https://www.gnu.org/licenses/lgpl-3.0.txt

//This program was written by Pierre Blavy.
//Documentation is available on
//https://tentacule.be/fossil/cpp-msmodels/wiki?name=unit_lite

#ifndef SRC_LIB_UNIT_LITE_UNIT_LITE_HPP_
#define SRC_LIB_UNIT_LITE_UNIT_LITE_HPP_


#include "Compose_unit.hpp"
#include "Quantity.hpp"

#include "impl/cu_add.hpp"
#include "impl/cu_mult.hpp"
#include "impl/cu_pow.hpp"
#include "limits.hpp"


#include <type_traits>
#include <ostream>
#include <string>

//Multiple is used for auto conversion chains.
//This class store multiple of a unit to the related BASE unit.
//template<typename Me> struct Multiple;



//easy to use macro

//use UL_DECLARE_UNIT in your namespace
#define UL_DECLARE_UNIT(name,value_t,print_str)\
	struct  name##_tag  {\
		name##_tag()=delete;\
		typedef std::ratio<1,1>factor;\
		typedef name##_tag base_tag;\
		static std::string str(){return print_str;}\
\
};\
	typedef unit_lite::Compose_unit<std::tuple<name##_tag>, std::tuple<std::ratio<1,1> >  >  name##_unit;\
	const auto name = unit_lite::Quantity<name##_unit, value_t>::from_value(static_cast<value_t>(1));\
	typedef std::remove_const<decltype(name)>::type name##_quantity;\


#define UL_DECLARE_UNIT_N(name,value_t)\
	UL_DECLARE_UNIT(name,value_t,#name)

/*
//use UL_PRINT_UNIT in global namespace
#define UL_PRINT_UNIT(name,print)\
namespace unit_lite{\
  template<> struct print_tag<name##_tag>{static std::string str(){return print;} };\
}
*/

//use it in the namespace where you want to import your unit
#define UL_IMPORT_UNIT(name)\
	using name;\
	using name##_tag;\
	using name##_quantity;\

#define UL_IMPORT_COMPOSED_UNIT(name)\
	using name;\
	using name##_quantity;\


//use it in the namespace where you want to define your unit
#define UL_COMPOSE_UNIT(name,formula)\
	typedef std::remove_const<decltype((formula))>::type name##_quantity; \
	const auto name = name##_quantity::from_value(static_cast<name##_quantity::value_t>(1)) ;\




//use it in the namespace where you want to define your multiple
//don't forget to use UL_PRINT_UNIT outside your namespace to define how to print the unit
//e.g.
//namespace myunits{UL_MULTIPLE(cm,m,1,100)}
//UL_PRINT_UNIT(myunits::cm,"cm")
//note
//you can chain units definitions
//UL_MULTIPLE(dm,m,1,10)
//UL_MULTIPLE(cm,dm,1,10) //equivalent to UL_MULTIPLE(cm,m,1,100)
#define UL_MULTIPLE(ME,BASE,R1,R2,print_str)\
struct ME##_tag{\
  ME##_tag()=delete;\
  typedef BASE##_tag::base_tag base_tag;\
  typedef std::ratio_multiply< std::ratio<R2,R1>, BASE##_tag::factor >factor;\
  static std::string str(){return print_str;}\
};\
\
typedef unit_lite::Compose_unit<std::tuple<ME##_tag>, std::tuple<std::ratio<1,1> >  >  ME##_unit;\
const auto ME = unit_lite::Quantity<ME##_unit, decltype(BASE.value)>::from_value(static_cast<decltype(BASE.value)>(1));\
typedef std::remove_const<decltype(ME)>::type ME##_quantity;\








namespace unit_lite{
	template<typename Tag_t> struct print_tag;
}


template<
	typename U1,
	typename V1,
	typename T,
	typename = typename std::enable_if< std::numeric_limits<T>::is_integer, T>::type
>
auto operator<<(const ::unit_lite::Quantity<U1,V1> &a , T t ){
	auto v2=a.value<<t;
	return unit_lite::Quantity<U1,decltype(v2)>::from_value(v2);
}

template<
	typename U1,
	typename V1,
	typename T,
	typename = typename std::enable_if< std::numeric_limits<T>::is_integer, T>::type
>
auto operator>>(const unit_lite::Quantity<U1,V1> &a , T t ){
	auto v2=a.value>>t;
	return unit_lite::Quantity<U1,decltype(v2)>::from_value(v2);
}



template<
  typename U1,typename V1,typename T,
  typename std::enable_if<std::numeric_limits<T>::is_integral , T>::type
>
unit_lite::Quantity<U1,V1>& operator <<=(unit_lite::Quantity<U1,V1> &a , const T &b ){
	a.value<<=b;
}


template<
  typename U1,typename V1,typename T,
  typename std::enable_if<std::numeric_limits<T>::is_integral , T>::type
>
unit_lite::Quantity<U1,V1>& operator >>=(unit_lite::Quantity<U1,V1> &a , const T &b ){
	a.value>>=b;
}

template<
  typename U1, typename V1,typename T,
  typename = typename std::enable_if<std::is_arithmetic<T>::value , T>::type
>
unit_lite::Quantity<U1,V1>& operator %=(unit_lite::Quantity<U1,V1> &a , const T&b ){
	a.value%=b.value;
	return a;
}






//Arithmetic functions
template<
  std::intmax_t num,
  std::intmax_t den=1,
  typename U1,typename V1
  >
auto pow(const unit_lite::Quantity<U1,V1> &t);

#include <cmath>
template<typename U1,typename V1>
bool  isnan( const unit_lite::Quantity<U1,V1> &a){
	return isnan(a.value ) ;
}










//Operators implementation




//operators binary + - %
template<
  typename U1, typename V1,
  typename U2, typename V2
>
auto operator +(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot add heterogeneous units" );
	typedef decltype(a.value + b.value) value_t;
	return unit_lite::Quantity<U1,value_t>::from_value(a.value + b.value);
}

template<
  typename U1, typename V1,
  typename U2, typename V2
>
auto operator -(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot substract heterogeneous units" );
	typedef decltype(a.value - b.value) value_t;
	return unit_lite::Quantity<U1,value_t>::from_value(a.value - b.value);

}


// % make sense only if the second argument is numeric or dimensionless
template<
  typename U1, typename V1,
  typename T, //real type
  typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
>
unit_lite::Quantity<U1,V1> operator %(const unit_lite::Quantity<U1,V1> &a, const T &b){
	return unit_lite::Quantity<U1,decltype(a.value%b) >::from_value(a.value%b);
}


template<  typename U1, typename V1,  typename U2, typename V2>
unit_lite::Quantity<U1,V1> operator %(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	static_assert(b.is_dimensionless,"The second arg of a % must be numeric or dimensionless");
	return a%b.value;
}



//++a
template<  typename U1, typename V1>
unit_lite::Quantity<U1,V1>& operator ++(unit_lite::Quantity<U1,V1> &a){
	++a.value;
	return a;
}

//--a
template<  typename U1, typename V1>
unit_lite::Quantity<U1,V1>& operator--(unit_lite::Quantity<U1,V1> &a){
	--a.value;
	return a;
}





//a++
template<  typename U1, typename V1>
unit_lite::Quantity<U1,V1> operator ++(unit_lite::Quantity<U1,V1> &a,int){
	auto r = a;
	++a.value;
	return r;
}

//a--
template<  typename U1, typename V1>
unit_lite::Quantity<U1,V1> operator--(unit_lite::Quantity<U1,V1> &a, int){
	auto r = a;
	--a.value;
	return r;
}






// operator binary * /
template<
  typename U1, typename V1,
  typename U2, typename V2
>
auto operator *(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){

	typedef typename unit_lite::impl::Compose_unit_mult<U1,U2,std::ratio_add>::type unit_t;

	//no mix of multiple of the same base unit
	static_assert(
			unit_lite::impl::Compose_unit_noMix<typename unit_t::Tag_t>::value ,
			"cannot multiply heterogeneous multiples of units (like meters * centimeter)."
	);

	return unit_lite::Quantity<unit_t,decltype(a.value * b.value)>::from_value(a.value * b.value);
}


template<
  typename U1, typename V1,
  typename U2, typename V2
>
auto operator /(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	typedef typename unit_lite::impl::Compose_unit_mult<U1,U2,std::ratio_subtract>::type unit_t;

	//no mix of multiple of the same base unit
	static_assert(
			unit_lite::impl::Compose_unit_noMix<typename unit_t::Tag_t>::value ,
			"cannot divide heterogeneous multiples of units (like meters / centimeter)."
	);

	return unit_lite::Quantity<unit_t,decltype(a.value / b.value)>::from_value(a.value / b.value);
}



template< typename U1, typename V1>
unit_lite::Quantity<U1,V1> operator -(const unit_lite::Quantity<U1,V1> &a){
	return unit_lite::Quantity<U1,V1>::from_value(-a.value);
}



//scalar multiplication and division
template<typename U1, typename V1>
auto operator *(unit_lite::Quantity<U1,V1> a, const V1 &b){a.value*=b;return a;}

template<typename U1, typename V1>
auto operator *( const V1 &b,unit_lite::Quantity<U1,V1> a){a.value*=b;return a;}


template<typename U1, typename V1>
auto operator /(unit_lite::Quantity<U1,V1> a, const V1 &b){a.value/=b;return a;}

template<typename U1, typename V1>
auto operator /( const V1 &b,const unit_lite::Quantity<U1,V1> &a){
	return unit_lite::Quantity_dimensionless<V1>::from_value(b) / a;
}


template<typename U1, typename V1>
unit_lite::Quantity<U1,V1>& operator *=(unit_lite::Quantity<U1,V1> &a , const V1 &b ){
	a.value*=b;
	return a;
}

template<typename U1, typename V1>
unit_lite::Quantity<U1,V1>& operator /=(unit_lite::Quantity<U1,V1> &a , const V1 &b ){
	a.value/=b;
	return a;
}






//--- logical operatrors ---
template<typename U1, typename V1>
auto operator !(const unit_lite::Quantity<U1,V1>& a){
	return unit_lite::Quantity<U1, decltype(!a.value)>::from_value(!a.value );
}


template<typename U1, typename V1,typename U2, typename V2>
auto operator &&(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute and (&&) on heterogeneous units");
	return unit_lite::Quantity<U1, decltype(a.value && b.value)>::from_value(a.value && b.value );
}

template<typename U1, typename V1,typename U2, typename V2>
auto operator ||(const unit_lite::Quantity<U1,V1> &a, const unit_lite::Quantity<U2,V2> &b){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute or (||) on heterogeneous units");
	return unit_lite::Quantity<U1, decltype(a.value || b.value)>::from_value(a.value || b.value );
}



//bitewise operators
template< typename U1, typename V1>
unit_lite::Quantity<U1,V1> operator~(const unit_lite::Quantity<U1,V1> &a ){
	auto v2 = ~a.value;
	return unit_lite::Quantity<U1, decltype(v2)>::from_value( v2 );
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1> operator&(const unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute bitand (&) on heterogeneous units");
	auto v2 = a.value & b.value;
	return unit_lite::Quantity<U1, decltype(v2)>::from_value( v2 );
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1> operator|(const unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute bitor (|) on heterogeneous units");
	auto v2 = a.value | b.value;
	return unit_lite::Quantity<U1, decltype(v2)>::from_value( v2 );
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1> operator^(const unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute bit xor (^) on heterogeneous units");
	auto v2 = a.value ^ b.value;
	return unit_lite::Quantity<U1, decltype(v2)>::from_value( v2 );
}



//Compound assignment
template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator +=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute += on heterogeneous units");
	a.value+=b.value;
	return a;
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator -=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute -= on heterogeneous units");
	a.value-=b.value;
	return a;
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator *=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(
			b.is_dimensionless
			,"cannot compute *=, the right member must be dimensionless"
	);
	a.value*=b.value;
	return a;
}


template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator /=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(
		b.is_dimensionless
		,"cannot compute /=, the right member must be dimensionless"
	);
	a.value/=b.value;
	return a;
}



template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator &=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute &= on heterogeneous units");
	a.value&=b.value;
	return a;
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator |=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute |= on heterogeneous units");
	a.value|=b.value;
	return a;
}

template<typename U1, typename V1,typename U2, typename V2>
unit_lite::Quantity<U1,V1>& operator ^=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	static_assert(unit_lite::same_unit<U1,U2>::value,"cannot compute ^= on heterogeneous units");
	a.value^=b.value;
	return a;
}



/*
template<
typename U1,
typename V1,
typename T,
typename std::enable_if<std::numeric_limits<T>::is_integral , T>::type
>
unit_lite::Quantity<U1,V1>& operator <<=(unit_lite::Quantity<U1,V1> &a , const T &b ){
	a.value<<=b;
}


template<
typename U1,
typename V1,
typename T,
typename std::enable_if<std::numeric_limits<T>::is_integral , T>::type
>
unit_lite::Quantity<U1,V1>& operator >>=(unit_lite::Quantity<U1,V1> &a , const T &b ){
	a.value>>=b;
}



template<
typename U1, typename V1,
typename T,
typename = typename std::enable_if<std::is_arithmetic<T>::value , T>::type
>
unit_lite::Quantity<U1,V1>& operator %=(unit_lite::Quantity<U1,V1> &a , const unit_lite::Quantity<U2,V2> &b ){
	a.value%=b.value;
	return a;
}
*/



#include <cmath>



template<
  std::intmax_t num,
  std::intmax_t den=1,
  typename U1,typename V1
  >
auto pow(const unit_lite::Quantity<U1,V1> &t){

	typedef typename unit_lite::impl::Compose_unit_pow< std::ratio<num,den>  , U1 >::type unit_t;
    double exp=(double)num/(double)den;
	return unit_lite::Quantity<unit_t,decltype(pow(t.value,exp) )>::from_value(pow(t.value,exp));
}



#endif /* SRC_LIB_UNIT_LITE_UNIT_LITE_HPP_ */
