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


#ifndef SRC_LIB_TUPLE_TOOLS_TUPLE_HAS_TYPE_HPP_
#define SRC_LIB_TUPLE_TOOLS_TUPLE_HAS_TYPE_HPP_

namespace unit_lite{
namespace impl{


	//Check if a type exists in a tuple
	//usage:
	//typedef std::tuple<int,double,double> tuple_t;
	//tuple_has_type<tuple_t,double>::value; //true
	template<class Tuple, class T>
	struct tuple_has_type;


	template< class T>
	struct tuple_has_type<std::tuple<>,T>{
		 static constexpr bool value = false;
		 typedef std::false_type type;
	};

	template <class T, class... Types>
	struct tuple_has_type<std::tuple<T, Types...>,T> {
	    static constexpr bool value = true;
		typedef std::true_type type;

	};

	template <class T, class U, class... Types>
	struct tuple_has_type< std::tuple<U, Types...>,T> {
	    static constexpr bool value = tuple_has_type< std::tuple<Types...>, T>::value;
		typedef typename tuple_has_type< std::tuple<Types...>, T>::type type;
	};


}}

#endif /* SRC_LIB_TUPLE_TOOLS_TUPLE_HAS_TYPE_HPP_ */
