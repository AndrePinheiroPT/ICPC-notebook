/**
 * Author: supplied by user, adapted
 * Summary: Dictionary membership and prefix counts, with duplicates.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Trie accepts lowercase a-z words, including empty. insert(s) adds
 *  one copy; count(s) counts exact copies, contains(s) tests membership,
 *  countPrefix(s) counts all inserted words starting with s.
 *  \item Root=0. walk(s) returns its node ID, or -1 if absent. node[v].next
 *  contains child IDs (-1 absent); terminal counts word endings; pass
 *  counts words passing through. countPrefix("") counts all insertions.
 *  \item Dynamic nodes; insertion may invalidate references into node. No erase.
 *  Insertions/nodes $<INT\_MAX$. Calls $O(|s|+1)$ (insert amortised);
 *  space $O(26L)$ for L distinct prefixes plus root.
 *  \end{itemize}
 * Usage: Trie t; t.insert("cat"); t.insert("cat");
 *  int copies=t.count("cat"); // 2
 */
#pragma once

struct Trie {
	struct Node {
		array<int,26> next;
		int terminal = 0, pass = 0;
		Node() { next.fill(-1); }
	};
	vec<Node> node = vec<Node>(1);
	void insert(const string& s) {
		assert(node[0].pass < INT_MAX);
		int v = 0; ++node[v].pass;
		for (char c : s) {
			int x = c-'a'; assert(0 <= x && x < 26);
			if (node[v].next[x] == -1) {
				assert(node.size() < INT_MAX);
				int id = sz(node); node.emplace_back();
				node[v].next[x] = id;
			}
			v = node[v].next[x]; ++node[v].pass;
		}
		++node[v].terminal;
	}
	int walk(const string& s) const {
		int v = 0;
		for (char c : s) {
			int x = c-'a'; assert(0 <= x && x < 26);
			v = node[v].next[x];
			if (v == -1) break;
		}
		return v;
	}
	int count(const string& s) const {
		int v = walk(s); return v == -1 ? 0 : node[v].terminal;
	}
	bool contains(const string& s) const { return count(s) != 0; }
	int countPrefix(const string& s) const {
		int v = walk(s); return v == -1 ? 0 : node[v].pass;
	}
};
