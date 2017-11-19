#include <iostream>
#include <algorithm>
#include <windows.h>
#include <map>

using namespace std;

using point = pair<uint32_t ,uint32_t > ;

class Solution {
	uint32_t  size_1,size_2;
	uint32_t * delta,*s1,*s2;
	uint32_t ** alpha,**subVal;
	uint32_t num,min_cost;
	uint32_t MAX = 0xFFFFFFFF;
	uint32_t ** rec;
	multimap<uint32_t,point> previous;
public:
	Solution(uint32_t n);
	uint32_t getVal(bool choose);           //0 for dp, 1 for shortest path;
private:
	uint32_t helper(uint32_t a,uint32_t b); //help do dp by recursion
	void Minimum_Cost();                    //dynamic program
	void Short_Path();                      //shortest path algorithm
	void dij(uint32_t u,uint32_t v);        //dijkstra's algorithm
};

Solution::Solution(uint32_t n) :num(n),delta(new uint32_t[n]) { //get data and init
	alpha = new uint32_t*[n];
	for (uint32_t i = 0 ; i != num ; ++i) {
		cin >> delta[i];
		alpha[i] = new uint32_t[n];
	}
	//cout << "1" << endl;
	for (uint32_t i = 0 ; i != num ; ++i) {
		for (uint32_t j = 0 ; j != num ; ++j) {
			cin >> alpha[i][j];
		}
	}
	//cout << "2" << endl;
	cin >> size_1;
	s1 = new uint32_t[size_1+1];
	for (uint32_t  i = 1 ; i != size_1 + 1 ; ++ i) {
		cin >> s1[i];
	}
	cin >> size_2;
	//cout << "3" << endl;
	s2 = new uint32_t[size_2 + 1];
	for (uint32_t  i = 1 ; i != size_2 + 1 ; ++ i) {
		cin >> s2[i];
	}
	//cout << "4" <<endl;
	subVal = new uint32_t*[size_1+1];
	rec = new uint32_t*[size_1+1];
	subVal[0] = new uint32_t[size_2+1];
	rec[0] = new uint32_t[size_2+1];
	for (uint32_t  i = 1 ; i != size_1 + 1 ; ++ i) {
		subVal[i] = new uint32_t[size_2 + 1];
		rec[i] = new uint32_t[size_2+1];
		for (uint32_t j = 1 ; j != size_2 + 1 ; ++j) {
			subVal[i][j] = MAX;
		}
	}
	//cout << "5" << endl;
	uint32_t count = 0;
	for(uint32_t  i = 1; i != size_1 + 1 ; ++ i) {
		count += delta[s1[i]];
		subVal[i][0] = count;
	}
	count = 0;
	//cout << "6" << endl;
	for(uint32_t  i = 1 ; i != size_2 + 1; ++ i) {
		count += delta[s2[i]];
		subVal[0][i] = count;
	}
	//cout << "7" << endl;
}

void Solution::Minimum_Cost() {
	//min_cost = helper(size_1 ,size_2 );
	for (uint32_t  i = 1 ; i != size_1 + 1; ++i) {          //compute sub solution from bottom to top
		for (uint32_t  j = 1 ; j != size_2 + 1; ++j) {
			uint32_t opt_1,opt_2,opt_3,tmp;
			opt_1 = subVal[i-1][j-1] + alpha[s1[i]][s2[j]];
			opt_2 = subVal[i-1][j] + delta[s1[i]];
			opt_3 = subVal[i][j-1] + delta[s2[j]];
			tmp = min(opt_1,opt_2);
			subVal[i][j] = min(tmp,opt_3);
		}
	}
	min_cost = subVal[size_1][size_2];
}

uint32_t Solution::getVal(bool choose) {                    //get the best value
	if (choose==0)
		Minimum_Cost();
	else
		Short_Path();
	return min_cost;
}

void Solution::Short_Path() {
	for (uint32_t  i = 0 ; i != size_1 + 1 ; ++ i) {
		for (uint32_t j = 0 ; j != size_2 + 1 ; ++j) {
			subVal[i][j] = MAX;
			rec[i][j]=0;
		}
	}
	subVal[0][0] = 0;
	previous.insert({0,point(0,0)});                        //previous is a multimap of edges with limited weight
	dij(0,0);                                               //where key = weight, value = point(x,y)
	min_cost = subVal[size_1][size_2];                      //and on initial state, only {0,point(0,0)} is in it.
}

uint32_t Solution::helper(uint32_t a, uint32_t b) {         //compute the solution from top to bottom, but a bit slower
	if (subVal[a][b] != MAX) {
		//cout << subVal[a][b] << endl ;
		return subVal[a][b];
	}

	uint32_t opt_1,opt_2,opt_3,tmp;
	opt_1 = helper(a-1,b-1) + alpha[s1[a]][s2[b]];          //use three recursion
	opt_2 = helper(a-1,b) + delta[s1[a]];
	opt_3 = helper(a,b-1) + delta[s2[b]];
	tmp = min(opt_1,opt_2);
	//cout << subVal[a][b] << " ";
	subVal[a][b] = min(tmp,opt_3);
	//cout << subVal[a][b] << " " << endl;
}

void Solution::dij(uint32_t u,uint32_t v) {                 //a implement of dijkstra's with a bit difference
	uint32_t opt_1,opt_2,opt_3;                             //based on this special graph
	if (v == size_2 && u == size_1 ) return;                //if point(u,v) is the destination, return.
	else if (v == size_2) {                                 //consider the points on the top line
		opt_1 = subVal[u][v] + delta[s1[u+1]];
		if (opt_1 < subVal[u+1][v]) {
			subVal[u+1][v] = opt_1;
			if(!rec[u+1][v]) previous.insert({opt_1,point(u+1,v)});
		}
	}
	else if (u == size_1) {                                 //consider the points on the most right line
		opt_2 = subVal[u][v] + delta[s2[v+1]];
		if (opt_2 < subVal[u][v+1]) {
			subVal[u][v+1] = opt_2;
			if(!rec[u][v+1]) previous.insert({opt_2,point(u,v+1)});
		}
	}
	else if(u!=size_1 && v!= size_2) {                      //for each point such as p(u,v) , only three out_edge
		uint32_t tmp1, tmp2, tmp3;                          //for each out_edge (p,q) , update the shortest distance
		tmp1 = subVal[u + 1][v];                            //from s to q, then if (p,q) is the edge with min wight
		tmp2 = subVal[u][v + 1];                            //in previous ,choose q as next source point to run the dij
		tmp3 = subVal[u + 1][v + 1];
		opt_1 = subVal[u][v] + delta[s1[u + 1]];
		opt_2 = subVal[u][v] + delta[s2[v + 1]];
		opt_3 = subVal[u][v] + alpha[s1[u + 1]][s2[v + 1]];
		if (opt_1 < tmp1) {
			subVal[u + 1][v] = opt_1;
			if (!rec[u+1][v]) previous.insert({opt_1, point(u + 1, v)});
		}
		if (opt_2 < tmp2) {
			subVal[u][v + 1] = opt_2;
			if (!rec[u][v+1]) previous.insert({opt_2, point(u, v + 1)});
		}
		if (opt_3 < tmp3) {
			subVal[u + 1][v + 1] = opt_3;
			if (!rec[u+1][v+1]) previous.insert({opt_3, point(u + 1, v + 1)});
		}
	}
	point  next = previous.begin()->second;
	rec[next.first][next.second] = 1;
	previous.erase(previous.begin());
	dij(next.first,next.second);
}


int main() {
	uint32_t n ;
	while(cin >> n) {
		Solution solution(n);
		long long QPart1,QPart2;
		double dfMinus, dfFreq, dfTim;
		LARGE_INTEGER litmp;
		QueryPerformanceFrequency(&litmp);
		dfFreq = (double)litmp.QuadPart;
		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;
		cout << "\nThe min cost using dp is :" << solution.getVal(0) << endl;
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus * 1000000 /dfFreq;
		cout << "dp run time :" << dfTim << "us\n" << endl;

		QueryPerformanceCounter(&litmp);
		QPart1 = litmp.QuadPart;
		cout << "The min cost using sp is :" << solution.getVal(1) << endl;
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus * 1000000 /dfFreq;
		cout << "sp run time :" << dfTim << "us" << endl;
	}
	return 0;
}
