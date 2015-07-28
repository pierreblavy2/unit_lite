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


#ifndef SRC_LIB_UNIT_LITE_LIMITS_HPP_
#define SRC_LIB_UNIT_LITE_LIMITS_HPP_
#include <limits>


namespace unit_lite{
	template<typename T,typename U> struct Quantity;
}


namespace std{
template<typename T,typename U>
struct numeric_limits<unit_lite::Quantity<T,U> >{
	//from : http://en.cppreference.com/w/cpp/types/numeric_limits

	typedef unit_lite::Quantity<T,U> quantity_t;
	typedef U value_t;


	static constexpr bool is_signed   = std::numeric_limits<U>::is_signed;
	static constexpr bool is_integer  = std::numeric_limits<U>::is_integer;
	static constexpr bool is_exact    = std::numeric_limits<U>::is_exact;
	static constexpr bool has_infinity= std::numeric_limits<U>::has_infinity;

	static constexpr bool  has_quiet_NaN    = std::numeric_limits<U>::has_quiet_NaN;
	static constexpr bool  has_signaling_NaN= std::numeric_limits<U>::has_signaling_NaN;
	static constexpr bool  has_denorm       = std::numeric_limits<U>::has_denorm;
	static constexpr bool  has_denorm_loss  = std::numeric_limits<U>::has_denorm_loss;

	static constexpr decltype(std::numeric_limits<U>::round_style)  round_style = std::numeric_limits<U>::round_style;


	static constexpr bool  is_ice559   = std::numeric_limits<U>::is_ice559;
	static constexpr bool  is_bounded  = std::numeric_limits<U>::is_bounded;
	static constexpr bool  is_modulo   = std::numeric_limits<U>::is_modulo;

	static constexpr decltype(std::numeric_limits<U>::digits      )  digits       = std::numeric_limits<U>::digits;
	static constexpr decltype(std::numeric_limits<U>::digits10    )  digits10     = std::numeric_limits<U>::digits10;
	static constexpr decltype(std::numeric_limits<U>::max_digits10)  max_digits10 = std::numeric_limits<U>::max_digits10;

	static constexpr decltype(std::numeric_limits<U>::radix)  radix = std::numeric_limits<U>::radix;

	static constexpr decltype(std::numeric_limits<U>::min_exponent  )  min_exponent   = std::numeric_limits<U>::min_exponent;
	static constexpr decltype(std::numeric_limits<U>::min_exponent10)  min_exponent10 = std::numeric_limits<U>::min_exponent10;
	static constexpr decltype(std::numeric_limits<U>::max_exponent  )  max_exponent   = std::numeric_limits<U>::max_exponent;
	static constexpr decltype(std::numeric_limits<U>::max_exponent10)  max_exponent10 = std::numeric_limits<U>::max_exponent10;

	static constexpr decltype(std::numeric_limits<U>::traps          )  traps           = std::numeric_limits<U>::traps;
	static constexpr decltype(std::numeric_limits<U>::tinyness_before)  tinyness_before = std::numeric_limits<U>::tinyness_before;

	static constexpr quantity_t min() {return quantity_t::from_value ( std::numeric_limits<U>::min() ) ;}
	static constexpr quantity_t max() {return quantity_t::from_value ( std::numeric_limits<U>::max() ) ;}

	static constexpr quantity_t lowest()  {return quantity_t::from_value ( std::numeric_limits<U>::lowest() ) ;}
	static constexpr quantity_t epsilon() {return quantity_t::from_value ( std::numeric_limits<U>::epsilon() ) ;}

	static constexpr quantity_t round_error()  {return quantity_t::from_value ( std::numeric_limits<U>::round_error() ) ;}
	static constexpr quantity_t infinity()     {return quantity_t::from_value ( std::numeric_limits<U>::infinity() ) ;}
	static constexpr quantity_t quiet_NaN()    {return quantity_t::from_value ( std::numeric_limits<U>::quiet_NaN() ) ;}
	static constexpr quantity_t signaling_NaN(){return quantity_t::from_value ( std::numeric_limits<U>::signaling_NaN() ) ;}
	static constexpr decltype(std::numeric_limits<U>::denorm_min()) denorm_min()   {return quantity_t::from_value ( std::numeric_limits<U>::denorm_min() ) ;}

	//typedef typename std::numeric_limits<U>::float_round_style  float_round_style;
	//typedef typename std::numeric_limits<U>::float_denorm_style float_denorm_style;

};

}//end namespace std




#endif /* SRC_LIB_UNIT_LITE_LIMITS_HPP_ */
