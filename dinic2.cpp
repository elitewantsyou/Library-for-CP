namespace Dinic {

	using F = int;
	const F INF = 1e9+1e8;

	const int N = 220000;
	const int M = 1100000;

	int fst[N], nxt[M], to[M];
	F cap[M];
	int dis[N], q[N], ptr[N];
	int E;

	void init() {
		memset(fst, -1, sizeof fst);
		E = 0;
	}
	inline void add_edge(int u, int v, F c) {
		to[E] = v, cap[E] = c, nxt[E] = fst[u], fst[u] = E++;
		to[E] = u, cap[E] = 0, nxt[E] = fst[v], fst[v] = E++;
	}
	inline bool bfs(int S, int T, int n) {
		memset(dis, -1, sizeof(int) * n);
		int h = 0, t = 0;
		dis[S] = 0, q[t++] = S;
		while (h < t) {
			int u = q[h++];
			for (int e = fst[u]; ~e; e = nxt[e]) if (cap[e] > 0 && dis[to[e]] == -1) {
				dis[to[e]] = dis[u] + 1, q[t++] = to[e];
				if (to[e] == T) return 1;
			}
		}
		return (dis[T] != -1);
	}
	F dfs(int u, int T, F f) {
		if (u == T) return f;
		for (int &e = ptr[u]; ~e; e = nxt[e]) if (cap[e] > 0 && dis[to[e]] > dis[u]) {
			F ret = dfs(to[e], T, min(f, cap[e]));
			if (ret > 0) {
				cap[e] -= ret, cap[e ^ 1] += ret;
				return ret;
			}
		}
		return 0;
	}
	F max_flow(int S, int T, int n) {
		F ret = 0;
		while (bfs(S, T, n)) {
			memcpy(ptr, fst, sizeof(int) * n);
			for (F cur; (cur = dfs(S, T, INF)) > 0; ret += cur);
		}
		return ret;
	}
};
