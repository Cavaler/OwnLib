#include <stdio.h>

template<int _Length, int _Mass, int _Time, int _Current, int _Light> class Unit {
public:
	Unit() : m_Value(0) {}
	explicit Unit(double _Value) : m_Value(_Value) {}
	Unit(const Unit &_Value) : m_Value(_Value.m_Value) {}

	Unit operator = (const Unit &_Value) {m_Value = _Value.m_Value; return *this; }
	Unit operator + (const Unit &_Value) {return Unit(m_Value + _Value.m_Value); }
	Unit operator - (const Unit &_Value) {return Unit(m_Value - _Value.m_Value); }
	Unit operator * (double _Value) {return Unit(m_Value * _Value); }
	Unit operator / (double _Value) {return Unit(m_Value / _Value); }

	double Value() const {return m_Value;}

protected:
	double m_Value;

/*	Somewhy VC7/8 doesn't digest it, so we have to use Value()

	template<int _Length, int _Mass, int _Time, int _Current, int _Light>
	friend
		Unit<_Length, _Mass, _Time, _Current, _Light> operator * (
			double _Value1,
			const Unit<_Length, _Mass, _Time, _Current, _Light> & _Value2
		);

	template<int _Length, int _Mass, int _Time, int _Current, int _Light>
	friend
		Unit<-_Length, -_Mass, -_Time, -_Current, -_Light> operator / (
			double _Value1,
			const Unit<_Length, _Mass, _Time, _Current, _Light> & _Value2
		);

	template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
			 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>
	friend
		Unit<_Length1+_Length2, _Mass1+_Mass2, _Time1+_Time2, _Current1+_Current2, _Light1+_Light2> operator * (
			const Unit<_Length1, _Mass1, _Time1, _Current1, _Light1> & _Value1,
			const Unit<_Length2, _Mass2, _Time2, _Current2, _Light2> & _Value2
		);

	template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
			 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>
	friend
		Unit<_Length1-_Length2, _Mass1-_Mass2, _Time1-_Time2, _Current1-_Current2, _Light1-_Light2> operator / (
			const Unit<_Length1, _Mass1, _Time1, _Current1, _Light1> & _Value1,
			const Unit<_Length2, _Mass2, _Time2, _Current2, _Light2> & _Value2
		);*/
};

template<int _Length, int _Mass, int _Time, int _Current, int _Light>

Unit<_Length, _Mass, _Time, _Current, _Light> operator * (
	double _Value1,
	const Unit<_Length, _Mass, _Time, _Current, _Light> & _Value2
) {
	return Unit<_Length, _Mass, _Time, _Current, _Light>(_Value1 * _Value2.Value());
}

template<int _Length, int _Mass, int _Time, int _Current, int _Light>

Unit<-_Length, -_Mass, -_Time, -_Current, -_Light> operator / (
	double _Value1,
	const Unit<_Length, _Mass, _Time, _Current, _Light> & _Value2
) {
	return Unit<-_Length, -_Mass, -_Time, -_Current, -_Light>(_Value1 / _Value2.Value());
}

template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
		 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>

Unit<_Length1+_Length2, _Mass1+_Mass2, _Time1+_Time2, _Current1+_Current2, _Light1+_Light2> operator * (
	const Unit<_Length1, _Mass1, _Time1, _Current1, _Light1> & _Value1,
	const Unit<_Length2, _Mass2, _Time2, _Current2, _Light2> & _Value2
) {
	return Unit<_Length1+_Length2, _Mass1+_Mass2, _Time1+_Time2, _Current1+_Current2, _Light1+_Light2>
		(_Value1.Value() * _Value2.Value());
}

template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
		 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>

Unit<_Length1-_Length2, _Mass1-_Mass2, _Time1-_Time2, _Current1-_Current2, _Light1-_Light2> operator / (
	const Unit<_Length1, _Mass1, _Time1, _Current1, _Light1> & _Value1,
	const Unit<_Length2, _Mass2, _Time2, _Current2, _Light2> & _Value2
) {
	return Unit<_Length1-_Length2, _Mass1-_Mass2, _Time1-_Time2, _Current1-_Current2, _Light1-_Light2>
		(_Value1.Value() / _Value2.Value());
}


template<class U1, class U2> class Mul {};
template<class U1, class U2> class Div {};

template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
		 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>

class Mul<
	class Unit<_Length1, _Mass1, _Time1, _Current1, _Light1>,
	class Unit<_Length2, _Mass2, _Time2, _Current2, _Light2>
> {
public:
	typedef Unit<_Length1+_Length2, _Mass1+_Mass2, _Time1+_Time2, _Current1+_Current2, _Light1+_Light2> Type;
};

template<int _Length1, int _Mass1, int _Time1, int _Current1, int _Light1,
		 int _Length2, int _Mass2, int _Time2, int _Current2, int _Light2>

class Div<
	class Unit<_Length1, _Mass1, _Time1, _Current1, _Light1>,
	class Unit<_Length2, _Mass2, _Time2, _Current2, _Light2>
> {
public:
	typedef Unit<_Length1-_Length2, _Mass1-_Mass2, _Time1-_Time2, _Current1-_Current2, _Light1-_Light2> Type;
};

typedef Unit<1, 0, 0, 0, 0> Length;
typedef Unit<2, 0, 0, 0, 0> Square;
typedef Unit<0, 0, 1, 0, 0> Time;
typedef Unit<1, 0, -1, 0, 0> Velocity;

int main(int argc,char *argv[]) {
	Length Len1, Len2(2), Len3 = Len1 + Len2;
	Velocity Spd(Len3 / Time(2));
	Square Sqr(Len1*Len2);

	Div<Length, Time>::Type Spd2(Spd);
//	Div<Length, Time>::Type Spd3(Len3);		// Error

	Div<Div<Length, Time>::Type, Time>::Type Accel;

	return 0;
}
