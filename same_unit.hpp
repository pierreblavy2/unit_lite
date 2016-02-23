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


#ifndef SRC_LIB_UNIT_LITE_SAME_UNIT_HPP_
#define SRC_LIB_UNIT_LITE_SAME_UNIT_HPP_

#include<tuple>

//static type check
namespace unit_lite{
	template<typename T, typename U>  struct same_unit;

	template<typename T, typename U>  struct Quantity;

	template<typename T, typename U>  struct Compose_unit;

	namespace impl{
		template<typename T, typename U> struct Compose_unit_add;
	}
}



namespace unit_lite{

//specialize same_unit for quantities
template<
   typename T1, typename R1, typename V1,
   typename T2, typename R2, typename V2
>
struct same_unit<
  Quantity< Compose_unit<T1,R1>,V1> ,
  Quantity< Compose_unit<T2,R2>,V2>
>{
private:
	typedef unit_lite::Compose_unit<T1,R1> c1;
	typedef unit_lite::Compose_unit<T2,R2> c2;
	typedef unit_lite::impl::Compose_unit_add<c1,c2> checker_t;
public:
	static constexpr bool value = checker_t::run();
};




//specialize same_unit for Compose_unit
template<typename T1, typename R1, typename T2, typename R2>
struct same_unit<
  Compose_unit<T1,R1> ,
  Compose_unit<T2,R2>
>{
private:
	typedef unit_lite::Compose_unit<T1,R1> c1;
	typedef unit_lite::Compose_unit<T2,R2> c2;
	typedef unit_lite::impl::Compose_unit_add<c1,c2> checker_t;
	public:
	static constexpr bool value = checker_t::run();
};

template<>
struct same_unit<
  Compose_unit<std::tuple<>,std::tuple<>> ,
  Compose_unit<std::tuple<>,std::tuple<>>
>{
	static constexpr bool value = true;
};


//specialize for a mix of Compose_unit and Quantity
template<
  typename T1, typename R1,
  typename T2, typename R2,typename V2>
struct same_unit<
  Compose_unit<T1,R1> ,
  Quantity< Compose_unit<T2,R2>,V2>
>{
	private:
	typedef unit_lite::Compose_unit<T1,R1> c1;
	typedef unit_lite::Compose_unit<T2,R2> c2;
	typedef unit_lite::impl::Compose_unit_add<c1,c2> checker_t;
	public:
	static constexpr bool value = checker_t::run();
};


//specialize for a mix of Quantity and Compose_unit
template<
  typename T1, typename R1,typename V1,
  typename T2, typename R2>
struct same_unit<
  Quantity< Compose_unit<T1,R1>,V1> ,
  Compose_unit<T2,R2>
>{
	private:
	typedef unit_lite::Compose_unit<T1,R1> c1;
	typedef unit_lite::Compose_unit<T2,R2> c2;
	typedef unit_lite::impl::Compose_unit_add<c1,c2> checker_t;
	public:
	static constexpr bool value = checker_t::run();
};




}//end namespace unit_lite

#endif /* SRC_LIB_UNIT_LITE_SAME_UNIT_HPP_ */
