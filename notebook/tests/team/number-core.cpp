#include "prelude.hpp"
#include "../../content/number-theory/ExtGCD.h"
#include "../../content/number-theory/ModInv.h"
#include "../../content/number-theory/GeneralCRT.h"
#include "../../content/number-theory/Prime64.h"
#include "../../content/number-theory/Factor64.h"
#include "../../content/number-theory/ModLog.h"
#include "../../content/number-theory/ModSqrt.h"
#include "../../content/number-theory/ModSum.h"
#include "../../content/number-theory/TotientMobius.h"
#include "../../content/number-theory/SPF.h"
#include "../../content/number-theory/TrialDivision.h"
#include "../../content/number-theory/Divisors.h"
int main(){
 mt19937_64 rng(42091);ll checks=0;
 for(ll a=-100;a<=100;++a)for(ll b=-100;b<=100;++b){ll x,y,g=extGcd(a,b,x,y);assert(g==gcd(a,b)&&a*x+b*y==g);++checks;}
 for(ll m=2;m<=200;++m)for(ll a=-200;a<=200;++a){auto x=modInv(a,m);assert(bool(x)==(gcd(a,m)==1));if(x)assert((a*(*x)%m+m)%m==1);++checks;}
 rep(it,0,10000){ll m=1+rng()%50,n=1+rng()%50,a=int(rng()%201)-100,b=int(rng()%201)-100;auto [r,M]=crtMerge(a,m,b,n);ll want=-1,L=lcm(m,n);rep(x,0,L)if((x-a)%m==0&&(x-b)%n==0){want=x;break;}if(want==-1)assert(M==0);else assert(M==L&&r==want);++checks;}
 assert(crtMerge(0,LLONG_MAX,0,LLONG_MAX-1).second==-1);
 SPF s(100001);auto [phi,mu]=numberFunctions(10000);
 rep(n,1,100001){bool prime=n>=2&&s.p[n]==n;assert(isPrime64(n)==prime);if(n<5000){auto a=s.factor(n),b=trialDivision(n);assert(a==b);auto ds=divisors(a);sort(all(ds));vec<int>w;for(int d=1;d<=n;++d)if(n%d==0)w.pb(d);assert(ds==w);if(n<2000){auto c=factor64(n);assert(vec<ull>(all(a))==c);}}++checks;}
 rep(n,1,3001){int p=0;rep(k,1,n+1)p+=gcd(n,k)==1;assert(phi[n]==p);int total=0;rep(d,1,n+1)if(n%d==0)total+=mu[d];assert(total==(n==1));++checks;}
 vec<ull> large{18446744073709551557ULL,18446744073709551615ULL,1000000007ULL*1000000009ULL,1ULL<<63,4294967291ULL*4294967279ULL,3215031751ULL};
 for(ull n:large){auto f=factor64(n);__uint128_t product=1;for(ull p:f){assert(isPrime64(p));product*=p;}assert(product==n);++checks;}
 assert(isPrime64(18446744073709551557ULL));assert(!isPrime64(3215031751ULL));
 for(ll m=1;m<=90;++m)for(ll a=0;a<m;++a){vec<ll>w(m,-1);ll x=1%m;for(ll e=1;e<=m+1;++e){x=x*a%m;if(w[x]<0)w[x]=e;}for(ll b=0;b<m;++b){assert(modLog(a,b,m)==w[b]);++checks;}}
 rep(p,2,3000)if(s.p[p]==p){vec<int>roots(p);rep(x,0,p)roots[x*x%p]=1;rep(a,0,p){auto x=modSqrt(a,p);assert(bool(x)==bool(roots[a]));if(x)assert(*x*(*x)%p==a);++checks;}}
 rep(it,0,50000){ll n=rng()%100,m=1+rng()%30,a=int(rng()%101)-50,b=int(rng()%101)-50;ll f=0,r=0;rep(i,0,n){ll v=a*i+b;ll q=v/m-(v%m<0);f+=q;r+=v-q*m;}assert(floorSum(n,m,a,b)==f);assert(modSum(n,m,a,b)==r);++checks;}
 assert(floorSum(1000000000,1,1000000000,1000000000)==(__int128)1000000000*1000000000*1000000001/2);
 cout<<"number-core: "<<checks<<" checks passed\n";
}
