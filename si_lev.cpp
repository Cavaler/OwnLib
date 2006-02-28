#include "si_lev.h"

typedef SI::Unit<1,0,-1,0,0> Velocity;

int main(int argc, char *argv[])
{
  SI::Time t(1);
  SI::Length l(1);
  Velocity v1(0);
  SI::Div<SI::Length, SI::Time>::Type v2(0);
  
  v1 = l / t;
  v2 = v1;
  //t = t * t;
  return 0;
}
