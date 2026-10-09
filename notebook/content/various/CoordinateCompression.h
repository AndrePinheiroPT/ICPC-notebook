/**
 * Author: team notebook
 * Summary: Map ordered coordinates to compact indices.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item compressCoordinates(a) returns (values,rank), without changing a.
 *  values is sorted and unique; values[rank[i]]=a[i]. Equal values get
 *  equal ranks. Compress ALL values that will be compared together.
 *  \item Preserves order, not distances: widths use values[j]-values[i],
 *  not j-i. Requires consistent < and == (no NaN). Empty input is valid.
 *  $O(n\log n)$ time, $O(n)$ space.
 *  \end{itemize}
 * Usage: auto [values,rank]=compressCoordinates(vec<int>{50,10,50});
 *  // values={10,50}, rank={1,0,1}
 */
#pragma once

template<class T>
pair<vec<T>,vec<int>> compressCoordinates(const vec<T>& a) {
	vec<T> values = a;
	sort(values.begin(),values.end());
	values.erase(unique(values.begin(),values.end()),
		values.end());
	vec<int> rank;
	for (const T& x : a) {
		auto it = lower_bound(values.begin(),values.end(),x);
		rank.pb(int(it-values.begin()));
	}
	return {values,rank};
}
