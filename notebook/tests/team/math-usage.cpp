// Literal Usage snippets from all 34 active math/numerical/number-theory/combinatorial headers.
// Captured after interface consolidation; each runs in its own function.
#include "prelude.hpp"
#include "../../content/math/IntDivision.h"
void use_IntDivision(){
 assert(ceilDiv(17LL,5LL) == 4);
 assert(floorDiv(-7LL,3LL) == -3);
}
#include "../../content/math/ModAlign.h"
void use_ModAlign(){
 assert(nextTerm(17LL,3LL,5LL) == 18);
}
#include "../../content/math/Int128.h"
void use_Int128(){
 i128 x = i128(1000000000000LL)*1000000000000LL;
 cout << toString(x);
 // Postconditions validating the literal example above.
 assert(toString(x) == "1000000000000000000000000");
}
#include "../../content/math/Probability.h"
void use_Probability(){
 auto prob = binomialProb(4,0.5L); // prob[2]=0.375
 assert(abs(atLeastOne(4,0.5L)-0.9375L) < 1e-15L);
 // Postconditions validating the literal example above.
 assert((prob == vec<long double>{0.0625L,0.25L,0.375L,0.25L,0.0625L}));
}
#include "../../content/numerical/BinarySearch.h"
void use_BinarySearch(){
 auto x = firstTrue(0,11,[](ll t) { return t/2+t/3 >= 5; });
 assert(x && *x == 6);
}
#include "../../content/numerical/Bisection.h"
void use_Bisection(){
 double x = bisect(0.,2.,[](double x) { return x*x >= 2; });
 // Postconditions validating the literal example above.
 assert(x*x >= 2 && abs(x-sqrt(2.)) < 1e-12);
}
#include "../../content/numerical/TernarySearch.h"
void use_TernarySearch(){
 auto x = ternaryMin(0,10,[](long double x) { return (x-3)*(x-3); });
 // Postconditions validating the literal example above.
 assert(abs(x-3) < 1e-12L);
}
#include "../../content/numerical/ModPower.h"
void use_ModPower(){
 assert(modPow(2LL,10,7LL) == 2);
}
#include "../../content/numerical/Matrix.h"
void use_Matrix(){
 vec<vec<ll>> a{{1,1},{1,0}};
 auto f = matPow(a,10,1000000007LL); // f[0][1]=55
 // Postconditions validating the literal example above.
 assert((f == vec<vec<ll>>{{89,55},{55,34}}));
}
#include "../../content/numerical/Gauss.h"
void use_Gauss(){
 vec<vec<double>> a{{2,1,5},{1,-1,1}}; vec<double> x;
 int rank = gauss(a,2,x); // rank=2, x={2,1}
 // Postconditions validating the literal example above.
 assert(rank == 2 && sz(x) == 2);
 assert(abs(x[0]-2) < 1e-12 && abs(x[1]-1) < 1e-12);
}
#include "../../content/numerical/GaussMod.h"
void use_GaussMod(){
 vec<vec<int>> a{{2,1,5},{1,-1,1}}; vec<int> x;
 int rank = gaussMod(a,2,x,7); // rank=2, x={2,1}
 // Postconditions validating the literal example above.
 assert(rank == 2 && (x == vec<int>{2,1}));
}
#include "../../content/numerical/GaussBinary.h"
void use_GaussBinary(){
 vec<bitset<4>> a{bitset<4>(5),bitset<4>(6)}; vec<int> x;
 int rank = gaussBinary(a,2,x); // x={1,1}
 // Postconditions validating the literal example above.
 assert(rank == 2 && (x == vec<int>{1,1}));
}
#include "../../content/numerical/Determinant.h"
void use_Determinant(){
 assert(abs(determinant({{2,1},{1,3}})-5) < 1e-9L);
}
#include "../../content/numerical/IntDeterminant.h"
void use_IntDeterminant(){
 assert(detMod({{2,1},{1,3}},6) == 5);
}
#include "../../content/numerical/MatrixInverse.h"
void use_MatrixInverse(){
 auto inv = matrixInverse({{2,1},{1,3}}); // test inv before *inv
 // Postconditions validating the literal example above.
 assert(inv && sz(*inv) == 2 && sz((*inv)[0]) == 2 && sz((*inv)[1]) == 2);
 assert(abs((*inv)[0][0]-0.6L) < 1e-12L);
 assert(abs((*inv)[0][1]+0.2L) < 1e-12L);
 assert(abs((*inv)[1][0]+0.2L) < 1e-12L);
 assert(abs((*inv)[1][1]-0.4L) < 1e-12L);
}
#include "../../content/numerical/PolyInterpolate.h"
void use_PolyInterpolate(){
 auto a = interpolate({0,1,2},{1,4,9}); // {1,2,1}
 // Postconditions validating the literal example above.
 assert(sz(a) == 3);
 assert(abs(a[0]-1) < 1e-12L && abs(a[1]-2) < 1e-12L && abs(a[2]-1) < 1e-12L);
}
#include "../../content/numerical/LinearRecurrence.h"
void use_LinearRecurrence(){
 assert(linearRec({0,1},{1,1},10,1000000007) == 55);
}
#include "../../content/numerical/NTT.h"
void use_NTT(){
 auto c = NTT::conv(vec<int>{1,2},vec<int>{3,4});
 // c = {3,10,8}
 // Postconditions validating the literal example above.
 assert((c == vec<int>{3,10,8}));
}
#include "../../content/numerical/FastFourierTransform.h"
void use_FastFourierTransform(){
 auto c = FFT::conv({1,2},{3,4}); // approximately {3,10,8}
 // Postconditions validating the literal example above.
 assert(sz(c) == 3);
 assert(abs(c[0]-3) < 1e-12L && abs(c[1]-10) < 1e-12L && abs(c[2]-8) < 1e-12L);
}
#include "../../content/number-theory/ExtGCD.h"
void use_ExtGCD(){
 ll x,y; ll g = extGcd(30LL,18LL,x,y);
 // g=6 and 30*x+18*y=6
 // Postconditions validating the literal example above.
 assert(g == 6 && 30*x+18*y == 6);
}
#include "../../content/number-theory/ModInv.h"
void use_ModInv(){
 auto inv = modInv(3,7); // *inv=5; 4/3 mod 7 is 4*5 mod 7
 assert(!modInv(2,6));
 // Postconditions validating the literal example above.
 assert(inv && *inv == 5 && 4*(*inv)%7 == 6);
}
#include "../../content/number-theory/GeneralCRT.h"
void use_GeneralCRT(){
 auto [r,m] = crtMerge(2,6,5,9);
 // (14,18); check m>0 before using r
 // Postconditions validating the literal example above.
 assert(r == 14 && m == 18);
}
#include "../../content/number-theory/ModLog.h"
void use_ModLog(){
 assert(modLog(2,8,13) == 3);
}
#include "../../content/number-theory/ModSqrt.h"
void use_ModSqrt(){
 auto x = modSqrt(10,13); // roots 6 and 7
 // Postconditions validating the literal example above.
 assert(x && (*x == 6 || *x == 7) && *x*(*x)%13 == 10);
}
#include "../../content/number-theory/ModSum.h"
void use_ModSum(){
 assert(floorSum(4,3,2,1) == 4); // 0+1+1+2
}
#include "../../content/number-theory/SPF.h"
void use_SPF(){
 SPF s(101); auto f = s.factor(60); // {2,2,3,5}
 // Postconditions validating the literal example above.
 assert((f == vec<int>{2,2,3,5}));
}
#include "../../content/number-theory/TrialDivision.h"
void use_TrialDivision(){
 auto f = trialDivision(84LL); // {2,2,3,7}
 // Postconditions validating the literal example above.
 assert((f == vec<ll>{2,2,3,7}));
}
#include "../../content/number-theory/Prime64.h"
void use_Prime64(){
 bool prime = isPrime64(1000000007ULL);
 // Postconditions validating the literal example above.
 assert(prime);
}
#include "../../content/number-theory/Factor64.h"
void use_Factor64(){
 auto f = factor64(360); // {2,2,2,3,3,5}
 // Postconditions validating the literal example above.
 assert((f == vec<ull>{2,2,2,3,3,5}));
}
#include "../../content/number-theory/Divisors.h"
void use_Divisors(){
 auto d = divisors(vec<ll>{2,2,3});
 sort(all(d)); // {1,2,3,4,6,12}
 // Postconditions validating the literal example above.
 assert((d == vec<ll>{1,2,3,4,6,12}));
}
#include "../../content/number-theory/TotientMobius.h"
void use_TotientMobius(){
 auto [phi,mu] = numberFunctions(12); // phi[12]=4, mu[12]=0
 // Postconditions validating the literal example above.
 assert(sz(phi) == 13 && sz(mu) == 13 && phi[12] == 4 && mu[12] == 0);
}
#include "../../content/combinatorial/Binomial.h"
void use_Binomial(){
 assert(binom(8,3) == 56);
}
#include "../../content/combinatorial/ModComb.h"
void use_ModComb(){
 ModComb c(101,1000000007LL);
 assert(c.choose(100,3) == 161700);
}
#include "../../content/combinatorial/PermCycles.h"
void use_PermCycles(){
 auto len = cycleLengths({1,2,0,4,3}); // {3,2}
 // Postconditions validating the literal example above.
 assert((len == vec<int>{3,2}));
}
int main(){use_IntDivision();use_ModAlign();use_Int128();use_Probability();use_BinarySearch();use_Bisection();use_TernarySearch();use_ModPower();use_Matrix();use_Gauss();use_GaussMod();use_GaussBinary();use_Determinant();use_IntDeterminant();use_MatrixInverse();use_PolyInterpolate();use_LinearRecurrence();use_NTT();use_FastFourierTransform();use_ExtGCD();use_ModInv();use_GeneralCRT();use_ModLog();use_ModSqrt();use_ModSum();use_SPF();use_TrialDivision();use_Prime64();use_Factor64();use_Divisors();use_TotientMobius();use_Binomial();use_ModComb();use_PermCycles(); cout << "\nmath-usage: 34 literal snippets passed\n";}
