/**
 * Author: jet notebook
 * Summary: Ordered set with ranks and kth-smallest selection.
 * Description: \begin{itemize}[nosep,leftmargin=*]
 *  \item Use when insert/erase must coexist with
 *  ranks or kth-smallest queries. OST<int> stores unique
 *  ints in increasing order; insert(x), erase(x) work
 *  like set. \texttt{order\_of\_key(x)} counts values <x.
 *  \item \texttt{find\_by\_order(k)} returns an iterator
 *  to the 0-based kth value; check against end() before
 *  reading *it. For duplicates, store pair<T,int> with a
 *  unique id. Custom comparator C must be strict.
 *  \item Operations $O(\log n)$, space $O(n)$.
 *  \end{itemize}
 * Usage: OST<int> s; s.insert(7); s.insert(3);
 *  auto it = s.find_by_order(0); // *it = 3
 */
#pragma once

#include <ext/pb_ds/assoc_container.hpp> /** keep-include */
#include <ext/pb_ds/tree_policy.hpp> /** keep-include */

template<class T, class C = less<T>>
using OST = __gnu_pbds::tree<T,
	__gnu_pbds::null_type, C,
	__gnu_pbds::rb_tree_tag,
	__gnu_pbds::tree_order_statistics_node_update>;
