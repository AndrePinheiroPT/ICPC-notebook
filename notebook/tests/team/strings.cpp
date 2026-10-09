#include "prelude.hpp"
#include "../../content/strings/KMP.h"
#include "../../content/strings/ZFunction.h"
#include "../../content/strings/DoubleHash.h"
#include "../../content/strings/AlphabetTrie.h"
#include "../../content/strings/Aho.h"
#include "../../content/strings/Suffixes.h"
#include "../../content/strings/Palindromes.h"
#include "../../content/strings/LeastRotation.h"
#include "../../content/data-structures/RMQ.h"
static long long checks=0;
#define CHECK(x) do {++checks; if (!(x)) {cerr<<"FAIL line "<<__LINE__<<" "<<#x<<endl; abort();}} while(0)
mt19937 rng(3249071);
int rnd(int l,int r) {return uniform_int_distribution<int>(l,r)(rng);}
string randomString(int n,int alphabet){string s;while(n--)s+=char(rnd(0,alphabet-1));return s;}
DoubleHash::Key oracle(const vec<int>&s){DoubleHash::Key key{0,0,sz(s)};for(int x:s)rep(k,0,2)key[k]=(ll)(((__int128)key[k]*DoubleHash::base[k]+x)%DoubleHash::mod[k]);return key;}
int main(){
 rep(trial,0,350){
  string s=randomString(rnd(0,55),trial%2?4:256),p=randomString(rnd(0,15),trial%2?4:256);
  auto pi=prefixFunction(s),z=zFunction(s),matches=kmpMatches(s,p);vec<int>want;
  rep(i,0,sz(s)+1)if(i+sz(p)<=sz(s)&&s.substr(i,sz(p))==p)want.pb(i);
  CHECK(matches==want);
  rep(i,0,sz(s)){int border=0;rep(k,1,i+1)if(s.substr(0,k)==s.substr(i-k+1,k))border=k;CHECK(pi[i]==border);
   int len=0;if(i)while(i+len<sz(s)&&s[len]==s[i+len])++len;CHECK(z[i]==len);}
  Suffixes suffix(s);vec<int>sa(sz(s));iota(all(sa),0);
  sort(all(sa),[&](int a,int b){while(a<sz(s)&&b<sz(s)){if(s[a]!=s[b])return (unsigned char)s[a]<(unsigned char)s[b];++a;++b;}return a==sz(s)&&b!=sz(s);});
  CHECK(suffix.sa==sa);rep(i,0,sz(s))CHECK(suffix.rank[sa[i]]==i);
  rep(i,0,sz(suffix.lcp)){int len=0,a=sa[i],b=sa[i+1];while(a+len<sz(s)&&b+len<sz(s)&&s[a+len]==s[b+len])++len;CHECK(suffix.lcp[i]==len);}
  RMQ rmq(suffix.lcp);
  rep(q,0,50)if(sz(s)>1){int a=rnd(0,sz(s)-1),b=rnd(0,sz(s)-1);if(a==b)continue;int len=0;while(a+len<sz(s)&&b+len<sz(s)&&s[a+len]==s[b+len])++len;
   int x=suffix.rank[a],y=suffix.rank[b];if(x>y)swap(x,y);CHECK(rmq.query(x,y)==len);}
  auto [odd,even]=palindromes(s);
  rep(i,0,sz(s)){int o=1,e=0;while(i-o>=0&&i+o<sz(s)&&s[i-o]==s[i+o])++o;while(i-e-1>=0&&i+e<sz(s)&&s[i-e-1]==s[i+e])++e;CHECK(odd[i]==o);CHECK(even[i]==e);}
  auto unsignedVec=[&](const string&a){return vec<unsigned char>(all(a));};
  int rot=0;rep(i,1,sz(s))if(unsignedVec(s.substr(i)+s.substr(0,i))<unsignedVec(s.substr(rot)+s.substr(0,rot)))rot=i;CHECK(leastRotation(s)==rot);
  vec<int>bytes;for(unsigned char c:s)bytes.pb(c);DoubleHash h(s);CHECK(h.get(0,sz(s))==oracle(bytes));
  rep(l,0,sz(s)+1)rep(r,l,sz(s)+1){vec<int>b(bytes.begin()+l,bytes.begin()+r);CHECK(h.get(l,r)==oracle(b));
   int m=rnd(l,r);CHECK(h.concat(h.get(l,m),h.get(m,r))==h.get(l,r));}
  for(int&x:bytes)x=rnd(0,1000000006);
  DoubleHash integer(bytes);CHECK(integer.get(0,sz(bytes))==oracle(bytes));
 }
 rep(trial,0,200){
  Trie t;vec<string>words;
  rep(i,0,90){string w=randomString(rnd(0,12),4);for(char&c:w)c+='a';t.insert(w);words.pb(w);
   if(i%3==0){t.insert(w);words.pb(w);}
   CHECK(t.count(w)==count(all(words),w));CHECK(t.contains(w));CHECK(t.walk(w)>=0);CHECK(t.countPrefix("")==sz(words));
   string p=w.substr(0,rnd(0,sz(w)));int count=0;for(auto&s:words)count+=s.substr(0,sz(p))==p;CHECK(t.countPrefix(p)==count);
  }
  CHECK(!t.contains("z"));CHECK(t.count("z")==0);CHECK(t.countPrefix("z")==0);CHECK(t.walk("z")==-1);
 }
 rep(trial,0,220){
  bool bytes=trial%2;int K=bytes?256:4,first=bytes?0:'a';Aho a(K,first);vec<string>patterns;
  rep(i,0,30){string w=randomString(rnd(0,7),bytes?256:4);for(char&c:w)c=char((unsigned char)c+first);patterns.pb(w);CHECK(a.add(w)==i);}
  // Empty and duplicate patterns must report distinct IDs.
  patterns.pb("");CHECK(a.add("")==sz(patterns)-1);patterns.pb(patterns[0]);CHECK(a.add(patterns[0])==sz(patterns)-1);a.build();
  string text=randomString(100,bytes?256:4);for(char&c:text)c=char((unsigned char)c+first);int state=0;ll total=0,oracleTotal=0;
  rep(end,0,sz(text)+1){if(end)state=a.go(state,(unsigned char)text[end-1]);vec<int>want,got;
   rep(i,0,sz(patterns)){int l=end-sz(patterns[i]);if(l>=0&&text.substr(l,end-l)==patterns[i])want.pb(i);}
   a.report(state,[&](int id){got.pb(id);});sort(all(got));CHECK(got==want);CHECK(a.count(state)==sz(want));total+=a.count(state);oracleTotal+=sz(want);
  }CHECK(total==oracleTotal);
 }
 {string allBytes;rep(c,0,256)allBytes+=char(c);DoubleHash h(allBytes);rep(i,0,256)CHECK(h.get(i,i+1)==DoubleHash::Key({i,i,1}));
  DoubleHash z(vec<int>{0}),zz(vec<int>{0,0});CHECK(z.get(0,1)!=zz.get(0,2));}
 {string s(100000,'a');Suffixes a(s);CHECK(a.sa[0]==99999&&a.sa.back()==0);CHECK(a.lcp.front()==1&&a.lcp.back()==99999);
  CHECK(sz(kmpMatches(s,string("aaa")))==99998);auto p=palindromes(s);CHECK(p[0][50000]==50000);CHECK(leastRotation(s)==0);}
 cout<<"strings: "<<checks<<" checks passed\n";
}
