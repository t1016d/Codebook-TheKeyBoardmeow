#include <stdio.h>
#include <math.h>

#include <deque>
#include <vector>
#include <algorithm>

#define PB push_back
#define MP make_pair
#define F first
#define S second
#define SZ(x) ((int)(x).size())
#define ALL(x) (x).begin(), (x).end()

const double eps = 1e-9;
const double inf = 1.0/0.0;

using namespace std;
typedef long long ll;
typedef pair<int,int> PII;
typedef vector<int> VI;

const double PI = acos(-1);
const double INF = 1e18;
const double EPS = 1e-8;

struct node {
	double x,y;
	node(double _x=0, double _y=0) : x(_x),y(_y) {}
	node operator+(const node& rhs) const
		{ return node(x+rhs.x, y+rhs.y); }
	node operator-(const node& rhs) const
		{ return node(x-rhs.x, y-rhs.y); }
	node operator*(const double& rhs) const
		{ return node(x*rhs, y*rhs); }
	node operator/(const double& rhs) const
		{ return node(x/rhs, y/rhs); }
	double operator*(const node& rhs) const
		{ return x*rhs.x+y*rhs.y; }
	double operator^(const node& rhs) const
		{ return x*rhs.y-y*rhs.x; }
	double len2() const { return x*x+y*y; }
	double len() const { return sqrt(x*x+y*y); }
	node unit() const { return *this/len(); }
	node T() const { return node(-y,x); } // counter-clockwise
	node TR() const { return node(y,-x); } // clockwise
	node rot(double rad) const { // rotate counter-clockwise in rad
		return node(cos(rad)*x-sin(rad)*y, sin(rad)*x+cos(rad)*y);
	}
};
typedef pair<node,node> Line; // pos, way


node inter(const node &p1, const node &v1, const node &p2, const node &v2) // intersection
{
	//if(fabs(v1^v2) < EPS)
	//	return node(INF, INF);
	double k = ((p2-p1)^v2) / (v1^v2);
	return p1 + v1*k;
}
node inter(const Line& L1, const Line& L2){
	return inter(L1.F, L1.S, L2.F, L2.S);
}

bool isin(Line L, node p)
{
	return (L.S^(p-L.F))>=0;
}

bool cmp(Line a, Line b)
{
	return atan2(a.S.y, a.S.x) < atan2(b.S.y, b.S.x);
}

vector<Line> halfplane(vector<Line> lines) {
	sort(lines.begin(), lines.end(), cmp);
    deque<Line> dq;
    for (int i=0; i<SZ(lines); i++) {
        while(SZ(dq) >= 2 && !isin(lines[i], inter(dq[SZ(dq)-2], dq[SZ(dq)-1])))
            dq.pop_back();
        while(SZ(dq) >= 2 && !isin(lines[i], inter(dq[0], dq[1])))
            dq.pop_front();
        dq.push_back(lines[i]);
    }
    while (SZ(dq) >= 3 && !isin(dq[0], inter(dq[SZ(dq)-2], dq[SZ(dq)-1])))
        dq.pop_back();
    while (SZ(dq) >= 3 && !isin(dq[SZ(dq)-1], inter(dq[0], dq[1])))
        dq.pop_front();
    vector<Line> res(SZ(dq));
    copy(ALL(dq), res.begin());
    return res;
}

node arr[505];

int main()
{
	int cas;
	scanf("%d",&cas);
	while(cas--){
		int n;
		double w, h;
		scanf("%d %lf %lf",&n, &w, &h);
		for(int i=0; i<n; i++){
			scanf("%lf %lf",&arr[i].x, &arr[i].y);
		}
		for(int per=0; per<n; per++){
			node tar = arr[per];
			double tans=0;
			{
				vector<Line> vec;
				vec.PB(MP(node(0,0),node(1,0)));
				vec.PB(MP(node(w,0),node(0,1)));
				vec.PB(MP(node(w,h),node(-1,0)));
				vec.PB(MP(node(0,h),node(0,-1)));
				for(int i=0; i<n; i++){
					if(i==per) continue;
					node con = arr[i];
					vec.PB(MP((con+tar)/2, (con-tar).T()));
				}
				vec = halfplane(vec);
				vec.PB(vec[0]);
				vec.PB(vec[1]);
				double ans = 0;
				for(int i=1; i<SZ(vec)-1; i++){
					ans += (inter(vec[i-1], vec[i])^inter(vec[i], vec[i+1]))/2;
				}
				tans += ans;
				//printf("%f\n",ans);
			}
			{
				vector<node> vec[2];
				vec[0].PB(node(0,0));
				vec[0].PB(node(w,0));
				vec[0].PB(node(w,h));
				vec[0].PB(node(0,h));
				int now=0, nxt=1;
				for(int i=0; i<n; i++){
					if(i==per) continue;
					vec[nxt].clear();
					node con = arr[i];
					node pos = (con + tar)/2;
					node way = (con - tar).T();
					node nor = (tar - con);
					vec[now].PB(vec[now][0]);
					for(int j=0; j<SZ(vec[now])-1; j++){
						if(nor*vec[now][j] >= nor*pos && nor*vec[now][j+1] >= nor*pos){
							vec[nxt].PB(vec[now][j]);
						}else if(nor*vec[now][j] >= nor*pos && nor*vec[now][j+1] < nor*pos){
							vec[nxt].PB(vec[now][j]);
							node tmp = inter(vec[now][j], vec[now][j+1]-vec[now][j], pos, way);
							vec[nxt].PB(tmp);
						}else if(nor*vec[now][j] < nor*pos && nor*vec[now][j+1] >= nor*pos){
							node tmp = inter(vec[now][j], vec[now][j+1]-vec[now][j], pos, way);
							vec[nxt].PB(tmp);
						}
					}
					now^=1, nxt^=1;
				}
				vec[now].PB(vec[now][0]);
				double ans = 0;
				for(int i=0; i<SZ(vec[now])-1; i++){
					ans += (vec[now][i]^vec[now][i+1])/2;
				}
				tans-=ans;
				printf("%f\n",ans);
			}
			if(fabs(tans)>eps){
				if(tans>0)
					while(tans);
				else{
					printf("wa\n");
				}
			}
		}
	}
	return 0;
}
