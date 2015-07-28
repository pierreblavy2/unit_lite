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


#ifndef SRC_LIB_TUPLE_TOOLS_TUPLE_TYPEMAP_HPP_
#define SRC_LIB_TUPLE_TOOLS_TUPLE_TYPEMAP_HPP_


#include "tuple_index.hpp"


//Structure that holds a unique version of some types
//this is usefull for compile time indexing of tuples

//usage:
//typedef tuple_typeMap<> empty_map;
//typedef typedef empty_map::add      < double,int,double> map_t2; // add a double and an int to map
//typedef typedef empty_map::add_tuple< std::tuple<int>  > map_t3; // add an int
//typedef map_t2::type t; //t is a std::tuple<double,int>


namespace unit_lite{
namespace impl{
	template<typename previous >
	struct tuple_typeMap;
}}




namespace unit_lite{
namespace impl{


namespace tuple_typeMap_impl{


	//add_tuple
	template<typename IT, typename Tuple>
	struct Next_tuple_t;

	template<typename It, typename X, typename... X2>
	struct Next_tuple_t<tuple_typeMap<It>, std::tuple<X,X2...> >{
		private:
		typedef tuple_typeMap<It> tuple_index_t;
		using xxx = typename tuple_index_t::template add1<X>;

		public:
		typedef typename Next_tuple_t< xxx  ,std::tuple<X2...>>::type type;
	};

	template<typename tuple_index>
	struct Next_tuple_t<tuple_index, std::tuple<> >{
		typedef tuple_index type;
	};


	//Next_args
	template<typename It, typename... A>
	struct Next_args_t{
		private:
		typedef tuple_typeMap<It> tuple_index_t;

		public:
		typedef typename tuple_index_t::template add_tuple< std::tuple<A...> > type;
	};

}



template<>
struct tuple_typeMap<std::tuple<>> {
	typedef std::tuple<> tuple_type;
	typedef tuple_typeMap<tuple_type> this_t;

	template<typename X> struct has_type{static constexpr bool value = false; typedef std::true_type type;};
	template<typename X> struct index{};

	//add
	template<typename    X> using add1=tuple_typeMap<std::tuple<X>>;
	template<typename... X> using add=typename tuple_typeMap_impl::Next_args_t<std::tuple<>,X...>::type;


	template<typename X> using add_tuple=typename tuple_typeMap_impl::Next_tuple_t<this_t , X >::type;

};

template<typename T, typename... A>
struct tuple_typeMap<std::tuple<T,A...>> {
	typedef std::tuple<T,A...> tuple_type;
	typedef tuple_typeMap<tuple_type> this_t;


	template<typename X>
	using has_type=tuple_has_type<std::tuple<T,A...>,X>;

	template<typename X>
	using index=tuple_index<std::tuple<T,A...>,X>;

	private:
	template<typename X, bool b> struct Next_dispatcher{typedef tuple_typeMap<std::tuple<T,A...,X>> type;};
	template<typename X>         struct Next_dispatcher<X,true>{typedef tuple_typeMap<std::tuple<T,A...>> type;};
	public:
	template<typename X> using add1=typename Next_dispatcher<X,tuple_has_type<std::tuple<T,A...>,X>::value >::type;



	template<typename... X> using add=typename tuple_typeMap_impl::Next_args_t<std::tuple<>,X...>::type;
	template<typename X>
	using add_tuple=typename tuple_typeMap_impl::Next_tuple_t<this_t , X >::type;

};


}}//end namespace unit_l_te::impl


#endif /* SRC_LIB_TUPLE_TOOLS_TUPLE_TYPEMAP_HPP_ */
