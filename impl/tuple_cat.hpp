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



#ifndef SRC_LIB_TUPLE_TOOLS_TUPLE_CAT_HPP_
#define SRC_LIB_TUPLE_TOOLS_TUPLE_CAT_HPP_

namespace unit_lite{
namespace impl{

template<typename Tuple1, typename Tuple2>
struct tuple_cat_tuple;

template<typename... Args1, typename... Args2>
struct tuple_cat_tuple< std::tuple<Args1...>, std::tuple<Args2...> >{
	typedef std::tuple<Args1...> e1;
	typedef std::tuple<Args2...> e2;

	typedef decltype(std::tuple_cat(*static_cast<e1*>(nullptr), *static_cast<e2*>(nullptr) )) type;
};




template<typename Tuple, typename... T>
struct tuple_cat_arg;

template<typename... Args, typename... T>
struct tuple_cat_arg<std::tuple<Args...>,T...>{

	typedef  std::tuple<Args...> e1;
	typedef  std::tuple<T...>    e2;

	typedef typename tuple_cat_tuple<e1,e2>::type type;
};


}}


#endif /* SRC_LIB_TUPLE_TOOLS_TUPLE_CAT_HPP_ */
