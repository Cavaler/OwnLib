#ifndef __SI_H__
#define __SI_H__

#ifndef __cplusplus
#error Must be compiled in C++ mode only
#endif

namespace SI {

// Shortcuts
#define SI_TEMPLATE     template<int _L, int _M, int _T, int _I, int _Th, int _J, int _MQ, int _A, int _SA>
#define SI_TEMPALTE_DEF template<int _L, int _M, int _T, int _I, int _Th, int _J = 0, int _MQ = 0, int _A = 0, int _SA = 0>
#define SI_UNIT_TYPE    Unit<_L,_M,_T,_I,_Th,_J,_MQ,_A,_SA>

// Forward declaration of unit class
SI_TEMPALTE_DEF class Unit;

// Forward declaration of operators, to make 'friend' works
SI_TEMPLATE SI_UNIT_TYPE operator*(SI_UNIT_TYPE, double);
SI_TEMPLATE SI_UNIT_TYPE operator*(double, SI_UNIT_TYPE);
SI_TEMPLATE SI_UNIT_TYPE operator/(SI_UNIT_TYPE, double);
SI_TEMPLATE Unit<-_L,-_M,-_T,-_I,-_Th,-_J,-_MQ,-_A,-_SA> operator/(double, SI_UNIT_TYPE);

SI_TEMPLATE SI_UNIT_TYPE operator+(SI_UNIT_TYPE, SI_UNIT_TYPE);
SI_TEMPLATE SI_UNIT_TYPE operator-(SI_UNIT_TYPE, SI_UNIT_TYPE);

SI_TEMPLATE class Unit {
private:
  double _v;
  
  Unit();
public:
  explicit Unit(double v) : _v(v) {};
  double v() const { return _v; };
  
  typedef Unit<_L,_M,_T,_I,_Th,_J,_MQ,_A,_SA>                   Type;
  typedef Unit<-_L,-_M,-_T,-_I,-_Th,-_J,-_MQ,-_A,-_SA>          Reciprocal;
  typedef Unit<_L*2,_M*2,_T*2,_I*2,_Th*2,_J*2,_MQ*2,_A*2,_SA*2> Squared;
  typedef Unit<_L*3,_M*3,_T*3,_I*3,_Th*3,_J*3,_MQ*3,_A*3,_SA*3> Cubed;
  
  enum { L_pow  = _L };
  enum { M_pow  = _M };
  enum { T_pow  = _T };
  enum { I_pow  = _I };
  enum { Th_pow = _Th };
  enum { J_pow  = _J };
  enum { MQ_pow = _MQ };
  enum { A_pow  = _A };
  enum { SA_pow = _SA };
  
  friend Type operator*<>(Type, double);
  friend Type operator*<>(double, Type);
  friend Type operator/<>(Type, double);
  friend Reciprocal operator/<>(double, Type);

  friend Type operator+<>(Type, Type);
  friend Type operator-<>(Type, Type);
};

SI_TEMPLATE SI_UNIT_TYPE operator*(SI_UNIT_TYPE ls, double rs) { return SI_UNIT_TYPE(ls._v * rs); }
SI_TEMPLATE SI_UNIT_TYPE operator*(double ls, SI_UNIT_TYPE rs) { return SI_UNIT_TYPE(ls * rs._v); }
SI_TEMPLATE SI_UNIT_TYPE operator/(SI_UNIT_TYPE ls, double rs) { return SI_UNIT_TYPE(ls._v / rs); }
SI_TEMPLATE Unit<-_L,-_M,-_T,-_I,-_Th,-_J,-_MQ,-_A,-_SA> operator/(double ls, SI_UNIT_TYPE rs) { return SI_UNIT_TYPE::Reciprocal(ls / rs._v); }

SI_TEMPLATE SI_UNIT_TYPE operator+(SI_UNIT_TYPE ls, SI_UNIT_TYPE rs) { return SI_UNIT_TYPE(ls._v + rs._v); }
SI_TEMPLATE SI_UNIT_TYPE operator-(SI_UNIT_TYPE ls, SI_UNIT_TYPE rs) { return SI_UNIT_TYPE(ls._v - rs._v); }

// Can not write this one as friends, I'm looser :(
template<int _L1, int _M1, int _T1, int _I1, int _Th1, int _J1, int _MQ1, int _A1, int _SA1,
         int _L2, int _M2, int _T2, int _I2, int _Th2, int _J2, int _MQ2, int _A2, int _SA2>
Unit<_L1+_L2,_M1+_M2,_T1+_T2,_I1+_I2,_Th1+_Th2,_J1+_J2,_MQ1+_MQ2,_A1+_A2,_SA1+_SA2>
operator*(Unit<_L1,_M1,_T1,_I1,_Th1,_J1,_MQ1,_A1,_SA1> ls, Unit<_L2,_M2,_T2,_I2,_Th2,_J2,_MQ2,_A2,_SA2> rs)
{
  return Unit<_L1+_L2,_M1+_M2,_T1+_T2,_I1+_I2,_Th1+_Th2,_J1+_J2,_MQ1+_MQ2,_A1+_A2,_SA1+_SA2>(ls.v() * rs.v());
}

// Can not write this one as friends, I'm looser :(
template<int _L1, int _M1, int _T1, int _I1, int _Th1, int _J1, int _MQ1, int _A1, int _SA1,
         int _L2, int _M2, int _T2, int _I2, int _Th2, int _J2, int _MQ2, int _A2, int _SA2>
Unit<_L1-_L2,_M1-_M2,_T1-_T2,_I1-_I2,_Th1-_Th2,_J1-_J2,_MQ1-_MQ2,_A1-_A2,_SA1-_SA2>
operator/(Unit<_L1,_M1,_T1,_I1,_Th1,_J1,_MQ1,_A1,_SA1> ls, Unit<_L2,_M2,_T2,_I2,_Th2,_J2,_MQ2,_A2,_SA2> rs)
{
  return Unit<_L1-_L2,_M1-_M2,_T1-_T2,_I1-_I2,_Th1-_Th2,_J1-_J2,_MQ1-_MQ2,_A1-_A2,_SA1-_SA2>(ls.v() / rs.v());
}

typedef Unit<0,0,0,0,0,0,0,0,0> Dimless;
typedef Unit<1,0,0,0,0,0,0,0,0> Length;
typedef Unit<0,1,0,0,0,0,0,0,0> Mass;
typedef Unit<0,0,1,0,0,0,0,0,0> Time;
typedef Unit<0,0,0,1,0,0,0,0,0> Current;
typedef Unit<0,0,0,0,1,0,0,0,0> Temperature;
typedef Unit<0,0,0,0,0,1,0,0,0> LightIntensity;
typedef Unit<0,0,0,0,0,0,1,0,0> MatterQuantity;
typedef Unit<0,0,0,0,0,0,0,1,0> Angle;
typedef Unit<0,0,0,0,0,0,0,0,1> SolidAngle;

template <class _0, class _1,
          class _2 = Dimless, class _3 = Dimless, class _4 = Dimless,
          class _5 = Dimless, class _6 = Dimless, class _7 = Dimless,
          class _8 = Dimless, class _9 = Dimless>
class Mul {
public:
  enum { L_pow  = _0::L_pow  + _1::L_pow  + _2::L_pow  + _3::L_pow  + _4::L_pow  + _5::L_pow  + _6::L_pow  + _7::L_pow  + _8::L_pow  + _9::L_pow  };
  enum { M_pow  = _0::M_pow  + _1::M_pow  + _2::M_pow  + _3::M_pow  + _4::M_pow  + _5::M_pow  + _6::M_pow  + _7::M_pow  + _8::M_pow  + _9::M_pow  };
  enum { T_pow  = _0::T_pow  + _1::T_pow  + _2::T_pow  + _3::T_pow  + _4::T_pow  + _5::T_pow  + _6::T_pow  + _7::T_pow  + _8::T_pow  + _9::T_pow  };
  enum { I_pow  = _0::I_pow  + _1::I_pow  + _2::I_pow  + _3::I_pow  + _4::I_pow  + _5::I_pow  + _6::I_pow  + _7::I_pow  + _8::I_pow  + _9::I_pow  };
  enum { Th_pow = _0::Th_pow + _1::Th_pow + _2::Th_pow + _3::Th_pow + _4::Th_pow + _5::Th_pow + _6::Th_pow + _7::Th_pow + _8::Th_pow + _9::Th_pow };
  enum { J_pow  = _0::J_pow  + _1::J_pow  + _2::J_pow  + _3::J_pow  + _4::J_pow  + _5::J_pow  + _6::J_pow  + _7::J_pow  + _8::J_pow  + _9::J_pow  };
  enum { MQ_pow = _0::MQ_pow + _1::MQ_pow + _2::MQ_pow + _3::MQ_pow + _4::MQ_pow + _5::MQ_pow + _6::MQ_pow + _7::MQ_pow + _8::MQ_pow + _9::MQ_pow };
  enum { A_pow  = _0::A_pow  + _1::A_pow  + _2::A_pow  + _3::A_pow  + _4::A_pow  + _5::A_pow  + _6::A_pow  + _7::A_pow  + _8::A_pow  + _9::A_pow  };
  enum { SA_pow = _0::SA_pow + _1::SA_pow + _2::SA_pow + _3::SA_pow + _4::SA_pow + _5::SA_pow + _6::SA_pow + _7::SA_pow + _8::SA_pow + _9::SA_pow };

  typedef Unit<L_pow,M_pow,T_pow,I_pow,Th_pow,J_pow,MQ_pow,A_pow,SA_pow>                   Type;
  typedef Unit<-L_pow,-M_pow,-T_pow,-I_pow,-Th_pow,-J_pow,-MQ_pow,-A_pow,-SA_pow>          Reciprocal;
  typedef Unit<L_pow*2,M_pow*2,T_pow*2,I_pow*2,Th_pow*2,J_pow*2,MQ_pow*2,A_pow*2,SA_pow*2> Squared;
  typedef Unit<L_pow*3,M_pow*3,T_pow*3,I_pow*3,Th_pow*3,J_pow*3,MQ_pow*3,A_pow*3,SA_pow*3> Cubed;
};

template <class _0, class _1>
class Div {
public:
public:
  enum { L_pow  = _0::L_pow  - _1::L_pow  };
  enum { M_pow  = _0::M_pow  - _1::M_pow  };
  enum { T_pow  = _0::T_pow  - _1::T_pow  };
  enum { I_pow  = _0::I_pow  - _1::I_pow  };
  enum { Th_pow = _0::Th_pow - _1::Th_pow };
  enum { J_pow  = _0::J_pow  - _1::J_pow  };
  enum { MQ_pow = _0::MQ_pow - _1::MQ_pow };
  enum { A_pow  = _0::A_pow  - _1::A_pow  };
  enum { SA_pow = _0::SA_pow - _1::SA_pow };

  typedef Unit<L_pow,M_pow,T_pow,I_pow,Th_pow,J_pow,MQ_pow,A_pow,SA_pow>                   Type;
  typedef Unit<-L_pow,-M_pow,-T_pow,-I_pow,-Th_pow,-J_pow,-MQ_pow,-A_pow,-SA_pow>          Reciprocal;
  typedef Unit<L_pow*2,M_pow*2,T_pow*2,I_pow*2,Th_pow*2,J_pow*2,MQ_pow*2,A_pow*2,SA_pow*2> Squared;
  typedef Unit<L_pow*3,M_pow*3,T_pow*3,I_pow*3,Th_pow*3,J_pow*3,MQ_pow*3,A_pow*3,SA_pow*3> Cubed;
};

};

#endif//__SI_H__
