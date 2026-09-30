/**
 * Author: a
 * License: CC0
 * Source: Codeforces
 * Description: Consider an array $A$ with $2^n$ elements. Our goal is to calculate $F(x)$ for all $x=0,1,2,\cdots,2^n-1$ 
 * Here, $F(x)$ represents the sum of values in array $A$ for all subsets (submasks) $i$ of $x$. That is:
 * $F(x) = \sum_{i \in x} A[i]$. This is basically a prefix sum on $n$-dimensional space.
 * Time: O(N 2^N)
 * Status: it works
 */

sos = A;
for(int i = 0; i < n; i++){  // Sweep along the i-th axis
	for(int x = 0; x < (1<<n); x++){
		if(x & (1 << i))  // If the i-th bit is set, accumulate
			sos[x] += sos[x^(1 << i)];
	}
}