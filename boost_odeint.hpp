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

#ifndef LIB_UNIT_LITE_ODEINT_HPP_
#define LIB_UNIT_LITE_ODEINT_HPP_


#include "Quantity.hpp"
#include "limits.hpp"
#include <unit_lite/unit_lite.hpp>


namespace boost{
namespace numeric{
namespace odeint{


	template<class Unit , class T>
	T get_unit_value(const unit_lite::Quantity< Unit , T> &t){
		return t.value;
	}

	template<class Unit , class T, class X>
	void set_unit_value(unit_lite::Quantity< Unit , T>&t , const X &v){
		static_assert(std::is_convertible<X,T>::value,"missing implicit conversion between values");
		t.value=static_cast<T>(v);
	}

}}}

//MUST appears AFTER specializing get_unit_value
#include <boost/numeric/odeint.hpp>





#endif /* LIB_UNIT_LITE_ODEINT_HPP_ */
