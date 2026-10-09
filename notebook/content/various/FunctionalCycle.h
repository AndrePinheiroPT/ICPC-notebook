/**
 * Author: jet notebook
 * Summary: Floyd: cycle entry distance and cycle length.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item A deterministic process eventually repeats: find how many
 *  steps before the cycle and how long one lap takes, without a visited array.
 *  \item floydCycle(start,step) returns (mu,len); step(state)
 *  must return the next state. Slow advances once, fast twice.
 *  Default equality is ==; optional same(a,b) must be an equivalence
 *  preserved by step; lengths are under same. No states returned.
 *  \item O(mu+len) calls, O(1) stored states; count state costs too.
 *  The process must become periodic and lengths must fit ll.
 *  \end{itemize}
 * Usage: auto [mu,len] = floydCycle(0,
 *  [&](int v){ return next[v]; });
 */
#pragma once

template<class T, class F,
	class Eq = equal_to<T>>
pair<ll,ll> floydCycle(T start, F step, Eq same = {}) {
	// Meet inside the cycle.
	T slow = step(start), fast = step(step(start));
	while (!same(slow,fast)) {
		slow = step(slow);
		fast = step(step(fast));
	}
	// Restart one pointer to find the cycle entry.
	ll mu = 0, len = 1;
	slow = start;
	while (!same(slow,fast)) {
		slow = step(slow);
		fast = step(fast);
		++mu;
	}
	// Walk once around the cycle.
	fast = step(slow);
	while (!same(slow,fast)) {
		fast = step(fast);
		++len;
	}
	return {mu,len};
}
