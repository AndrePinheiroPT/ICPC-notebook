#include "prelude.hpp"
using namespace std;
using ll = long long;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wconversion"
#include "../../content/geometry/Point.h"
#pragma GCC diagnostic pop
#include "../../content/geometry/NearestPair.h"
#include "../../content/geometry/RectUnion.h"

using W = __int128;
mt19937_64 rng(0x8c105e57);
size_t nearestCases = 0, rectangleCases = 0;
size_t enumeratedPairs = 0, checkedCells = 0;
#ifdef _GLIBCXX_DEBUG
// Debug lower_bound checks scan the full compressed coordinate range.
constexpr int stressN = 2000;
#else
constexpr int stressN = 100000;
#endif
int rnd(int n) { return int(rng() % unsigned(n)); }
ll wide() {
	return ll(rng()%200000001ULL)-100000000LL;
}

template<class T>
auto squared(Point<T> a, Point<T> b) {
	using R = conditional_t<is_integral_v<T>,__int128_t,T>;
	R dx = R(a.x)-R(b.x), dy = R(a.y)-R(b.y);
	return dx*dx+dy*dy;
}

template<class T>
void checkNearest(const vector<Point<T>>& p) {
	++nearestCases;
	auto copy = p;
	auto [actual,ids] = nearestPair(p);
	assert(p == copy);
	if (sz(p) < 2) {
		assert(actual == 0 && ids == make_pair(-1,-1));
		return;
	}
	auto expected = squared(p[0],p[1]);
	for (int i = 0; i < sz(p); ++i)
		for (int j = i+1; j < sz(p); ++j) {
			expected = min(expected,squared(p[i],p[j]));
			++enumeratedPairs;
		}
	assert(actual == expected);
	auto [a,b] = ids;
	assert(0 <= a && a < sz(p));
	assert(0 <= b && b < sz(p) && a != b);
	assert(squared(p[a],p[b]) == actual);
}

// Independent grid-cell oracle: boundaries partition the plane;
// inspect each cell against every original rectangle.
template<class T>
W cellsArea(const vector<array<T,4>>& rect) {
	vector<T> x,y;
	for (auto [x1,y1,x2,y2] : rect) {
		x.push_back(x1); x.push_back(x2);
		y.push_back(y1); y.push_back(y2);
	}
	sort(x.begin(),x.end()); sort(y.begin(),y.end());
	x.erase(unique(x.begin(),x.end()),x.end());
	y.erase(unique(y.begin(),y.end()),y.end());
	W result = 0;
	for (int i = 1; i < sz(x); ++i)
		for (int j = 1; j < sz(y); ++j) {
			bool covered = false;
			for (auto [x1,y1,x2,y2] : rect)
				covered |= x1 <= x[i-1] && x[i] <= x2 &&
					y1 <= y[j-1] && y[j] <= y2;
			if (covered)
				result += (W(x[i])-x[i-1])*(W(y[j])-y[j-1]);
			++checkedCells;
		}
	return result;
}

void checkRect(const vector<array<ll,4>>& rect, bool raster = false) {
	++rectangleCases;
	auto copy = rect;
	W actual = rectUnion(rect), expected = cellsArea(rect);
	assert(rect == copy && actual == expected);
	if (raster) {
		set<pair<int,int>> filled;
		for (auto [x1,y1,x2,y2] : rect)
			for (int x = int(x1); x < int(x2); ++x)
				for (int y = int(y1); y < int(y2); ++y)
					filled.insert({x,y});
		assert(actual == W(filled.size()));
	}
}

void smallNearest() {
	vector<Point<ll>> lattice;
	for (int x = -1; x <= 1; ++x)
		for (int y = -1; y <= 1; ++y) lattice.emplace_back(x,y);
	for (int mask = 0; mask < (1 << sz(lattice)); ++mask) {
		vector<Point<ll>> p;
		for (int i = 0; i < sz(lattice); ++i)
			if ((mask >> i)&1) p.push_back(lattice[i]);
		checkNearest(p);
		if (!p.empty()) {
			p.push_back(p[rnd(sz(p))]);
			shuffle(p.begin(),p.end(),rng);
			checkNearest(p);
		}
	}
	for (int run = 0; run < 6000; ++run) {
		int n = rnd(51);
		vector<Point<ll>> p;
		for (int i = 0; i < n; ++i) {
			int x = rnd(101)-50, y = rnd(101)-50;
			if (run%5 == 0) x = 0;
			if (run%5 == 1) y = 0;
			if (run%5 == 2) y = x;
			p.emplace_back(x,y);
		}
		checkNearest(p);
		vector<Point<ll>> shifted;
		ll dx = wide(), dy = wide();
		for (auto v : p) shifted.emplace_back(dx+v.x,dy+v.y);
		checkNearest(shifted);
	}
	for (int run = 0; run < 2000; ++run) {
		int n = rnd(31);
		vector<Point<ll>> p;
		for (int i = 0; i < n; ++i) p.emplace_back(wide(),wide());
		checkNearest(p);
	}
	for (int run = 0; run < 1000; ++run) {
		int n = rnd(31);
		vector<P2> p;
		for (int i = 0; i < n; ++i)
			p.emplace_back(ld(rnd(201)-100)/8,ld(rnd(201)-100)/8);
		checkNearest(p);
	}
	constexpr ll b = 100000000LL;
	checkNearest(vector<Point<ll>>{Point<ll>(-b,-b),Point<ll>(b,b)});
	checkNearest(vector<Point<ll>>{Point<ll>(-b,-b),Point<ll>(b,b),Point<ll>(-b,b)});
	checkNearest(vector<Point<ll>>{Point<ll>(-1000000000,-1000000000),
		Point<ll>(1000000000,1000000000)});
}

void smallRectangles() {
	checkRect(vector<array<ll,4>>{});
	vector<array<ll,4>> types;
	for (int x1 = -1; x1 <= 1; ++x1)
		for (int x2 = x1; x2 <= 1; ++x2)
			for (int y1 = -1; y1 <= 1; ++y1)
				for (int y2 = y1; y2 <= 1; ++y2)
					types.push_back({x1,y1,x2,y2});
	for (int a = 0; a < sz(types); ++a) {
		checkRect(vector{types[a]},true);
		for (int b = a; b < sz(types); ++b) {
			checkRect(vector{types[a],types[b]},true);
			for (int c = b; c < sz(types); ++c)
				checkRect(vector{types[a],types[b],types[c]},true);
		}
	}
	for (int run = 0; run < 3000; ++run) {
		int n = rnd(13);
		vector<array<ll,4>> rect;
		for (int i = 0; i < n; ++i) {
			int x1 = rnd(13)-6, x2 = rnd(13)-6;
			int y1 = rnd(13)-6, y2 = rnd(13)-6;
			if (x1 > x2) swap(x1,x2);
			if (y1 > y2) swap(y1,y2);
			rect.push_back({x1,y1,x2,y2});
			if (rnd(5) == 0) rect.push_back(rect.back());
		}
		checkRect(rect,true);
		vector<array<ll,4>> shifted;
		ll dx = wide(), dy = wide();
		for (auto [x1,y1,x2,y2] : rect)
			shifted.push_back({dx+x1,dy+y1,dx+x2,dy+y2});
		checkRect(shifted);
	}
	for (int run = 0; run < 2000; ++run) {
		int n = rnd(9);
		vector<array<ll,4>> rect;
		for (int i = 0; i < n; ++i) {
			ll x1 = wide(), x2 = wide(), y1 = wide(), y2 = wide();
			if (x1 > x2) swap(x1,x2);
			if (y1 > y2) swap(y1,y2);
			rect.push_back({x1,y1,x2,y2});
		}
		checkRect(rect);
	}
	checkRect(vector<array<ll,4>>{{-100000000LL,-1,100000000LL,1},
		{-100000000LL+1,0,100000000LL-1,4}});
	checkRect(vector<array<ll,4>>{{-1,-100000000LL,1,100000000LL},
		{0,-100000000LL+1,4,100000000LL-1}});
	checkRect(vector<array<ll,4>>{{-100000000LL,-100000000LL,
		-100000000LL,100000000LL},{-100000000LL,100000000LL,100000000LL,100000000LL}});
}

void largeCases() {
	constexpr int n = stressN;
	vector<Point<ll>> p;
	for (int i = 0; i < n; ++i) p.emplace_back(0,100LL*i);
	shuffle(p.begin(),p.end(),rng);
	auto line = nearestPair(p);
	assert(line.first == 10000);
	assert(squared(p[line.second.first],p[line.second.second]) == 10000);
	p.clear();
	for (int i = 0; i < 400; ++i)
		for (int j = 0; j < 250; ++j) p.emplace_back(3LL*i,3LL*j);
	shuffle(p.begin(),p.end(),rng);
	auto grid = nearestPair(p);
	assert(grid.first == 9);
	assert(squared(p[grid.second.first],p[grid.second.second]) == 9);
	p.assign(n,Point<ll>(17,-35));
	auto duplicate = nearestPair(p);
	assert(duplicate.first == 0 && duplicate.second.first != duplicate.second.second);
	vector<array<ll,4>> r;
	for (int i = 0; i < n; ++i) r.push_back({3LL*i,-1,3LL*i+2,2});
	assert(rectUnion(r) == W(6)*n);
	r.clear();
	for (int i = 0; i < n; ++i) r.push_back({0,2LL*i,1,2LL*i+1});
	assert(rectUnion(r) == n);
	r.clear();
	for (int i = 0; i < n; ++i) r.push_back({-ll(i),-ll(i),i,i});
	assert(rectUnion(r) == W(4)*(n-1)*(n-1));
	r.assign(n,{-10,-30,20,50});
	assert(rectUnion(r) == 2400);
}

int main() {
	smallNearest(); smallRectangles(); largeCases();
	cout << "Spatial verified: " << nearestCases << " nearest-pair cases ("
		<< enumeratedPairs << " brute pairs), " << rectangleCases
		<< " rectangle-union cases (" << checkedCells << " brute cells)"
		<< "; plus 100000-point grid and " << stressN
		<< "-point/rectangle structural cases\n";
}
