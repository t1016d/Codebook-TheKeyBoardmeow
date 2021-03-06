const int MAXS = 2000050;
char S[MAXS * 2];

int main() {
	while( scanf("%s", S) == 1 ) {
		int len = strlen( S );
		for(int i = 0; i < len; i++)
			S[len + i] = S[i];
		int a = 0, b = 1;
		int k = 0;
		while( a < len && b < len ) {
			if( a == b ) a++;
			while( k < len && S[a + k] == S[b + k] ) k++;
			if(S[a + k] <= S[b + k]) b += k + 1;
			else a += k + 1;
			k = 0;
		}
		int res = ( a < len )? a : b;
		S[res + len] = '\0';
		puts( S + res );
	}
	return 0;
}
