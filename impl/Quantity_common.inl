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


//This is the common code ov various specifications of Quantity
//Note that some operators are members (and not free function), because for some
//wired reason, free functions operators don't work properly in boost::odeint







Quantity()=default;

value_t value;
static Quantity<unit_t,value_t> from_value(const value_t&v){return this_t(v);}

private:
Quantity(const V&v):value(v){};
public:





template<typename X, typename Y>
Quantity(const Quantity<X,Y> &q){
	typedef typename Quantity<X,Y>::unit_t unit_q;
	static_assert(same_unit<unit_t, unit_q >::value,"cannot assign heterogeneous units" );
	value=q.value;
}

template<typename X, typename Y>
this_t & operator=(const Quantity<X,Y> &q){
	typedef typename Quantity<X,Y>::unit_t unit_q;
	static_assert(same_unit<unit_t, unit_q >::value,"cannot assign heterogeneous units" );
	value=q.value;
	return *this;
}



template<typename U2, typename V2>
bool operator <(const unit_lite::Quantity<U2,V2>& b)const{
	static_assert(unit_lite::same_unit<unit_t,U2>::value,"cannot compare (<) heterogeneous units");
	return value < b.value;
}

template<typename U2, typename V2>
bool operator >(const unit_lite::Quantity<U2,V2>& b)const{
	static_assert(unit_lite::same_unit<unit_t,U2>::value,"cannot compare (>) heterogeneous units");
	return value > b.value;
}

template<typename U2, typename V2>
bool operator ==(const unit_lite::Quantity<U2,V2>& b)const{
	static_assert(unit_lite::same_unit<unit_t,U2>::value,"cannot compare (==) heterogeneous units");
	return value == b.value;
}




template<typename U2, typename V2>
bool operator <=(const unit_lite::Quantity<U2,V2>& b)const{
	static_assert(unit_lite::same_unit<unit_t,U2>::value,"cannot compare (<=) heterogeneous units");
	return value <= b.value;
}



template<typename U2, typename V2>
bool operator >=(const unit_lite::Quantity<U2,V2>& b)const{
	static_assert(unit_lite::same_unit<unit_t,U2>::value,"cannot compare (>=) heterogeneous units");
	return value >= b.value;
}
