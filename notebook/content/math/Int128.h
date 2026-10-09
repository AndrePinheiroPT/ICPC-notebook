/**
 * Author: Team notebook; based on supplied notebooks
 * Summary: Read/print signed 128-bit integers exactly.
 * Description: toString(x) returns decimal text. read128(in,x) reads a signed integer;
 *  false for malformed input, overflow or EOF, leaving x unchanged.
 *  Cast BEFORE arithmetic: i128(a)*b, not i128(a*b). O(d) time/memory for d digits.
 * Usage: i128 x = i128(1000000000000LL)*1000000000000LL;
 *  cout << toString(x);
 */
#pragma once

using i128 = __int128_t;
using u128 = __uint128_t;

string toString(i128 x) {
	u128 v = x < 0 ? -u128(x) : u128(x);
	string s;
	do {
		s += char('0' + v % 10);
		v /= 10;
	} while (v);
	if (x < 0) s += '-';
	reverse(s.begin(), s.end());
	return s;
}

bool read128(istream& in, i128& x) {
	string s;
	if (!(in >> s)) return false;
	bool neg = s[0] == '-';
	int i = neg || s[0] == '+';
	if (i == sz(s)) return false;
	u128 v = 0, lim = (u128(1) << 127) - !neg;
	for (; i < sz(s); ++i) {
		int d = s[i] - '0';
		if (d < 0 || d > 9) return false;
		if (v > (lim - d) / 10) return false;
		v = v * 10 + d;
	}
	x = neg && v ? -i128(v - 1) - 1 : i128(v);
	return true;
}
