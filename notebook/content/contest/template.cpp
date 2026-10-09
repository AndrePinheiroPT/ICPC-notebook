#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using ull = unsigned long long;
template<class T> using vec = vector<T>;
#define rep(i,a,b) for (int i = (a); i < (b); ++i)
#define pb push_back
#define all(x) (x).begin(), (x).end()
constexpr int INF = 1000000000;
constexpr ll INFLL = LLONG_MAX/4;
#define sz(x) (int)(x).size()

#ifdef LOCAL
template<class T, class = void>
struct dbg_pair : false_type {};
template<class T> struct dbg_pair<T, void_t<
    decltype(declval<const T&>().first),
    decltype(declval<const T&>().second)>> : true_type {};
template<class T, class = void>
struct dbg_range : false_type {};
template<class T> struct dbg_range<T, void_t<
    decltype(begin(declval<const T&>())),
    decltype(end(declval<const T&>()))>> : true_type {};
template<class T> void dbg_print(const T& x) {
    if constexpr (
        is_convertible_v<const T&, string_view>)
        cerr << '"' << x << '"';
    else if constexpr (dbg_pair<T>::value) {
        cerr << '('; dbg_print(x.first);
        cerr << ','; dbg_print(x.second); cerr << ')';
    } else if constexpr (dbg_range<T>::value) {
        cerr << '[';
        const char* sep = "";
        for (const auto& v : x) {
            cerr << sep; dbg_print(v); sep = ",";
        }
        cerr << ']';
    } else cerr << x;
}
#define DEBUG(x) do { \
    cerr << #x << " = "; \
    dbg_print(x); cerr << '\n'; \
} while (0)
#else
#define DEBUG(x) ((void)0)
#endif

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

}
