/**
 * Author: supplied by user, adapted
 * Summary: Constant-time substring fingerprints (probabilistic).
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Construct from a string or vec<int>. get(l,r) returns
 *  \{hash1,hash2,length\} for $[l,r)$. Different keys prove inequality;
 *  equal keys may collide. h/p are prefix hashes/powers of size n+1.
 *  \item h.concat(x,y) represents x followed by y; the length of y must not
 *  exceed the input length of h. Strings use unsigned bytes. Integer IDs
 *  must lie in $[0,10^9+7)$: jointly compress ALL compared sequences,
 *  including signed/64-bit integers, before constructing hashes.
 *  \item All objects share fixed bases: deterministic and simple, but vulnerable
 *  to deliberately constructed collisions. For exact equality, verify
 *  matches or use an exact string algorithm. Build/space $O(n)$; get/concat $O(1)$.
 *  \end{itemize}
 * Usage: DoubleHash h("abc"), t("ac");
 *  bool same=h.concat(h.get(0,1),h.get(2,3))==t.get(0,2);
 */
#pragma once

struct DoubleHash {
	using Key = array<ll,3>; // hash1, hash2, comprimento
	static constexpr array<ll,2> mod{1000000007,1000000009};
	static constexpr array<ll,2> base{911382323,972663749};
	vec<array<ll,2>> h, p;
	explicit DoubleHash(const vec<int>& s)
		: h(sz(s)+1), p(sz(s)+1, {1,1}) {
		rep(i,0,sz(s)) {
			assert(0 <= s[i] && s[i] < mod[0]);
			rep(k,0,2) {
				h[i+1][k] = (h[i][k]*base[k]+s[i])%mod[k];
				p[i+1][k] = p[i][k]*base[k]%mod[k];
			}
		}
	}
	explicit DoubleHash(const vec<unsigned char>& s)
		: DoubleHash(vec<int>(s.begin(),s.end())) {}
	explicit DoubleHash(string_view s)
		: DoubleHash(vec<unsigned char>(s.begin(),s.end())) {}
	Key get(int l, int r) const {
		assert(0 <= l && l <= r && r < sz(h));
		Key ans{0,0,r-l};
		rep(k,0,2)
			ans[k] = (h[r][k]-h[l][k]*p[r-l][k]%mod[k]+mod[k])%mod[k];
		return ans;
	}
	Key concat(Key x, Key y) const {
		assert(0 <= y[2] && y[2] < sz(p));
		assert(0 <= x[2] && x[2] <= INT_MAX-y[2]);
		rep(k,0,2)
			x[k] = (x[k]*p[y[2]][k]+y[k])%mod[k];
		x[2] += y[2]; return x;
	}
};
