// Independent cross-agent audit: targeted large/signed arithmetic beyond the
// small random/exhaustive cases in math-core and number-core. Seed fixed.
#include "prelude.hpp"
#include "../../content/number-theory/ModLog.h"
#include "../../content/number-theory/ModSqrt.h"
#include "../../content/number-theory/Prime64.h"
#include "../../content/number-theory/ModSum.h"
#include "../../content/numerical/LinearRecurrence.h"
#include "../../content/numerical/NTT.h"
mt19937_64 gen(228192);
int main() {
    int checks=0;
    rep(it,0,4000) {
        ll m=1+gen()%4000,a=ll(gen()),b=ll(gen());
        ll an=a%m; if(an<0)an+=m;
        ll bn=b%m; if(bn<0)bn+=m;
        ll cur=1%m,expected=-1;
        for(ll x=1;x<=m+1;++x) {
            cur=cur*an%m;
            if(cur==bn){expected=x;break;}
        }
        assert(modLog(a,b,m)==expected); ++checks;
    }
    // A large p=1 mod4 exercises the Tonelli loop, not just its p=3 mod4 shortcut.
    ll prime=LLONG_MAX-2;
    while(!isPrime64(prime))prime-=4;
    for(ll p:{2LL,998244353LL,prime,9223372036854775783LL}) {
        assert(isPrime64(p));
        rep(it,0,400) {
            ll x=ll(gen()%ull(p));
            ll square=ll((__uint128_t)x*x%p);
            auto root=modSqrt(square,p);
            assert(root);
            assert((__uint128_t)*root*(*root)%p==ull(square)); ++checks;
        }
    }
    for(ll m:{1LL,6LL,LLONG_MAX-1,LLONG_MAX}) rep(it,0,200) {
        int d=1+gen()%6;
        vec<ll>s(d),c(d),a;
        for(auto*v:{&s,&c})for(ll&x:*v)x=ll(gen());
        a=s;
        for(ll&x:a){x%=m;if(x<0)x+=m;}
        rep(k,d,80) {
            ll value=0;
            rep(j,0,d) {
                ll coeff=c[j]%m; if(coeff<0)coeff+=m;
                value=ll((__uint128_t(value)+__uint128_t(coeff)*a[k-1-j])%m);
            }
            a.pb(value);
        }
        rep(k,0,80){assert(linearRec(s,c,k,m)==a[k]);++checks;}
    }
    rep(it,0,250) {
        int n=1+gen()%80,m=1+gen()%80;
        vec<int>a(n),b(m);
        for(int&x:a)x=gen()%NTT::mod;
        for(int&x:b)x=gen()%NTT::mod;
        auto actual=NTT::conv(a,b);
        vec<ll>expected(n+m-1);
        rep(i,0,n)rep(j,0,m)
            expected[i+j]=(expected[i+j]+ll(a[i])*b[j])%NTT::mod;
        rep(i,0,n+m-1)assert(actual[i]==expected[i]);
        ++checks;
    }
    for(ll m:{1LL,2LL,1000000000LL})
    for(ll a:{-1000000000LL,-1LL,0LL,1LL,1000000000LL})
    for(ll b:{-1000000000LL,0LL,1000000000LL}) {
        assert(floorSum(0,m,a,b)==0&&modSum(0,m,a,b)==0);
        ll n=1000000000;
        auto rest=modSum(n,m,a,b);
        assert(0<=rest&&rest<(__int128)n*m);
        // floor(y/m)+floor((-y+m-1)/m)==0 for integer y.
        // Both function calls must respect its documented |b|<=1e9 domain.
        if(-b+m-1<=1000000000&&-b+m-1>=-1000000000)
            assert(floorSum(n,m,a,b)+floorSum(n,m,-a,-b+m-1)==0);
        ++checks;
    }
    cout<<"math-crossaudit: "<<checks<<" extreme/signed checks passed\n";
}
