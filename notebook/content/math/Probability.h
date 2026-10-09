/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Probabilities for independent repeated trials.
 * Description: binomialProb(n,p) returns n+1 probabilities: entry k is exactly k successes.
 *  Sum entries for an interval. $O(n^2)$ time, O(n) memory; n>=0 must be moderate.
 *  atLeastOne(n,p) returns $1-(1-p)^n$ stably; geometricProb(k,p) returns probability of first success at k>=1.
 *  Both O(1). Fixed p in [0,1], independent trials. Long double is approximate;
 *  extremely small probabilities may underflow. p=0/1 and n=0 are handled.
 * Usage: auto prob = binomialProb(4,0.5L); // prob[2]=0.375
 *  assert(abs(atLeastOne(4,0.5L)-0.9375L) < 1e-15L);
 */
#pragma once

vec<long double> binomialProb(int n, long double p) {
	assert(n >= 0 && 0 <= p && p <= 1);
	vec<long double> prob((size_t)n + 1);
	prob[0] = 1;
	for (int i = 0; i < n; ++i) {
		// Descer: prob[k-1] ainda tem de descrever os i tiros anteriores.
		for (int k = i+1; k >= 1; --k)
			prob[k] = prob[k]*(1-p) + prob[k-1]*p;
		prob[0] *= 1-p;
	}
	return prob;
}

long double atLeastOne(long long n, long double p) {
	assert(n >= 0 && 0 <= p && p <= 1);
	if (n == 0 || p == 0) return 0;
	if (p == 1) return 1;
	// Forma estável de 1-(1-p)^n; condições evitam log(0).
	return -expm1l(n*log1pl(-p));
}

long double geometricProb(long long k, long double p) {
	assert(k >= 1 && 0 <= p && p <= 1);
	if (k == 1) return p;
	if (p == 0 || p == 1) return 0;
	return p*expl((k-1)*log1pl(-p));
}
