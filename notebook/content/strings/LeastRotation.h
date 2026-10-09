/**
 * Author: jet notebook
 * Summary: Lexicographically smallest cyclic rotation.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use to give circular strings/sequences a common form.
 *  \textbf{Input:} string/vector s. Returns the first start index
 *  of the smallest rotation; "baba" gives 1, empty gives 0.
 *  \item Read the rotation as s[(start+i)\%n]. i,j are candidate
 *  starts; k counts matching symbols; at wraps the index
 *  and treats char as an unsigned byte. Other types keep
 *  their order. $O(n)$ time, $O(1)$ extra space.
 *  \end{itemize}
 * Usage: int start = leastRotation(s);
 */
#pragma once

template<class S>
int leastRotation(const S& s) {
	int n = sz(s);
	ll i = 0, j = 1, k = 0;
	auto at = [&](ll pos) {
		auto c = s[size_t(pos%n)];
		if constexpr (is_same_v<decltype(c),char>)
			return (unsigned char)c;
		else return c;
	};
	while (i < n && j < n && k < n) {
		auto a = at(i+k), b = at(j+k);
		if (a == b) { ++k; continue; }
		if (b < a) {
			i += k+1;
			if (i <= j) i = j+1;
		} else {
			j += k+1;
			if (j <= i) j = i+1;
		}
		k = 0;
	}
	return n ? int(min(i,j)) : 0;
}
