/**
 * Author: Andre notebook; team adaptation
 * Summary: Representable XORs of subsets, in 64 bits.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item add(x) inserts an unsigned 64-bit value; returns true iff it
 *  increases rank. contains(x) tests whether some subset XOR equals x.
 *  maximum(x=0) returns the largest unsigned x XOR subset-XOR.
 *  To merge bases, add each nonzero pivot of the other basis.
 *  \item pivot[b] has highest set bit b; eliminate highest bits first.
 *  rank independent values generate $2^{rank}$ distinct XORs (do not
 *  evaluate 1ULL<<rank when rank=64). Does not reconstruct the subset.
 *  \item $O(64)$ per operation, $O(64)$ space; all 64 bits supported.
 *  \end{itemize}
 * Usage: XorBasis b; b.add(3); b.add(5);
 *  bool yes=b.contains(6); uint64_t best=b.maximum(); // true,6
 */
#pragma once

struct XorBasis {
	array<uint64_t,64> pivot{};
	int rank = 0;
	bool add(uint64_t x) {
		for (int b = 63; b >= 0; --b) if ((x>>b)&1) {
			if (!pivot[b]) { pivot[b] = x; ++rank; return true; }
			x ^= pivot[b];
		}
		return false;
	}
	bool contains(uint64_t x) const {
		for (int b = 63; b >= 0; --b) if ((x>>b)&1) x ^= pivot[b];
		return x == 0;
	}
	uint64_t maximum(uint64_t x = 0) const {
		for (int b = 63; b >= 0; --b) x = max(x,x^pivot[b]);
		return x;
	}
};
