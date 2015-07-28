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


#ifndef SRC_LIB_UNIT_LITE_IMPL_CU_ADD_HPP_
#define SRC_LIB_UNIT_LITE_IMPL_CU_ADD_HPP_


namespace unit_lite{
namespace impl{
	//static asserts that two units can be added (or substracted)
	//return true if it's ok
	template<typename A, typename B> struct Compose_unit_add;
}}



namespace unit_lite{
namespace impl{

	//same type but different kinds
	template<
	    typename A_tag,typename A_ratio,
	    typename B_tag,typename B_ratio
	>
	struct Compose_unit_add
	<
	  Compose_unit<A_tag,A_ratio>,
	  Compose_unit<B_tag,B_ratio>
	>{

		//typedef Compose_unit<A_tag,A_ratio> type;

		private: //check compatibility
		typedef Compose_unit<A_tag,A_ratio> A_type;
		typedef Compose_unit<B_tag,B_ratio> B_type;

		//same size
		static constexpr size_t size=A_type::size;


		//For each units in A
		template<size_t I>
		struct Check_one{
			typedef typename A_type::template tag<I> tag_i;

			//dispatch between found or not found
			template<bool b, typename T=void>
			struct Dispatch{//not found
				static constexpr bool run(){return false;}
			};

			template<typename T>
			struct Dispatch<true,T>{//not found
				static constexpr bool run(){
					constexpr size_t b_index = tuple_index<typename B_type::Tag_t,tag_i>::value;
					typedef typename A_type::template ratio<I      > a_ratio_i;
					typedef typename B_type::template ratio<b_index> b_ratio_i;

					constexpr bool has_type_b = tuple_has_type<typename B_type::Tag_t, tag_i>::value;
					constexpr bool is_same_b = std::is_same<a_ratio_i,b_ratio_i>::value;
					return has_type_b and is_same_b;
				}
			};

			static constexpr bool run(){
				constexpr bool tag_b_exists = tuple_has_type<typename B_type::Tag_t, tag_i>::value;
				return Dispatch<tag_b_exists>::run();
			}
		};



		//Recursive apply of check_one
		template<size_t I, typename T=void>
		struct Check_r{
			static constexpr bool run(){
				constexpr bool b1 = Check_r<I-1,T>::run();
				constexpr bool b2 = Check_one<I-1>::run();
				return b1 and b2;
			}
		};

		template<typename T>
		struct Check_r<0,T>{static constexpr bool run(){return true;}; };


		public:
		static constexpr bool run(){
			constexpr bool b1 = A_type::size == B_type::size;
			constexpr bool b2 = Check_r<size,void>::run();
			return b1 and b2;
			//static_assert(A_type::size == B_type::size,"Cannot add(substract) heterogeneous units : different number of dimensions");
			//Check_r<size,void>::run();

		}
	};


	//same type : can always add -> no check
	template<typename A_tag,typename A_ratio>
	struct Compose_unit_add
	<
	  Compose_unit<A_tag,A_ratio>,
	  Compose_unit<A_tag,A_ratio>
	>{
		//typedef Compose_unit<A_tag,A_ratio> type;
		static constexpr bool run(){return true;}
	};

}}//end namesapce unit_lite::impl

#endif /* SRC_LIB_UNIT_LITE_IMPL_CU_ADD_HPP_ */
