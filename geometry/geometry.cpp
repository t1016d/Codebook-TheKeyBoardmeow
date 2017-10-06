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

node __mirror(node normal, double constant, node point){ //2D3D
	double scale=(normal*point+constant)/(normal*normal);
	return point-normal*(2*scale);
}
node mirror(node p1, node p2, node p3){ // 2D3D
	return __mirror((p2-p1).T(), (p2-p1).T()*p1*(-1), p3);
}
double ori(const node& p1, const node& p2, const node& p3){ //平行四邊形面積(帶正負)
	return (p2-p1)^(p3-p1);
}
bool intersect(const node& p1, const node& p2, const node& p3, const node& p4){
	return (ori(p1,p2,p3)*ori(p1,p2,p4)<0 && ori(p3,p4,p1)*ori(p3,p4,p2)<0);
}
pair<node,node> two_circle_intersect(node p1, double r1, node p2, double r2){
	double degree=acos(((p2-p1).len2()+r1*r1-r2*r2)/(2*r1*(p2-p1).len()));
	return make_pair(p1+(p2-p1).unit().rot(degree)*r1, p1+(p2-p1).unit().rot(-degree)*r1);
}
node intersectionPoint(node p1, node p2, node p3, node p4){
	double a123 = (p2-p1)^(p3-p1);
	double a124 = (p2-p1)^(p4-p1);
	return (p4*a123-p3*a124)/(a123-a124);
}
node inter(const node &p1, const node &v1, const node &p2, const node &v2) // intersection
{
	if(fabs(v1^v2) < EPS)
		return node(INF, INF);
	double k = ((p2-p1)^v2) / (v1^v2);
	return p1 + v1*k;
}
void CircleInter(node o1, double r1, node o2, double r2) {
	if(r2>r1)
		swap(r1, r2), swap(o1, o2);
	double d = (o2-o1).len();
	node v = (o2-o1).unit();
	node t = v.TR();

	double area;
	vector<node> pts;
	if(d > r1+r2+EPS)
		area = 0;
	else if(d < r1-r2)
		area = r2*r2*PI;
	else if(r2*r2+d*d > r1*r1){
		double x = (r1*r1 - r2*r2 + d*d) / (2*d);
		double th1 = 2*acos(x/r1), th2 = 2*acos((d-x)/r2);
		area = (r1*r1*(th1 - sin(th1)) + r2*r2*(th2 - sin(th2))) / 2;
		double y = sqrt(r1*r1 - x*x);
		pts.PB(o1 + v*x + t*y), pts.PB(o1 + v*x - t*y);
	} else {
		double x = (r1*r1 - r2*r2 - d*d) / (2*d);
		double th1 = acos((d+x)/r1), th2 = acos(x/r2);
		area = r1*r1*th1 - r1*d*sin(th1) + r2*r2*(PI-th2);
		double y = sqrt(r2*r2 - x*x);
		pts.PB(o2 + v*x + t*y), pts.PB(o2 + v*x - t*y);
	}
	//Area: area
	//Intersections: pts
}
