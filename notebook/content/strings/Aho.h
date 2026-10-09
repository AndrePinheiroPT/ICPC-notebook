/**
 * Author: jet notebook
 * Summary: Aho--Corasick: many pattern matches in one text scan.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use to find many patterns while reading a text once.
 *  add(word) returns its ID; add all words, then build once.
 *  \textbf{Aho(K,first):} K bytes starting at first; default a-z.
 *  Aho a(256,0) accepts every byte.
 *  \item Start v=0; v=go(v,ch) reads one byte. count(v) returns
 *  patterns ending here; report(v,emit) calls emit(id)
 *  for each one. Duplicate words have distinct IDs. Empty
 *  words match every boundary, including v=0 before scanning.
 *  \item Node next stores transitions; link falls back to a suffix,
 *  out skips to a matching suffix; ids holds pattern IDs,
 *  cnt counts matches. Never add after build.
 *  \item K letters, V nodes, P patterns: add amortized $O(K(|s|+1))$;
 *  build/space $O(KV+P)$. go/count $O(1)$; report $O(1+out)$.
 *  Output lists are not copied along links; total counts use ll.
 *  \end{itemize}
 * Usage: Aho a; a.add("aba"); a.build();
 *  int v = 0; ll total = 0; // text: input string
 *  for (char c : text) { v=a.go(v,c); total+=a.count(v); }
 *  a.report(v,[&](int id) { cout << id << ' '; });
 */
#pragma once

struct Aho {
	int K, BASE;
	struct Node {
		vec<int> next;
		int link = 0, out = -1, cnt = 0;
		vec<int> ids;
		explicit Node(int k) : next(k,-1) {}
	};
	vec<Node> t;
	explicit Aho(int alphabet = 26,
		int first = 'a')
		: K(alphabet), BASE(first) {
		assert(BASE >= 0 && K > 0 && K <= 256-BASE);
		t.emplace_back(K);
	}
	int patterns = 0;
	bool built = false;
	int code(unsigned char ch) const {
		int c = int(ch)-BASE;
		assert(0 <= c && c < K); return c;
	}
	int add(string_view s) {
		assert(!built);
		int v = 0;
		for (unsigned char ch : s) {
			int c = code(ch), u = t[v].next[c];
			if (u < 0) {
				t[v].next[c] = u = sz(t);
				t.emplace_back(K);
			}
			v = u;
		}
		t[v].ids.pb(patterns);
		++t[v].cnt;
		return patterns++;
	}
	void build() {
		assert(!built); built = true;
		queue<int> q; q.push(0);
		while (!q.empty()) {
			int v = q.front(); q.pop();
			if (v) {
				int p = t[v].link;
				t[v].out = t[p].ids.empty() ?
					t[p].out : p;
				t[v].cnt += t[p].cnt;
			}
			rep(c,0,K) {
				int u = t[v].next[c];
				int w = v ? t[t[v].link].next[c] : 0;
				if (u < 0) t[v].next[c] = w;
				else { t[u].link = w; q.push(u); }
			}
		}
	}
	int go(int v, unsigned char ch) const {
		assert(built); return t[v].next[code(ch)];
	}
	int count(int v) const {
		assert(built); return t[v].cnt;
	}
	void report(int v, const function<void(int)>& emit) const {
		assert(built);
		for (int u = v; u >= 0; u = t[u].out)
			for (int id : t[u].ids) emit(id);
	}
};
