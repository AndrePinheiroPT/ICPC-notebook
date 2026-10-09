/**
 * Author: jet notebook
 * Summary: Prefix-match length at every position.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item zFunction(s) accepts a string/vector and returns z[i], the common
 *  prefix length of s and s[i,n). z[0]=0; empty input gives empty output.
 *  Useful for prefix occurrences, borders and periodicity.
 *  \item The matched block [l,r) is reused to avoid comparing twice.
 *  $O(n)$ time/space.
 *  \end{itemize}
 * Usage: auto z=zFunction(string("aaaa")); // {0,3,2,1}
 */
#pragma once

template<class S> vec<int> zFunction(const S& s) {
	int n = sz(s), l = 0, r = 0;
	vec<int> z(n);
	rep(i,1,n) {
		if (i < r) z[i] = min(r-i,z[i-l]);
		while (z[i] < n-i && s[z[i]] == s[i+z[i]])
			++z[i];
		if (i+z[i] > r) l = i, r = i+z[i];
	}
	return z;
}
