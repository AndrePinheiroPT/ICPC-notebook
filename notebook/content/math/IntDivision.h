/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Floor/ceiling integer division, including negative operands.
 * Description: floorDiv(a,b) rounds down, ceilDiv(a,b) rounds up; C++ / rounds toward zero.
 *  Operands use the same integer type, b!=0; quotient must fit (exclude MIN/-1).
 *  O(1) time/memory. Useful for integer inequalities and number of required batches.
 * Usage: assert(ceilDiv(17LL,5LL) == 4);
 *  assert(floorDiv(-7LL,3LL) == -3);
 */
#pragma once

template<class T> T floorDiv(T a, T b) {
	T q = a / b, r = a % b;
	if (r != 0 && ((r < 0) != (b < 0))) --q;
	return q;
}

template<class T> T ceilDiv(T a, T b) {
	T q = a / b, r = a % b;
	if (r != 0 && ((r < 0) == (b < 0))) ++q;
	return q;
}
