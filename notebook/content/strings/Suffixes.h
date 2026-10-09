/**
 * Author: jet notebook
 * Summary: Sorted suffixes, inverse ranks and adjacent LCP.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use for repeated/common substrings or many suffix
 *  comparisons. Input: string s; unsigned-byte order.
 *  \item sa[i] is the start of suffix i in sorted order;
 *  rank[start] gives its position there. lcp[i] is the
 *  common-prefix length of sa[i] and sa[i+1]. No empty
 *  suffix: sizes n, n, max(0,n-1). max(lcp) is the
 *  longest repeated length (0 if lcp is empty).
 *  \item Doubling sorts pairs of ranks, then a linear scan builds
 *  lcp. $O(n\log n)$ time, $O(n)$ extra space.
 *  \end{itemize}
 * Usage: Suffixes a(s); RMQ<int> rmq(a.lcp);
 *  // suffixes i!=j: rmq.query(min(a.rank[i],a.rank[j]),
 *  // max(a.rank[i],a.rank[j])); i==j gives n-i.
 */
#pragma once

struct Suffixes {
	vec<int> sa, rank, lcp;
	Suffixes(string_view s) : sa(sz(s)),
		rank(sz(s)), lcp(max(0,sz(s)-1)) {
		int n = sz(s), c = 0;
		if (!n) return;
		iota(sa.begin(),sa.end(),0);
		sort(sa.begin(),sa.end(),[&](int a,int b) {
			return (unsigned char)s[a] <
				(unsigned char)s[b];
		});
		rep(i,0,n) {
			if (!i || s[sa[i]] != s[sa[i-1]]) ++c;
			rank[sa[i]] = c-1;
		}
		vec<int> y(n), r(n), cnt(n);
		for (int k = 1; c < n; k *= 2) {
			int p = 0;
			rep(i,n-k,n) y[p++] = i;
			for (int v : sa)
				if (v >= k) y[p++] = v-k;
			fill(cnt.begin(),cnt.begin()+c,0);
			for (int v : y) ++cnt[rank[v]];
			rep(i,1,c)
				cnt[i] += cnt[i-1];
			for (int i = n; i--;)
				sa[--cnt[rank[y[i]]]] = y[i];
			c = 1; r[sa[0]] = 0;
			rep(i,1,n) {
				int a = sa[i-1], b = sa[i];
				int ra = a < n-k ? rank[a+k] : -1;
				int rb = b < n-k ? rank[b+k] : -1;
				if (rank[a] != rank[b] || ra != rb) ++c;
				r[b] = c-1;
			}
			rank.swap(r);
			if (c == n) break;
		}
		for (int i = 0, h = 0; i < n; ++i) {
			int rnk = rank[i];
			if (rnk == n-1) { h = 0; continue; }
			int j = sa[rnk+1];
			while (h < n-i && h < n-j &&
					s[i+h] == s[j+h]) ++h;
			lcp[rnk] = h;
			if (h) --h;
		}
	}
};
