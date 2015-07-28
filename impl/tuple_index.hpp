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

#ifndef TUPLE_TOOLS_TUPLE_INDEX_HPP
#define TUPLE_TOOLS_TUPLE_INDEX_HPP


namespace unit_lite{
namespace impl{

//--- crashes if the index is missing ---
template < class Tuple, class T>
struct tuple_index;

template < class... Types, class T>
struct tuple_index<std::tuple<T, Types...>,T> {
    static const std::size_t value = 0;
};

template <class T, class U, class... Types>
struct tuple_index< std::tuple<U, Types...>, T> {
    static const std::size_t value = 1 + tuple_index<std::tuple<Types...>,T>::value;
};


}}
#endif


