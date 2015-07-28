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


#ifndef SRC_LIB_UNIT_LITE_QUANTITY_HPP_
#define SRC_LIB_UNIT_LITE_QUANTITY_HPP_



#include "same_unit.hpp"
#include "Compose_unit.hpp"

#include "impl/tuple_cat.hpp"

#include <ostream>

namespace unit_lite{
	//A Quantity holds a value
	//A Quantity holds a typedef on the unit type of value
    //    this typedef is a kind of Compose_unit
	template<typename Unit_t,typename Value_t>
	struct Quantity;

}


namespace unit_lite{
  //user may specialize to multiply his value type by a std::ratio
  template< typename Ratio, typename V> struct multiply_by_ratio_t;

  template<typename V, std::intmax_t N, std::intmax_t D>
  struct multiply_by_ratio_t<  std::ratio<N,D>,V  >{
	 typedef typename std::ratio<N,D>::type ratio_t;
	 static V run(const V& source){
		 return (source * static_cast<V>(ratio_t::num))/static_cast<V>(ratio_t::den);
	 }
  };

  template<typename R,typename V>
  inline V multiply_by_ratio(const V&v){return multiply_by_ratio_t<R,V>::run(v);}

}











namespace unit_lite{
namespace impl{


struct XXX_DEBUG{}; //todo remove me!!!

template<typename Ratio>
struct ratio_is_natural{
	static constexpr bool value=Ratio::type::den == 1;
};



//Ratio pow
template<typename Ratio_p, size_t I,bool positive>
struct Ratio_pow_r{
	typedef typename std::ratio_multiply<
			typename Ratio_pow_r<Ratio_p,I-1,true>::type,
			Ratio_p>::type type;
};
template<typename Ratio>
struct Ratio_pow_r<Ratio,0,true>{
	typedef std::ratio<1,1> type;
};
template<typename Ratio>
struct Ratio_pow_r<Ratio,0,false>{
	typedef std::ratio<1,1> type;
};

template<typename Ratio_p, size_t I>
struct Ratio_pow_r<Ratio_p,I,false>{
	typedef typename std::ratio_divide<
			typename Ratio_pow_r<Ratio_p,I-1,false>::type,
			Ratio_p
	>::type type;
};

template<int I, bool> struct abs_r         {static constexpr int value=I;};
template<int I>       struct abs_r<I,false>{static constexpr int value= -I;};
template<int I>       struct abs           {static constexpr int value= abs_r<I, (I>0) >::value;};

template<typename Ratio, int I>
struct ratio_pow{
	typedef typename Ratio_pow_r<Ratio,abs<I>::value, (I>0) >::type type;
};





template<size_t I, typename T> struct multiple_base_one;
template<size_t I, typename T, typename R >
struct multiple_base_one<I, Compose_unit<T,R> >{

	typedef Compose_unit<T,R> CU;
	static_assert( I<CU::size,"logical error : bad index in multiple_base_one ");

	typedef typename CU::template tag<I> tag;

	typedef typename tag::base_tag base_tag;
	typedef typename tag::factor base_factor;
	typedef typename CU::template ratio<I> cu_ratio;

	//factor = base_factor^cu_ratio;
	//e.g. mm^2, factor = (1/1000)^2
	static_assert(ratio_is_natural<cu_ratio>::value,"cannot convert a unit with a non natural (i.e. x/1) power (e.g., m^2 is OK but m^(1/2) is not ).");
	static constexpr std::intmax_t p = cu_ratio::type::num;
	typedef typename ratio_pow<base_factor,p>::type factor;
};


	template<size_t I, typename CU>
	struct multiple_base_r;

	template<size_t I, typename T, typename R>
	struct multiple_base_r< I, Compose_unit<T,R> >{
		typedef Compose_unit<T,R> CU;
		typedef multiple_base_one<I-1,CU> MBO;

		//factor is the product of factors
		typedef typename std::ratio_multiply<
			typename multiple_base_r<I-1,CU>::factor,
			typename MBO::factor
		>::type factor;

		//unit tag the concatenation of base type
		typedef typename impl::tuple_cat_arg<
			typename multiple_base_r<I-1,CU>::Tag_t,
			typename MBO::base_tag
		>::type Tag_t;

		//Value and ratio doesn't change
		typedef Compose_unit<Tag_t,R> unit_t;
	};

	template<typename T, typename R>
	struct multiple_base_r< 0, Compose_unit<T,R> >{
		typedef std::ratio<1,1> factor;
		typedef std::tuple<> Tag_t;
	};

	template<>
	struct multiple_base_r< 0, Compose_unit<std::tuple<>,std::tuple<>> >{
			typedef std::ratio<1,1> factor;
			typedef std::tuple<> Tag_t;
			typedef Compose_unit<std::tuple<>,std::tuple<>> unit_t;
		};

	template<typename T> struct multiple_base;
	template<typename T, typename R>
	class multiple_base<Compose_unit<T,R> >{
		typedef Compose_unit<T,R> CU;
		typedef multiple_base_r<CU::size,CU> MBR;
		public:
		typedef typename MBR::factor factor;
		typedef typename MBR::unit_t unit_t;
	};




}
}




namespace unit_lite{

	template<typename T, typename R, typename V>
	struct Quantity<Compose_unit<T,R>,V>{
		typedef Quantity<Compose_unit<T,R>,V> this_t;
		typedef Compose_unit<T,R> unit_t;
		typedef V value_t;

		#include <unit_lite/impl/Quantity_common.inl>

		static constexpr bool is_dimensionless=false;


		//Convert to an other quantity
		//This conversion require a dimension
		template<typename T2,typename R2, typename V2>
		void to(Quantity<Compose_unit<T2,R2>,V2> &write_here)const{

			typedef unit_t              unit_t1;
			typedef Compose_unit<T2,R2> unit_t2;

			//No conversion between dimensionless
			typedef Compose_unit<std::tuple<>,std::tuple<>> dimensionless;
			static_assert(
				! unit_lite::same_unit<dimensionless,unit_t1>::value,
				"cannot convert (x.to) because the source class is dimensionless."
			);

			typedef Compose_unit<std::tuple<>,std::tuple<>> dimensionless;
			static_assert(
				! unit_lite::same_unit<dimensionless,unit_t2>::value,
				"cannot convert (x.to) because the target class is dimensionless."
			);






			//[1] We want to convert mm/min into km/s
			//    First we convert to base with multiple_base
			//    mm/min    -->  r1=(1/1000)^1  * (60/1)^-1  , m/s
			//    km/s      -->  r2=(1000/1)^1  * (1/1 )^-1  , m/s
			typedef typename impl::multiple_base<unit_t1> UR1;
			typedef typename impl::multiple_base<unit_t2> UR2;

			//[2] Then we assert that multiple_base have the same units (m/s)
			static_assert(
					unit_lite::same_unit<
					typename UR1::unit_t,
					typename UR2::unit_t>::value,
					"cannot convert (.to) between heterogeneous units."
			);

			//[3] Then we use their ratio to generate the conversion
			//new_value = old_value * (r2/r1)
			typedef typename std::ratio_divide<
					typename UR2::factor ,
					typename UR1::factor
			>::type factor;

			write_here.value = unit_lite::multiply_by_ratio<factor>(this->value);
		}

		template<typename AA>
		AA to()const{	AA r; this->to(r);return r;	}

	};


	//dimensionless idem + auto cast to V
	template<typename V>
	struct Quantity<Compose_unit<std::tuple<>,std::tuple<>>,V>{
		typedef Quantity<Compose_unit<std::tuple<>,std::tuple<>>,V> this_t;
		typedef Compose_unit<std::tuple<>,std::tuple<> > unit_t;
		typedef V value_t;

		#include <unit_lite/impl/Quantity_common.inl>

		operator V&()           {return value;}
		operator const V&()const{return value;}
		static constexpr bool is_dimensionless=true;


		private:
		template<typename... A>
		struct false_t{static constexpr bool value=false;};

		public:
		//Cannot convert between dimensionless units
		template<typename T2,typename R2, typename V2>
		void to(Quantity<Compose_unit<T2,R2>,V2> &write_here)const{
			static_assert(
				false_t<decltype(write_here)>::value,
				"cannot convert (source.to) because the source class is dimensionless."
			);
		}
		template<typename AA>
		AA to()const{	AA r; this->to(r);return r;	}

		/*
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
		}*/



	};





	template<typename V>
	using Quantity_dimensionless=Quantity<Compose_unit<std::tuple<>,std::tuple<>>,V>;


}//end namespace unit_lite



namespace unit_lite{
template <typename T> struct print_compose_unit;
}

template<typename T, typename U>
std::ostream & operator<<(std::ostream & out, const unit_lite::Quantity<T,U> &q){
	typedef unit_lite::Quantity<T,U> quantity_t;
	out << q.value <<" "
		<< unit_lite::print_compose_unit<typename quantity_t::unit_t>::str();
	return out;
}




#endif /* SRC_LIB_UNIT_LITE_QUANTITY_HPP_ */
