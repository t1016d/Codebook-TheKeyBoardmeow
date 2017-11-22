typedef unsigned long long ull;
const int MAXN = 50 + 5;
int n;
int e[MAXN][MAXN];

int max_clique(ull clq, ull pos) {
	if(pos == 0ull) return __builtin_popcountll(clq);
	int res = 0;
	int u = -1;
	for(int i = 0; i < n; i++)
		if(pos & (1ull << i)) {
			u = i;
			break;
		}
	for(int i = u; i < n; i++) {
		if(i != u && e[i][u]) continue;
		if((pos & (1ull << i)) == 0) continue;
		pos &= ~(1ull << i);
		ull oripos = pos;
		ull oriclq = clq;
		clq |= (1ull << i);
		for(int j = 0; j < n; j++) {
			if(e[i][j] || (pos & (1ull << j)) == 0) continue;
			pos &= ~(1ull << j);
		}
		res = max(res, max_clique(clq, pos));
		pos = oripos;
		clq = oriclq;
	}
	return res;
}

int main() {
	while(scanf("%d", &n) == 1 && n) {
		for(int i = 0; i < n; i++)
			for(int j = 0; j < n; j++)
				scanf("%d", &e[i][j]);
		printf("%d\n", max_clique(0, (1ull << n) - 1));
	}
	return 0;
}
