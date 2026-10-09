/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Approximate the first feasible real value.
 * Description: Finite l<r with f(l)=false, f(r)=true; f switches only from false to true.
 *  Returns the true-side boundary approximation, not f(x). Floating type T,
 *  k>=0 iterations: O(k) evaluations, O(1) memory. Stops if rounding prevents progress.
 * Usage: double x = bisect(0.,2.,[](double x) { return x*x >= 2; });
 */
#pragma once

template<class T, class F>
T bisect(T l, T r, F f, int k = 100) {
	static_assert(is_floating_point_v<T>);
	assert(l < r && k >= 0);
	while (k--) {
		T m = l < 0 && r > 0 ? (l+r)/2
			: l+(r-l)/2;
		if (m == l || m == r) break;
		if (f(m)) r = m;
		else l = m;
	}
	return r;
}
