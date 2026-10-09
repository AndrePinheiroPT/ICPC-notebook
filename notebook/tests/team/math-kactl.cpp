// Adapted from supplied KACTL stress-tests/numerical/{LinearRecurrence,
// IntDeterminant,SolveLinearBinary}.cpp: enumerate small instances, compare
// with direct recurrence / permutation determinant / all XOR assignments.
#include "prelude.hpp"
#include "../../content/numerical/LinearRecurrence.h"
#include "../../content/numerical/IntDeterminant.h"
#include "../../content/numerical/GaussBinary.h"
int main(){
 ll checks=0;
 rep(d,1,4){int lim=1;rep(i,0,2*d)lim*=5;
  rep(mask,0,lim){int z=mask;vec<ll>s(d),c(d);for(auto*v:{&s,&c})for(ll&x:*v){x=z%5;z/=5;}
   vec<ll>full=s;rep(i,d,10*d+3){ll v=0;rep(j,0,d)v+=c[j]*full[i-1-j];full.pb(v%5);}
   rep(i,0,sz(full)){assert(linearRec(s,c,i,5)==full[i]);++checks;}
  }
 }
 rep(n,0,4){int lim=1;rep(i,0,n*n)lim*=3;
  rep(mask,0,lim){vec<vec<ll>>a(n,vec<ll>(n));int z=mask;for(auto&row:a)for(ll&x:row){x=z%3;z/=3;}
   vec<int>p(n);iota(all(p),0);ll det=0;
   do {ll v=1;int odd=0;rep(i,0,n){v*=a[i][p[i]];rep(j,i+1,n)odd^=p[i]>p[j];}det+=odd?-v:v;}while(next_permutation(all(p)));
   for(ll mod:{2LL,3LL,4LL,6LL,7LL,1000000007LL}){assert(detMod(a,mod)==(det%mod+mod)%mod);++checks;}
  }
 }
 rep(n,0,4)rep(m,0,5){int bits=n*(m+1);
  rep(mask,0,1<<bits){vec<bitset<6>>a(n);int z=mask;for(auto&row:a)rep(j,0,m+1){row[j]=z&1;z>>=1;}auto before=a;
   int count=0;rep(assignment,0,1<<m){bool ok=true;for(auto&row:a){int v=row[m];rep(j,0,m)v^=row[j]&&((assignment>>j)&1);ok&=!v;}count+=ok;}
   vec<int>x;int rank=gaussBinary(a,m,x);assert((rank==-1)==(count==0));if(rank!=-1){assert(count==(1<<(m-rank)));for(auto&row:before){int v=row[m];rep(j,0,m)v^=row[j]&&x[j];assert(v==0);}}++checks;
  }
 }
 cout<<"math-kactl: "<<checks<<" exhaustive comparisons passed\n";
}
