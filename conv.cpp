//https://codeforces.com/blog/entry/72488

#include <bits/stdc++.h>

#define fi first
#define se second

const int N = 100100;

using namespace std;

vector < int > zeta(vector < int > a, int n)
{
        vector < int > res = a;
        for(int i = 0; i < n; i++){
                for(int j = 0; j < (1 << n); j++){
                        if(j & (1 << i)){
                                res[j] += res[j ^ (1 << i)];
                        }
                }
        }
        return res;
}

// inverse of zeta
vector < int > mobius(vector < int > a, int n)
{
        vector < int > res = a;
        for(int i = 0; i < n; i++){
                for(int j = 0; j < (1 << n); j++){
                        if(j & (1 << i)){
                                res[j] -= res[j ^ (1 << i)];
                        }
                }
        }
        return res;
}

vector < int > conv(vector < int > a, vector < int > b, int n)
{
        vector < int > res(1 << n, 0);
        vector < vector < int > > A(n + 1, vector < int > (1 << n, 0));
        vector < vector < int > > B(n + 1, vector < int > (1 << n, 0));
        vector < vector < int > > C(n + 1, vector < int > (1 << n, 0));

        for(int i = 0; i < (1 << n); i++){
                A[__builtin_popcount(i)][i] = a[i];
                B[__builtin_popcount(i)][i] = b[i];
        }

        for(int i = 0; i <= n; i++){
                A[i] = zeta(A[i], n);
                B[i] = zeta(B[i], n);
        }

        for(int i = 0; i < (1 << n); i++){
                for(int j = 0; j <= n; j++){
                        for(int h = 0; h <= j; h++){
                                C[j][i] += A[h][i] * B[j - h][i];
                        }
                }
        }

        for(int i = 0; i <= n; i++){
                C[i] = mobius(C[i], n);
        }

        for(int i = 0; i < (1 << n); i++){
                res[i] = C[__builtin_popcount(i)][i];
        }
        return res;
}

int main()
{
        ios_base::sync_with_stdio(0);

        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);

        int n = 10;
        vector < int > a(1 << n);
        vector < int > b(1 << n);
        for(int i = 0; i < (1 << n); i++){
                a[i] = rand() % 1000;
                b[i] = rand() % 1000;
        }

        auto res = conv(a, b, n);
        for(int i = 0; i < (1 << n); i++){
                int cnt = 0;
                for(int j = 0; j < (1 << n); j++){
                        if((j & i) == j){
                                cnt += a[j] * b[i ^ j];
                        }
                }
                assert(cnt == res[i]);
        }
}
