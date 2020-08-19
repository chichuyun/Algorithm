#include<iostream>
#include<cmath>
#include<stack>
#include<vector>
#include<algorithm>

using namespace std;

vector<int> point;

class GrahamScan {
private:
    vector<vector<int>> res;
    vector<vector<int>> points;

    static int _distance(vector<int>& p, vector<int>& q) {
        return (p[0] - q[0])*(p[0] - q[0]) +
               (p[1] - q[1])*(p[1] - q[1]);
    }

    static int _cross(vector<int>& p, vector<int>& q, vector<int>& r) {
        // p -> q -> r
        // (r - q) `cross` (q - p)
        return (q[0] - p[0])*(r[1] - q[1]) -
               (q[1] - p[1])*(r[0] - q[0]);
    }

    static bool _func_l(vector<int>& p, vector<int>& q) {
        int cross = _cross(point, p, q);
        if(cross != 0) return cross > 0;
        if(p[0] > point[0]) {
            return _distance(p, point) < _distance(q, point);
        } else {
            return _distance(p, point) > _distance(q, point);
        }
    }

    static bool _func_r(vector<int>& p, vector<int>& q) {
        int cross = _cross(point, p, q);
        if(cross != 0) return cross > 0;
        if(p[0] >= point[0]) {
            return _distance(p, point) < _distance(q, point);
        } else {
            return _distance(p, point) > _distance(q, point);
        }
    }
public:
    GrahamScan(vector<vector<int>>& points) : points(points) {}

    void solve() {
        if(points.size() < 2) return;
        point.resize(2); // left bottom point
        point[0] = INT_MAX; point[1] = INT_MAX;

        int right = INT_MIN;
        int index = 0;
        for(int i=0; i<points.size(); ++i) {
            auto &p = points[i];
            right = max(right, p[0]);
            if(p[1] < point[1] || (p[1] == point[1] && p[0] < point[0])) {
                point = p;
                index = i;
            }
        }

        swap(points[0], points[index]);  // move point to points[0]
        if(point[0] == right) {
            sort(points.begin()+1, points.end(), _func_r);
        } else {
            sort(points.begin()+1, points.end(), _func_l);
        }

        int cross;
        stack<vector<int>> nodes;
        nodes.push(points[0]);
        nodes.push(points[1]);
        for(int i=2; i<points.size(); ++i) {
            vector<int> top = nodes.top();
            nodes.pop();
            cross = _cross(nodes.top(), top, points[i]);

            if(cross < 0) {
                while(true) {
                    top = nodes.top();
                    nodes.pop();
                    cross = _cross(nodes.top(), top, points[i]);
                    if(cross >= 0) break;
                }
            }
            nodes.push(top);
            nodes.push(points[i]);
        }

        while(!nodes.empty()) {
            res.push_back(nodes.top());
            nodes.pop();
        }
    }

    vector<vector<int>> getLists() {
        if(res.empty()) solve();
        return res;
    }

    double getArea() {
        if(res.empty()) solve();
        vector<vector<int>> areas(res);
        for(int i=0; i<res.size(); ++i) {
            areas[i][0] -= point[0];
            areas[i][1] -= point[1];
        }
        double area = 0.0;
        for(int i=1; i<areas.size(); ++i) {
            area += areas[i-1][1]*areas[i][0] - areas[i][1]*areas[i-1][0];
        }
        area += areas[0][1]*areas.back()[0] - areas.back()[1]*areas[0][0];
        return fabs(area)/2.0;
    }

    double getPerimeter() {
        if(res.empty()) solve();
        vector<vector<int>> pers(res);
        for(int i=0; i<pers.size(); ++i) {
            pers[i][0] -= point[0];
            pers[i][1] -= point[1];
        }
        double perimeter = 0.0;
        for(int i=1; i<pers.size(); ++i) {
            perimeter += sqrt((pers[i][0]- pers[i-1][0])*(pers[i][0]- pers[i-1][0]) +
                              (pers[i][1]- pers[i-1][1])*(pers[i][1]- pers[i-1][1]));
        }
        perimeter += sqrt((pers[0][0]- pers.back()[0])*(pers[0][0]- pers.back()[0]) +
                          (pers[0][1]- pers.back()[1])*(pers[0][1]- pers.back()[1]));
        return perimeter;
    }
};

int main() {
    vector<vector<int>> points = {{3, 0},
                                  {4, 0},
                                  {5, 0},
                                  {6, 1},
                                  {7, 2},
                                  {7, 3},
                                  {7, 4},
                                  {6, 5},
                                  {5, 5},
                                  {4, 5},
                                  {3, 5},
                                  {2, 5},
                                  {1, 4},
                                  {1, 3},
                                  {1, 2},
                                  {2, 1}};
    GrahamScan *h = new GrahamScan(points);
    h->solve();
    vector<vector<int>> res = h->getLists();
    for(auto& p : res) cout << p[0] << " " << p[1] << endl;
    double area = h->getArea();
    double perimeter = h->getPerimeter();
    cout << "Area:      " << area << endl;
    cout << "Perimeter: " << perimeter << endl;

    return 0;
}
