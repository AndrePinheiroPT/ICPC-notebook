#include "prelude.hpp"
#include "../../content/various/CoordinateCompression.h"
#include "../../content/various/TwoPointers.h"
#include "../../content/various/MonotonicStack.h"
#include "../../content/various/MonotonicDeque.h"
#include "../../content/various/Subsequence.h"
#include "../../content/various/ClassicDP.h"
#include "../../content/various/SubsetBits.h"
#include "../../content/various/SOS.h"
#include "../../content/various/DigitDP.h"
#include "../../content/various/DivideAndConquerDP.h"
#include "../../content/various/XorBasis.h"
#include "../../content/various/CoverIntervals.h"
#include "../../content/various/Histogram.h"
#include "../../content/various/FunctionalCycle.h"
#include "../../content/various/LineHull.h"
#include "../../content/various/GregorianDays.h"
static long long checks=0;
#define CHECK(x) do {++checks; if (!(x)) {cerr<<"FAIL line "<<__LINE__<<" "<<#x<<endl; abort();}} while(0)
mt19937 rng(9934723);
int rnd(int l,int r) {return uniform_int_distribution<int>(l,r)(rng);}
bool subseq(string a,string b){int i=0;for(char c:b)if(i<sz(a)&&c==a[i])++i;return i==sz(a);}
int main(){
 rep(trial,0,220){
  int n=rnd(0,13);vec<int>a(n);for(int&x:a)x=rnd(-10,10);
  auto [v,rank]=compressCoordinates(a);CHECK(is_sorted(all(v)));CHECK(adjacent_find(all(v))==v.end());rep(i,0,n)CHECK(v[rank[i]]==a[i]);
  auto left=nearestSmaller(a),right=nearestSmaller(a,true);
  rep(i,0,n){int l=i-1,r=i+1;while(l>=0&&a[l]>=a[i])--l;while(r<n&&a[r]>=a[i])++r;CHECK(left[i]==l);CHECK(right[i]==r);}
  rep(k,1,n+3){auto m=slidingMin(a,k);CHECK(sz(m)==max(0,n-k+1));rep(l,0,sz(m)){int best=l;rep(i,l,l+k)if(a[i]<=a[best])best=i;CHECK(m[l]==best);}}
  rep(k,0,6){pair<int,int>best{0,0};rep(l,0,n)rep(r,l+1,n+1){set<int>s(a.begin()+l,a.begin()+r);if(sz(s)<=k&&r-l>best.second-best.first)best={l,r};}CHECK(longestKDistinct(a,k)==best);}
  for(bool weak:{false,true}){int best=0;rep(mask,0,1<<n){vec<int>b;rep(i,0,n)if(mask>>i&1)b.pb(a[i]);bool valid=true;rep(i,1,sz(b))valid&=weak?b[i-1]<=b[i]:b[i-1]<b[i];if(valid)best=max(best,sz(b));}
   auto id=lis(a,weak);CHECK(sz(id)==best);rep(i,1,sz(id)){CHECK(id[i-1]<id[i]);CHECK(weak?a[id[i-1]]<=a[id[i]]:a[id[i-1]]<a[id[i]]);}}
  vec<ll>b(n);for(ll&x:b)x=rnd(0,20);ll limit=rnd(0,50);pair<int,int>best{0,0};rep(l,0,n)rep(r,l+1,n+1)if(accumulate(b.begin()+l,b.begin()+r,0LL)<=limit&&r-l>best.second-best.first)best={l,r};CHECK(longestSumAtMost(b,limit)==best);
  vec<int>w(n);vec<ll>value(n);for(int&x:w)x=rnd(0,12);for(ll&x:value)x=rnd(-10,30);int cap=rnd(0,40);ll optimum=0;bitset<128>bits;
  rep(mask,0,1<<n){int weight=0;ll val=0;rep(i,0,n)if(mask>>i&1){weight+=w[i];val+=value[i];}if(weight<128)bits[weight]=1;if(weight<=cap)optimum=max(optimum,val);}
  CHECK(knapsack01(w,value,cap)==optimum);CHECK(subsetBits<128>(w)==bits);
  if(n){vec<ll>s(all(a));ll linear=LLONG_MIN,circular=LLONG_MIN;rep(l,0,n)rep(len,1,n+1){ll sum=0;rep(i,0,len)sum+=s[(l+i)%n];circular=max(circular,sum);if(l+len<=n)linear=max(linear,sum);}CHECK(maxSubarray(s)==make_pair(linear,circular));}
 }
 rep(trial,0,120){
  int k=rnd(0,8),n=1<<k;vec<ll>a(n);for(ll&x:a)x=rnd(-20,20);auto sub=a,sup=a; sos(sub);sos(sup,true);
  rep(mask,0,n){ll x=0,y=0;rep(s,0,n){if((s&mask)==s)x+=a[s];if((s&mask)==mask)y+=a[s];}CHECK(sub[mask]==x);CHECK(sup[mask]==y);}
  auto maximum=a;sos(maximum,false,[](ll x,ll y){return max(x,y);});rep(mask,0,n){ll x=LLONG_MIN;rep(s,0,n)if((s&mask)==s)x=max(x,a[s]);CHECK(maximum[mask]==x);}
 }
 rep(trial,0,160){
  int n=rnd(0,11);XorBasis b;vec<uint64_t>a(n);for(auto&x:a)x=rnd(0,255);for(auto x:a)b.add(x);set<uint64_t>poss;
  rep(mask,0,1<<n){uint64_t v=0;rep(i,0,n)if(mask>>i&1)v^=a[i];poss.insert(v);}
  CHECK(poss.size()==(1ULL<<b.rank));rep(x,0,300)CHECK(b.contains(x)==(poss.count(x)!=0));rep(start,0,20){uint64_t best=0;for(auto x:poss)best=max(best,x^start);CHECK(b.maximum(start)==best);}
 }
 {XorBasis b;rep(i,0,64)CHECK(b.add(1ULL<<i));CHECK(b.rank==64);CHECK(b.maximum()==ULLONG_MAX);CHECK(b.contains(ULLONG_MAX));CHECK(!b.add(ULLONG_MAX));}
 rep(trial,0,200){int bound=rnd(0,10000),d=rnd(1,25),mod=rnd(1,100000);int count=0;rep(i,0,bound+1){int s=0;for(char c:to_string(i))s+=c-'0';count+=s%d==0;}CHECK(digitSumCount(to_string(bound),d,mod)==count%mod);CHECK(digitSumCount("000"+to_string(bound),d,mod)==count%mod);}
 CHECK(digitSumCount("99999999999999999999",1,97)==73); // 10^20 mod 97
 rep(trial,0,150){int n=rnd(0,40);vec<ll>p(n+1);rep(i,0,n)p[i+1]=p[i]+rnd(0,10);vec<ll>prev(n+1,INFLL);prev[0]=0;
  auto cost=[&](int l,int r){ll x=p[r]-p[l];return x*x;};rep(g,0,8){vec<ll>slow(n+1,INFLL);rep(j,1,n+1)rep(k,0,j)if(prev[k]!=INFLL)slow[j]=min(slow[j],prev[k]+cost(k,j));auto fast=partitionLayer(prev,cost);CHECK(fast==slow);prev=fast;}}
 rep(trial,0,250){int n=rnd(0,18);vec<int>h(n),w(n);for(int&x:h)x=rnd(0,20);for(int&x:w)x=rnd(1,8);ll best=0;
  rep(l,0,n){int height=INT_MAX;ll width=0;rep(r,l,n){height=min(height,h[r]);width+=w[r];best=max(best,height*width);}}
  auto [area,l,r]=largestRectangle(h,w);CHECK(area==best);CHECK(0<=l&&l<=r&&r<=n);if(area)CHECK(area==ll(*min_element(h.begin()+l,h.begin()+r))*accumulate(w.begin()+l,w.begin()+r,0LL));
 }
 rep(trial,0,200){int n=rnd(0,9),L=rnd(0,6),R=rnd(L,12);vec<pair<int,int>>segments;rep(i,0,n){int l=rnd(0,12),r=rnd(l,12);segments.pb({l,r});}int best=INF;
  rep(mask,0,1<<n){bool ok=true;rep(x,L,R){bool covered=false;rep(i,0,n)if(mask>>i&1)covered|=segments[i].first<=x&&x<segments[i].second;ok&=covered;}if(ok)best=min(best,__builtin_popcount(unsigned(mask)));}
  auto ans=coverIntervals(segments,L,R);CHECK(bool(ans)==(best!=INF));if(ans){CHECK(sz(*ans)==best);rep(x,L,R){bool covered=false;for(int i:*ans)covered|=segments[i].first<=x&&x<segments[i].second;CHECK(covered);}}
 }
 rep(trial,0,300){int n=rnd(1,150),start=rnd(0,n-1);vec<int>next(n),seen(n,-1);for(int&x:next)x=rnd(0,n-1);int v=start,step=0;while(seen[v]<0){seen[v]=step++;v=next[v];}auto got=floydCycle(start,[&](int x){return next[x];});CHECK(got==make_pair(ll(seen[v]),ll(step-seen[v])));}
 rep(trial,0,200){LineHull h;vec<pair<ll,ll>>lines;rep(i,0,100){ll k=rnd(-200,200),m=rnd(-10000,10000);h.add(k,m);lines.pb({k,m});rep(j,0,12){ll x=rnd(-100000,100000),best=LLONG_MIN;for(auto[k,m]:lines)best=max(best,k*x+m);CHECK(h.query(x)==best);}}}
 {LineHull h;h.add(0,LLONG_MIN);h.add(0,LLONG_MAX);CHECK(h.query(LLONG_MIN)==LLONG_MAX);CHECK(h.query(LLONG_MAX)==LLONG_MAX);
  LineHull g;g.add(LLONG_MIN,0);g.add(LLONG_MAX,1);CHECK(g.query(0)==1);}
 rep(trial,0,160){vec<int>coins;rep(c,1,8)if(rnd(0,1))coins.pb(c);int S=rnd(0,30);vec<int>dist(S+1,-1);queue<int>q;q.push(0);dist[0]=0;while(!q.empty()){int s=q.front();q.pop();for(int c:coins)if(s+c<=S&&dist[s+c]<0){dist[s+c]=dist[s]+1;q.push(s+c);}}CHECK(minCoins(coins,S)==dist[S]);
  auto count=[&](auto&&self,int i,int left)->ll{if(i==sz(coins))return left==0;ll ans=0;for(int x=0;x<=left;x+=coins[i])ans+=self(self,i+1,left-x);return ans;};int mod=rnd(1,1000);CHECK(coinWays(coins,S,mod)==count(count,0,S)%mod);
  string a,b;rep(i,0,rnd(0,9))a+=char('a'+rnd(0,2));rep(i,0,rnd(0,9))b+=char('a'+rnd(0,2));int common=0;rep(mask,0,1<<sz(a)){string c;rep(i,0,sz(a))if(mask>>i&1)c+=a[i];if(subseq(c,b))common=max(common,sz(c));}CHECK(lcsLength(a,b)==common);
  int rows=rnd(0,6),cols=rnd(0,6);vec<string>g(rows,string(cols,'.'));for(auto&row:g)for(char&c:row)if(rnd(0,3)==0)c='#';
  auto paths=[&](auto&&self,int r,int c)->int{if(r>=rows||c>=cols||g[r][c]=='#')return 0;if(r==rows-1&&c==cols-1)return 1;return self(self,r+1,c)+self(self,r,c+1);};CHECK(gridPaths(g,mod)==paths(paths,0,0)%mod);
 }
 {CHECK(gregorianDays(1970,1,1)==0);ll expected=0;for(int y=1970;y<=2030;++y)for(int m=1;m<=12;++m){int days=31;if(m==4||m==6||m==9||m==11)days=30;if(m==2)days=28+(y%4==0&&(y%100!=0||y%400==0));for(int d=1;d<=days;++d){CHECK(gregorianDays(y,m,d)==expected++);CHECK(gregorianDays(y-400,m,d)==gregorianDays(y,m,d)-146097);}}
 CHECK(gregorianDays(0,3,1)-gregorianDays(0,2,28)==2);CHECK(gregorianDays(-400,3,1)-gregorianDays(-400,2,28)==2);}
 cout<<"various: "<<checks<<" checks passed\n";
}
