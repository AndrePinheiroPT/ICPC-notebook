/**
 * Author: team adaptation of KACTL / Andre notebook
 * Summary: Integer hash map with a mixed, randomly salted hash.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item HashMap<V> maps ll keys to values V; standard unordered\_map
 *  interface (find, erase, count, operator[]). operator[] inserts a
 *  default value when absent; find does not. Negative keys are valid.
 *  \item reserve(expected) avoids repeated growth. Expected $O(1)$
 *  operations, worst-case $O(n)$; salt reduces predictable collision
 *  attacks but gives no deterministic guarantee. Hashing preserves exact
 *  key equality (unlike polynomial string fingerprints).
 *  \end{itemize}
 * Usage: HashMap<int> freq; freq.reserve(1000); ++freq[-7];
 */
#pragma once

struct IntHash {
	static uint64_t mix(uint64_t x) {
		x += 0x9e3779b97f4a7c15ULL;
		x = (x^(x>>30))*0xbf58476d1ce4e5b9ULL;
		x = (x^(x>>27))*0x94d049bb133111ebULL;
		return x^(x>>31);
	}
	size_t operator()(ll x) const {
		static const uint64_t salt = chrono::steady_clock::now()
			.time_since_epoch().count();
		return mix(uint64_t(x)+salt);
	}
};
template<class V> using HashMap = unordered_map<ll,V,IntHash>;
