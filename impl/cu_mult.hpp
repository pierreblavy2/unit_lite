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



#ifndef SRC_LIB_UNIT_LITE_IMPL_CU_MULT_HPP_
#define SRC_LIB_UNIT_LITE_IMPL_CU_MULT_HPP_


#include "cu_rmNullRatio.hpp"
#include "tuple_typeMap.hpp"
#include "tuple_cat.hpp"



namespace unit_lite{
namespace impl{
	//you can always put heteregeneous stuff in a multiplication
	//this code help for building the multiplicative type by summing ratios
	template<
		typename A, //Compose_unit A
		typename B, //Compose_unit B
		template <typename,typename> class ratio_op //what to do with ratios (std::ratio_add for multiplication or  std::ratio_subtract for division)
	>
	struct Compose_unit_mult;
}
}




namespace unit_lite{
namespace impl{




	template<
		typename A_tag,typename A_ratio,
		typename B_tag,typename B_ratio,
		template <typename,typename> class  ratio_op
	>

	struct Compose_unit_mult
	<
	  Compose_unit<A_tag,A_ratio>,
	  Compose_unit<B_tag,B_ratio>,
	  ratio_op
	>{

		private:
		typedef  Compose_unit<A_tag,A_ratio> A_type;
		typedef  Compose_unit<B_tag,B_ratio> B_type;


		//step 1 : collect DISTINCT tags
		typedef tuple_typeMap<A_tag>    a_map;
		typedef typename a_map::template add_tuple<B_tag> distinct_tag_map;
		typedef typename distinct_tag_map::tuple_type distinct_tag;

		//step 2 : sum ratios
		template<size_t I, typename previous>
		struct Sum_r{
			typedef typename std::tuple_element<I-1,distinct_tag>::type tag_i;
			typedef typename A_type::template ratio_from_tag<tag_i>::type a_ratio;
			typedef typename B_type::template ratio_from_tag<tag_i>::type b_ratio;

			typedef ratio_op<a_ratio,b_ratio> sum_t; //todo change me for div


			typedef typename tuple_cat_arg<
					typename Sum_r<I-1,previous>::type,
					sum_t
			>::type type;
		};

		template< typename previous>
		struct Sum_r<0,previous>{typedef previous type;};

		typedef typename Sum_r<
				std::tuple_size<distinct_tag>::value,
				std::tuple<>
		>::type ratio_withNull;



		//step 3 : discard null ratios
		typedef  Compose_unit<distinct_tag,ratio_withNull > u_type;

		public:
		typedef typename Compose_unit_rmNullRatio<u_type>::type type;



	};

}}//end namespace unit_lite::impl




//for multiples : check that there is no mix like cm*m
//step 1 create the unit normally so cm*cm*m -> cm²*m
//step 2 check that the base dimensions appers only one with Compose_unit_noMix

namespace unit_lite{
namespace impl{

template<
  size_t I,
  typename Tuple,
  typename Previous
>
struct Compose_unit_noMix_r{
	typedef typename std::tuple_element<I-1,Tuple>::type tag;
	typedef typename tag::base_tag  base_tag;

	typedef Compose_unit_noMix_r<
			I-1,
			Tuple,
			typename tuple_cat_arg<Previous,base_tag>::type
	> cu;

	static constexpr bool tmp  = unit_lite::impl::tuple_has_type<Previous,base_tag>::value;
	static constexpr bool value = cu::value and !tmp;

};
//todo patch in doc
template<
  typename Tuple,
  typename Previous
>
struct Compose_unit_noMix_r<0,Tuple,Previous>{
	static constexpr bool value = true;
};

template<typename Tuple>
struct Compose_unit_noMix{
	private:
	typedef Compose_unit_noMix_r<std::tuple_size<Tuple>::value, Tuple, std::tuple<>> cu;
	public:
	static constexpr bool value=cu::value;
};

}}//end namespace unitlite::impl


#endif /* SRC_LIB_UNIT_LITE_IMPL_CU_MULT_HPP_ */
