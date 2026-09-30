/**
 * Author: Neel Mishra
 * License: CC0
 * Source: https://neelmishra.github.io/blog/dsa/digit-dp/foundations.html
 * Description: 
 * Time:
 * Status: it works
 */

struct DigitDP {
    string D;
    // memo[pos][state] - only populated when tight == 0 && lz == 0
    vector<vector<long long>> memo;

    long long solve(int pos, int tight, int lz, int state) {
        if (pos == (int)D.size()) return isAccepting(lz, state) ? 1 : 0;
        if (!tight && !lz && memo[pos][state] != -1) return memo[pos][state];

        int hi = tight ? (D[pos] - '0') : 9;
        long long ans = 0;
        for (int d = 0; d <= hi; ++d) {
            if (!passesFilter(d, lz, state)) continue;
            int ns   = nextState(d, lz, state);
            int nlz  = lz && (d == 0);
            int nt   = tight && (d == hi);
            ans += solve(pos + 1, nt, nlz, ns);
        }
        if (!tight && !lz) memo[pos][state] = ans;
        return ans;
    }

    long long countUpTo(long long N) {
        D = to_string(N);
        memo.assign(D.size(), vector<long long>(STATE_MAX, -1));
        return solve(0, 1, 1, INITIAL_STATE);
    }

    // Plug in these four hooks:
    bool isAccepting(int lz, int state);     // terminal acceptance predicate
    bool passesFilter(int d, int lz, int s); // digit-level validity
    int  nextState(int d, int lz, int s);    // state transition
    // ... STATE_MAX, INITIAL_STATE defined per problem
};

// Errichto template
vec<vec<ll>> dp(d,vec<ll>(2,0)); // sum/is_smaller
dp[0][0]=1;
f(i,0,k.length()){
    vec<vec<ll>> dp2(d,vec<ll>(2,0));
    f(sum,0,d) f(ok,0,2) f(dig,0,10){
        if(dig<=k[i]-'0' || ok){
            dp2[(sum+dig)%d][ok || (dig<k[i]-'0')] += dp[sum][ok];
            dp2[(sum+dig)%d][ok || (dig<k[i]-'0')] %= MOD;
        }
    }
    dp=dp2;
}