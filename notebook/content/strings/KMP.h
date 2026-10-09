/**
 * Author: jet notebook
 * Summary: Exact pattern occurrences and prefix borders.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item kmpMatches(text,pattern) returns all starting indices, including
 *  overlaps; an empty pattern matches all n+1 boundaries. Strings or
 *  vectors; no special separator is required.
 *  \item prefixFunction(s) returns pi[i]: length of the longest proper prefix
 *  of s[0,i+1) that is also its suffix. Borders: follow pi[n-1],
 *  pi[k-1], etc. Candidate period p=n-pi[n-1] tiles s iff n\%p=0.
 *  \item Prefix function $O(m)$; matching $O(n+m)$ time, $O(m)$ plus output space.
 *  \end{itemize}
 * Usage: auto at=kmpMatches(string("ababa"),string("aba")); // {0,2}
 */
#pragma once

template<class S>
vec<int> prefixFunction(const S& s) {
	vec<int> p(sz(s));
	rep(i,1,sz(s)) {
		int j = p[i-1];
		while (j && s[i] != s[j]) j = p[j-1];
		if (s[i] == s[j]) ++j;
		p[i] = j;
	}
	return p;
}
template<class S, class P>
vec<int> kmpMatches(const S& s, const P& p) {
	vec<int> ans;
	int n = sz(s), m = sz(p), j = 0;
	if (!m) {
		ans.resize(n+1);
		iota(ans.begin(),ans.end(),0);
		return ans;
	}
	vec<int> pi = prefixFunction(p);
	rep(i,0,n) {
		while (j && s[i] != p[j]) j = pi[j-1];
		if (s[i] == p[j]) ++j;
		if (j == m) {
			ans.pb(i-m+1); j = pi[j-1];
		}
	}
	return ans;
}
