/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Next multiple or periodic event, including the present time.
 * Description: nextMultiple(x,k) returns the smallest multiple of k >=x. nextTerm(x,t0,k)
 *  returns the first t0+j*k >=x with j>=0. Integer operands, k>0; result must fit.
 *  O(1) time/memory.
 * Usage: assert(nextTerm(17LL,3LL,5LL) == 18);
 */
#pragma once

template<class T> T nextMultiple(T x, T k) {
	T r = x % k;
	if (r > 0) return x + (k - r);
	return x - r;
}

template<class T> T nextTerm(T x, T t0, T k) {
	if (x <= t0) return t0;
	T a = x % k, b = t0 % k;
	if (a < 0) a += k;
	if (b < 0) b += k;
	if (a <= b) return x + (b - a);
	return x + (k - (a - b));
}
