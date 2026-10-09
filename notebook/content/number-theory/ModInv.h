/**
 * Author: Team notebook; adapted from supplied KACTL notebooks
 * Summary: Modular division when an inverse exists.
 * Description: modInv(a,m), signed a and m>1, returns optional x in $[0,m)$ with ax=1 modulo m.
 *  Works for composite moduli too; nullopt when gcd(a,m)!=1. Test the optional before dereferencing.
 *  Extended Euclid with 128-bit intermediates: $O(\log m)$ time, $O(1)$ extra memory.
 * Usage: auto inv = modInv(3,7); // *inv=5; 4/3 mod 7 is 4*5 mod 7
 *  assert(!modInv(2,6));
 */
#pragma once

optional<ll> modInv(ll a, ll m) {
	assert(m > 1);
	a %= m;
	if (a < 0) a += m;
	ll r = m, next = a;
	__int128 x = 0, y = 1;
	while (next) {
		ll q = r / next;
		ll rem = r % next;
		r = next; next = rem;
		__int128 z = x - (__int128)q*y;
		x = y; y = z;
	}
	if (r != 1) return nullopt;
	x %= m;
	if (x < 0) x += m;
	return ll(x);
}
