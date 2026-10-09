/**
 * Author: jet notebook
 * Summary: Manacher: all odd/even palindrome radii.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use to find palindromes around every centre. Input:
 *  string/vector s. Returns \{odd,even\} radius arrays.
 *  \item odd[i]=k means $[i-k+1,i+k)$, length 2k-1;
 *  even[i]=k means $[i-k,i+k)$, length 2k.
 *  Thus "aba" has odd[1]=2; "aa" has even[1]=1.
 *  \item Manacher reuses the furthest known palindrome [l,r].
 *  Empty input gives two empty arrays; $O(n)$ time/space.
 *  \end{itemize}
 * Usage: auto [odd,even] = palindromes(s);
 */
#pragma once

template<class S>
array<vec<int>,2> palindromes(const S& s) {
	int n = sz(s);
	vec<int> odd(n), even(n);
	for (int i = 0, l = 0, r = -1; i < n; ++i) {
		int k = i > r ? 1 : min(odd[l+(r-i)],r-i+1);
		while (k <= i && k < n-i && s[i-k] == s[i+k]) ++k;
		odd[i] = k;
		if (i+k-1 > r) { l = i-k+1; r = i+k-1; }
	}
	for (int i = 0, l = 0, r = -1; i < n; ++i) {
		int k = i > r ? 0 : min(even[l+(r-i)+1],r-i+1);
		while (k < i && k < n-i && s[i-k-1] == s[i+k]) ++k;
		even[i] = k;
		if (i+k-1 > r) { l = i-k; r = i+k-1; }
	}
	return {odd,even};
}
