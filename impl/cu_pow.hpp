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



#ifndef SRC_LIB_UNIT_LITE_IMPL_CU_POW_HPP_
#define SRC_LIB_UNIT_LITE_IMPL_CU_POW_HPP_



namespace unit_lite{
namespace impl{
	//T is a std::ratio
	//U is a Compose_unit
	template<typename T, typename U>
	struct Compose_unit_pow;
}}//end namespace unit_lite::impl




namespace unit_lite{
namespace impl{
	template<
	std::intmax_t Num,   std::intmax_t Den,
	typename A_tag,typename A_ratio
	>
	struct Compose_unit_pow<
	  std::ratio<Num,Den>,
	  Compose_unit<A_tag,A_ratio>
	>{
		typedef std::ratio<Num,Den> exponent;
		typedef Compose_unit<A_tag,A_ratio> A_type;

		//multiply every ratio by exponent
		template<size_t I, typename previous>
		struct Mult_r{

			typedef typename A_type::template ratio<I-1> ratio_i;

			typedef typename tuple_cat_arg<
					typename Mult_r<I-1,previous>::type,
					std::ratio_multiply<ratio_i,exponent>
			>::type type;
		};

		template< typename previous>
		struct Mult_r<0,previous>{typedef previous type;};

		typedef typename Mult_r<
				std::tuple_size<A_tag>::value,
				std::tuple<>
		>::type ratio_type;

		typedef Compose_unit<A_tag,ratio_type> type;

	};


	//null exponent -> return dimension-less
	template<
	std::intmax_t Den,
	typename A_tag,typename A_ratio
	>
	struct Compose_unit_pow<
	  std::ratio<0,Den>,
	  Compose_unit<A_tag,A_ratio>
	>{
		typedef Compose_unit<std::tuple<>,std::tuple<> > type;
	};
}}//end namespace unit_lite::impl





#endif /* SRC_LIB_UNIT_LITE_IMPL_CU_POW_HPP_ */
