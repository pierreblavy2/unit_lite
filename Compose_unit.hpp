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


#ifndef SRC_LIB_UNIT_LITE_COMPOSE_UNIT_HPP_
#define SRC_LIB_UNIT_LITE_COMPOSE_UNIT_HPP_


#include <tuple>
#include <ratio>

#include "same_unit.hpp"

namespace unit_lite{
	template<typename Tags, typename Ratios>
	struct Compose_unit;
}


//--implementation --
#include "impl/tuple_has_type.hpp"
#include "impl/tuple_index.hpp"



namespace unit_lite{
	template<
	  typename... Tags,
	  typename... Ratios
	 >
	struct Compose_unit<
		std::tuple<Tags...>  ,
		std::tuple<Ratios...>
		>
	{
	public:
		Compose_unit()=delete; //compile time class only (use in typedef OK, but don't create instances)


		typedef std::tuple<Tags  ...> Tag_t;
		typedef std::tuple<Ratios...> Ratio_t;

		template<size_t I> using tag   = typename std::tuple_element<I,Tag_t>::type;
		template<size_t I> using ratio = typename std::tuple_element<I,Ratio_t>::type::type; //double type as ratio::type returns the reduced type

		template<typename T> struct  tag_exists{static constexpr bool   value = impl::tuple_has_type<Tag_t,T>::value; };
		template<typename T> struct  tag_index {static constexpr size_t value = impl::tuple_index<Tag_t,T>::value;};


		//size
		static constexpr size_t size = std::tuple_size<Tag_t>::value;
		static_assert(std::tuple_size<Tag_t>::value == std::tuple_size<Ratio_t>::value, "size missmatch");


		//return a ratio if tag exists,
		//return std::ratio<0,1> if not exists
		template<typename tag>
		struct ratio_from_tag{
			static constexpr bool ratio_exists = tag_exists<tag>::value;

			template<bool b, typename XX=void>
			struct Dispatch{//false : 0
				typedef std::ratio<0,1> type;
			};

			template<typename XX>
			struct Dispatch<true,XX>{//true extract : 0
				static constexpr size_t index = tag_index<tag>::value;
				typedef ratio<index> type;
			};

			typedef typename Dispatch<ratio_exists>::type type;
		};


	};







}//end namespace unit_lite







//print stuff
#include <string>
namespace unit_lite{
	template <typename> struct print_compose_unit;
	template <typename> struct print_tag;


	template<typename T, typename R>
	struct print_compose_unit< Compose_unit<T,R> >{

	public :
		typedef std::ratio<1,1> one;
		typedef std::ratio<0,1> zero;
		typedef Compose_unit<T,R> Compose_unit_t;
		typedef typename Compose_unit_t::Ratio_t Ratio_t;
		typedef typename Compose_unit_t::Tag_t   Tag_t;


		template<typename Tag, typename Ratio>
		struct print_item{
			static std::string run(){
				//reduce ratio
				typedef typename Ratio::type rat;

				static_assert(!std::ratio_equal<zero,rat>::value,"unit_lite library error : null ratio");
				if(std::ratio_equal<one,rat>::value){
					//return ::unit_lite::print_tag<Tag>::str();
					return Tag::str();
				}else{
					std::string r = Tag::str() + "^"
							      + std::to_string(rat::num);

					if(rat::den!=1){
						r+="/"+ std::to_string(rat::den);
					}

					return r;
				}
			}
		};

		template<typename Tag, typename Ratio>
		struct print_item_pos{
			static std::string run(){
				if( std::ratio_less<Ratio,zero>::value ){return "";}
				return print_item<Tag,Ratio>::run();
			}
		};


		template<typename Tag, typename Ratio>
		struct print_item_neg{
			static std::string run(){
				if(! std::ratio_less<Ratio,zero>::value ){return "";}
				return print_item<Tag,Ratio>::run();
			}
		};

		static std::string merge(const std::string &a, const std::string &b){
			if(a==""){return b;}
			if(b==""){return a;}
			return a+"."+b;
		}

		template<size_t I, template<typename,typename> class Fn>
		struct print_r{
			static std::string run(){
			typedef typename std::tuple_element<I-1,Tag_t  >::type tag_t;
			typedef typename std::tuple_element<I-1,Ratio_t>::type ratio_t;

			static_assert(!std::ratio_equal<zero,ratio_t>::value,"unit_lite library error : null ratio");


			typedef Fn<tag_t,ratio_t> Fn_t;
				return merge(  print_r<I-1,Fn>::run() , Fn_t::run()  );
			}
		};

		template<template<typename,typename> class Fn>
		struct print_r<0, Fn >{
			static std::string run(){return "";}
		};


		public:
		static std::string str(){
			return merge( print_r<Compose_unit_t::size,print_item_pos>::run(),
				          print_r<Compose_unit_t::size,print_item_neg>::run());

		}


	};


}//end namespace unit_lite












#endif /* SRC_LIB_UNIT_LITE_COMPOSE_UNIT_HPP_ */
