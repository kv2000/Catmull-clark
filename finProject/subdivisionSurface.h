#pragma once

#include<iostream>
#include<algorithm>
#include<map>
#include<vector>
#include<string>
#include"utils.h"



class Point
{
public:
	int id;
	Vec3f pos;
	vector<int>quadIDList;
	vector<int>adjacentList;
	Point() {};
	Point(int _id, float _x, float _y, float _z) {
		quadIDList.clear();
		id = _id;
		pos.x = _x;
		pos.y = _y;
		pos.z = _z;
	}
	Point(int _id,Vec3f _pos) {
		quadIDList.clear();
		id = _id;
		pos = _pos;
	}
	void clearQuads() {
		quadIDList.clear();
	}
	void addQuad(int _id) {
		quadIDList.push_back(_id);
	}
	void clearAdjacent() {
		adjacentList.clear();
	}
	void addAdjacents(int _id) {
		bool exists = false;
		for (auto itt : adjacentList) {
			if (itt == _id) {
				exists = true;
				break;
			}
		}
		if (!exists) {
			adjacentList.push_back(_id);
		}
	}
	~Point() {};
};

class Quad
{
public:
	int id;
	Vec3f midCoord;
	vector<int> pointIDList;
	vector<Vec3f> pointPosList;
	Quad() {
		midCoord.x = 0;
		midCoord.y = 0;
		midCoord.z = 0;
	};
	void clearPoints() {
		pointIDList.clear();
		midCoord.x = 0;
		midCoord.y = 0;
		midCoord.z = 0;
	}
	void addPoint(int _id) {
		pointIDList.push_back(_id);
	}
	~Quad() {};
};


class subdivisionSurface
{
public:
	subdivisionSurface();
	void initSurface();
	void nextRound();
	void showQuads() {
		for (auto &it : surfaceQuads) {
			cout << it.id << ":\n";
			for (auto &it1 : it.pointIDList) {
				cout << it1 << " ";
				for (int i = 0; i < surfacePoints.size(); i++) {
					if (surfacePoints[i].id == it1) {
						cout << surfacePoints[i].pos.x << " "
							<< surfacePoints[i].pos.y << " "
							<< surfacePoints[i].pos.z << endl;
						break;
					}
				}
			}
			cout << endl;
		}
	}
	Vec3f getEdgePoint(int id1, int id2) {
		Vec3f retVec;
		Vec3f p1, p2;
		//cout << "#########################" << endl;
		for (auto it : surfacePoints) {
			if (it.id == id1) {
				p1 = it.pos;
			}
			if (it.id == id2) {
				p2 = it.pos;
			}
		}
		int nn = 0;
		for (auto &it : surfaceQuads) {
			int curNum = 0;
			for (auto it1 : it.pointIDList) {
				if (it1 == id1 || it1 == id2) {
					curNum++;
					//cout
				}
			}
			if (curNum == 2) {
				// the face with edge
				nn++;
				retVec += it.midCoord;
			}
		}
		if (nn != 2) {
			cout << "edge with two face points" << nn << endl;
		}
		retVec += p1;
		retVec += p2;

		//p1.printVect();
		//p2.printVect();
		retVec /= Vec3f(4.0f, 4.0f, 4.0f);
		//retVec.printVect();
		//cout << "##############################" << endl;
		return retVec;
	}
	Vec3f getCenterEdge(int id1) {
		Vec3f retVec;
		bool ok = false;
		int curEdgeNum = 0;
		Point center;
		for (auto &it : surfacePoints) {
			if (it.id != id1) {
				continue;
			}
			center = it;
			break;
		}
		//cout <<"centerid:"<< center.id << endl;
		int finNum = 0;
		for (auto &it : surfacePoints) {
			for (auto it1 : center.adjacentList) {
				if (it.id == it1) {
					finNum++;
					retVec += (it.pos + center.pos)*Vec3f(0.5f, 0.5f, 0.5f);
				}
			}
		}
		if (finNum == 0) {
			cout << "divided by 0" << endl;
		}
		//cout << finNum << endl;
		retVec /= Vec3f(1.0f * finNum, 1.0f * finNum, 1.0f * finNum);

		/*
		if (ok == false) {
			cout << "no "<<id1 << endl; 
		}
		*/
		return retVec;
	}
	Vec3f getAvgFacePoints(int _id) {
		Vec3f retVec(0.0f,0.0f,0.0f);
		int num = 0;
		for (auto &it : surfaceQuads) {
			int ok = false;
			for (auto it2 : it.pointIDList) {
				if (it2 == _id) {
					ok = true;
					break;
				}
			}
			if (ok) {
				num++;
				//cout << "itmidcoord" << endl;
				//it.midCoord.printVect();
				retVec += it.midCoord;
			}
		}
		//cout <<"average face point num"<< num << endl;
		if (num == 0) {
			cout << "face num 0" << endl;
		}
		else {
			retVec /= Vec3f((1.0f*num), (1.0f*num),(1.0f*num));
		}
		return retVec;
	}
	vector<Quad>surfaceQuads;
	vector<Point>surfacePoints;
	void preDraw() {
		for (auto &it : surfaceQuads) {
			it.pointPosList.clear();
			for (auto it1 : it.pointIDList) {
				bool ok = false;
				for (auto &it3 : surfacePoints) {
					if (it3.id == it1) {
						it.pointPosList.push_back(Vec3f(it3.pos.x, it3.pos.y, it3.pos.z));
						ok = true;
					}
				}
				if (!ok) {
					cout << "missing id:" << it1 << endl;
				}
			}
		}
	}
	void showAdjacents() {
		cout << "############adjacents###########" << endl;
		for (auto &it : surfacePoints) {
			cout << it.id << ":" << it.adjacentList.size() << endl;
			for (auto it1 : it.adjacentList) {
				cout << " " << it1;
			}
			cout << endl;
		}
		cout << "##############adjacents end##############" << endl;
	}
	~subdivisionSurface();

};

