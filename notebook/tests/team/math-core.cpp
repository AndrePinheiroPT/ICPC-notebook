#include "prelude.hpp"
#include "../../content/math/IntDivision.h"
#include "../../content/math/ModAlign.h"
#include "../../content/math/Int128.h"
#include "../../content/math/Probability.h"
#include "../../content/numerical/BinarySearch.h"
#include "../../content/numerical/Bisection.h"
#include "../../content/numerical/TernarySearch.h"
#include "../../content/numerical/Gauss.h"
#include "../../content/numerical/GaussMod.h"
#include "../../content/numerical/GaussBinary.h"
#include "../../content/numerical/Matrix.h"
#include "../../content/numerical/LinearRecurrence.h"
#include "../../content/numerical/PolyInterpolate.h"
#include "../../content/numerical/Determinant.h"
#include "../../content/numerical/IntDeterminant.h"
#include "../../content/numerical/MatrixInverse.h"
#include "../../content/numerical/NTT.h"
#include "../../content/numerical/FastFourierTransform.h"
#include "../../content/combinatorial/Binomial.h"
#include "../../content/combinatorial/ModComb.h"
#include "../../content/combinatorial/PermCycles.h"

ll detBrute(const vec<vec<ll>>& a) {
 int n=sz(a); vec<int> p(n); iota(all(p),0); ll ans=0;
 do {ll v=1; int odd=0; rep(i,0,n){v*=a[i][p[i]];rep(j,i+1,n)odd^=p[i]>p[j];} ans+=odd?-v:v;}while(next_permutation(all(p)));
 return ans;
}
int main() {
 mt19937 rng(471830); long long checks=0;
 for(ll a=-100;a<=100;++a) for(ll b=-20;b<=20;++b) if(b){
  assert(floorDiv(a,b)==(ll)floorl((long double)a/b));
  assert(ceilDiv(a,b)==(ll)ceill((long double)a/b));++checks;
 }
 for(ll x=-100;x<=100;++x) for(ll k=1;k<=20;++k){
  ll want=x;while(want%k)++want;assert(nextMultiple(x,k)==want);
  for(ll t=-20;t<=20;++t){ll e=t;while(e<x)e+=k;assert(nextTerm(x,t,k)==e);++checks;}
 }
 vec<string> strings{"0","1","-1","170141183460469231731687303715884105727","-170141183460469231731687303715884105728"};
 for(auto s:strings){istringstream in(s);i128 x=3;assert(read128(in,x));assert(toString(x)==s);++checks;}
 for(auto s:{"+","xyz","170141183460469231731687303715884105728","-170141183460469231731687303715884105729"}){istringstream in(s);i128 x=7;assert(!read128(in,x)&&x==7);}
 for(ll l=-10;l<=10;++l)for(ll r=l;r<=10;++r)for(ll t=-11;t<=11;++t){
  auto x=firstTrue(l,r,[&](ll a){return a>=t;});auto y=lastTrue(l,r,[&](ll a){return a<=t;});
  if(max(l,t)<r)assert(x&&*x==max(l,t));else assert(!x);
  if(min(r-1,t)>=l)assert(y&&*y==min(r-1,t));else assert(!y);++checks;
 }
 assert(firstTrue(LLONG_MIN,LLONG_MAX,[](ll x){return x>=0;})==0);
 assert(lastTrue(LLONG_MIN,LLONG_MAX,[](ll x){return x<0;})==-1);
 assert(firstTrue(LLONG_MIN,LLONG_MIN+1,[](ll){return true;})==LLONG_MIN);
 rep(t,0,100){long double target=(int(rng()%1000)-500)/13.0L;
  auto x=ternaryMin(-100,100,[&](long double x){return (x-target)*(x-target);});assert(abs(x-target)<1e-10L);
  auto y=bisect(-100.0L,100.0L,[&](long double x){return x>=target;});assert(y>=target&&abs(y-target)<1e-10L);
 }
 rep(n,0,18)rep(q,0,11){long double p=q/10.0L;auto prob=binomialProb(n,p);long double sum=0,mean=0;
  rep(k,0,n+1){auto exact=binom(n,k)*powl(p,k)*powl(1-p,n-k);assert(abs(prob[k]-exact)<1e-14L);sum+=prob[k];mean+=k*prob[k];}
  assert(abs(sum-1)<1e-14L&&abs(mean-n*p)<1e-13L);assert(abs(atLeastOne(n,p)-(1-prob[0]))<1e-14L);
  rep(k,1,20) { assert(abs(geometricProb(k,p)-p*powl(1-p,k-1))<1e-14L); }
  ++checks;
 }
 assert(atLeastOne(1000000000LL,1e-20L)>0);
 ModComb comb(51,1000000007LL);
 rep(n,0,51)rep(k,-1,n+2){assert(comb.choose(n,k)==binom(n,k)%1000000007);++checks;}
 rep(n,0,9){vec<int> p(n);iota(all(p),0);int count=0;do{auto len=cycleLengths(p);assert(accumulate(all(len),0)==n);++checks;}while(++count<1000&&next_permutation(all(p)));}
 rep(it,0,500){int n=1+rng()%5,m=1+rng()%5,p=5;vec<vec<int>> a(n,vec<int>(m+1));
  for(auto& row:a)for(int& v:row)v=int(rng()%11)-5;
  vec<int>x;int rank=gaussMod(a,m,x,p),solutions=0;int lim=1;rep(j,0,m)lim*=p;
  rep(mask,0,lim){int z=mask;vec<int> y(m);for(int&v:y){v=z%p;z/=p;}bool ok=true;for(auto&row:a){int s=-row[m];rep(j,0,m)s+=row[j]*y[j];ok&=s%p==0;}solutions+=ok;}
  assert((rank==-1)==(solutions==0));if(rank!=-1){int expected=1;rep(j,rank,m)expected*=p;assert(solutions==expected);for(auto&row:a){int s=-row[m];rep(j,0,m)s+=row[j]*x[j];assert(s%p==0);}}++checks;
 }
 rep(it,0,500){int n=rng()%7,m=rng()%7;vec<bitset<8>> a(n);for(auto&row:a)rep(j,0,m+1)row[j]=rng()&1;auto original=a;vec<int>x;int rank=gaussBinary(a,m,x),solutions=0;
  rep(mask,0,1<<m){bool ok=true;for(auto&row:original){int v=row[m];rep(j,0,m)v^=row[j]&&((mask>>j)&1);ok&=!v;}solutions+=ok;}
  assert((rank==-1)==!solutions);if(rank!=-1){assert(solutions==(1<<(m-rank)));for(auto&row:original){int v=row[m];rep(j,0,m)v^=row[j]&&x[j];assert(v==0);}}++checks;
 }
 rep(it,0,300){int n=1+rng()%6;vec<vec<ll>> a(n,vec<ll>(n));vec<vec<long double>> b(n,vec<long double>(n));for(int i=0;i<n;++i)for(int j=0;j<n;++j)b[i][j]=a[i][j]=int(rng()%9)-4;
  ll d=detBrute(a);assert(abs(determinant(b)-d)<1e-6L);
  for(ll mod:{1LL,2LL,6LL,101LL,LLONG_MAX}) {ll w=d%mod;if(w<0)w+=mod;assert(detMod(a,mod)==w);++checks;}
  auto inv=matrixInverse(b);assert(bool(inv)==(d!=0));if(inv)rep(i,0,n)rep(j,0,n){long double v=0;rep(k,0,n)v+=b[i][k]*(*inv)[k][j];assert(abs(v-(i==j))<1e-7L);}
  vec<vec<long double>> aug=b;vec<long double>truth(n);for(auto&v:truth)v=int(rng()%9)-4;
  rep(i,0,n){long double rhs=0;rep(j,0,n)rhs+=b[i][j]*truth[j];aug[i].pb(rhs);}vec<long double>x;int rank=gauss(aug,n,x);assert(rank>=0);rep(i,0,n){long double rhs=0;rep(j,0,n)rhs+=b[i][j]*x[j];long double w=0;rep(j,0,n)w+=b[i][j]*truth[j];assert(abs(rhs-w)<1e-7L);}
 }
 {vec<vec<double>> a{{1,1},{1,2}};vec<double>x;assert(gauss(a,1,x)==-1);}
 rep(it,0,300){int d=1+rng()%7;ll mod=1+rng()%1000000000;vec<ll>s(d),c(d);for(auto*v:{&s,&c})for(ll&x:*v)x=int(rng()%21)-10;vec<ll> seq=s;for(ll&x:seq){x%=mod;if(x<0)x+=mod;}
  rep(k,d,100){__int128 v=0;rep(j,0,d)v+=(__int128)c[j]*seq[k-1-j];ll x=ll(v%mod);if(x<0)x+=mod;seq.pb(x);}
  rep(k,0,100){assert(linearRec(s,c,k,mod)==seq[k]);++checks;}
 }
 {vec<vec<ull>> a{{ULLONG_MAX-2,ULLONG_MAX-3},{7,9}};ull mod=ULLONG_MAX;auto c=matMul(a,a,mod);rep(i,0,2)rep(j,0,2){ull v=0;rep(k,0,2)v=(__uint128_t(v)+__uint128_t(a[i][k])*a[k][j]%mod)%mod;assert(c[i][j]==v);}}
 rep(it,0,200){int n=1+rng()%4;ll mod=1+rng()%1000;vec<vec<ll>>a(n,vec<ll>(n)),r(n,vec<ll>(n));rep(i,0,n){r[i][i]=1%mod;rep(j,0,n)a[i][j]=rng()%mod;}rep(e,0,20){assert(matPow(a,e,mod)==r);r=matMul(r,a,mod);++checks;}}
 rep(it,0,200){int n=1+rng()%7;vec<long double>a(n),x(n),y(n);rep(i,0,n){a[i]=int(rng()%9)-4;x[i]=i-3;}rep(i,0,n){long double p=1;rep(j,0,n){y[i]+=a[j]*p;p*=x[i];}}auto z=interpolate(x,y);rep(i,0,n)assert(abs(z[i]-a[i])<1e-8L);++checks;}
 rep(it,0,200){int n=rng()%80,m=rng()%80;vec<int>a(n),b(m);vec<long double>x(n),y(m);rep(i,0,n)x[i]=a[i]=rng()%100;rep(i,0,m)y[i]=b[i]=rng()%100;auto c=NTT::conv(a,b);auto f=FFT::conv(x,y);vec<ll>w(n&&m?n+m-1:0);rep(i,0,n)rep(j,0,m)w[i+j]+=ll(a[i])*b[j];assert(sz(c)==sz(w)&&sz(f)==sz(w));rep(i,0,sz(w)){assert(c[i]==w[i]);assert(abs(f[i]-w[i])<1e-7L);++checks;}}
 cout<<"math-core: "<<checks<<" checks passed\n";
}
