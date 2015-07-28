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



#ifndef SRC_LIB_UNIT_LITE_IMPL_CU_RMNULLRATIO_HPP_
#define SRC_LIB_UNIT_LITE_IMPL_CU_RMNULLRATIO_HPP_

#include "tuple_cat.hpp"

namespace unit_lite{
	namespace impl{
		template<typename T>
		struct Compose_unit_rmNullRatio;
	}
}






namespace unit_lite{
	namespace impl{

	template<typename T, typename R>
	struct Compose_unit_rmNullRatio< Compose_unit<T,R> >{

		typedef Compose_unit<T,R> Compose_unit_t;


		template<bool B,  typename True, typename False>
		struct Type_dispatch{typedef False type;};

		template<typename True, typename False>
		struct Type_dispatch<true,True,False>{typedef True type;};


		template<size_t I, typename previous>
		struct Filter_t{
			//recursive type
			typedef typename Filter_t<I-1,previous>::type prev_t;
			typedef typename prev_t::Tag_t   prev_tag;
			typedef typename prev_t::Ratio_t prev_ratio;

			typedef typename std::tuple_element<I-1,R>::type ratio_i;  //I-1 th ratio type
			typedef typename std::tuple_element<I-1,T>::type tag_i;    //I-1 th tag   type
			//typedef typename std::tuple_element<I-1,V>::type value_i;  //I-1 th value type

			static constexpr bool keep_me = ratio_i::num != 0;
			typedef typename ::unit_lite::impl::tuple_cat_arg<prev_tag  ,tag_i  >::type keep_tag;
			typedef typename ::unit_lite::impl::tuple_cat_arg<prev_ratio,ratio_i>::type keep_ratio;

			//keep_unit
			typedef Compose_unit<keep_tag,keep_ratio> keep_unit;

			typedef typename Type_dispatch<
					keep_me,   //if keep_me
					keep_unit, //use keep_unit that append data
					prev_t     //use previous_t : no append
					>::type type;
		};


		template<typename previous>
		struct Filter_t<0,previous>{typedef previous type;};

		typedef Compose_unit<std::tuple<>, std::tuple<> > Compose_unit_empty;
		typedef typename Filter_t<std::tuple_size<R>::value, Compose_unit_empty >::type type;


		static_assert(std::tuple_size<R>::value == std::tuple_size<T>::value,"size missmatch");

	};


	}
}


#endif /* SRC_LIB_UNIT_LITE_IMPL_CU_RMNULLRATIO_HPP_ */
