#include "subdivisionSurface.h"



subdivisionSurface::subdivisionSurface()
{
}

void subdivisionSurface::initSurface() {
	surfacePoints.clear();
	surfaceQuads.clear();
	// z y x
	// add points
	for (int i = 0; i < 8; i++) {
		float curx = 5.0f, cury =5.0f, curz = 5.0f;
		if ((i & 1) == 0) {
			curz *= -1.0f;
		}
		if ((i & 2) == 0) {
			cury *= -1.0f;
		}
		if ((i & 4) == 0) {
			curx *= -1.0f;
		}
		//cout << curx << " " << cury << " " << curz << endl;
		surfacePoints.push_back(Point(i, curx, cury, curz));
	}
	// six surfaces
	Quad temQuad1;
	temQuad1.id = 0;
	temQuad1.addPoint(0); temQuad1.addPoint(1); temQuad1.addPoint(3); temQuad1.addPoint(2);
	surfaceQuads.push_back(temQuad1);

	Quad temQuad2;
	temQuad2.id = 1;
	temQuad2.addPoint(4); temQuad2.addPoint(0); temQuad2.addPoint(2); temQuad2.addPoint(6);
	surfaceQuads.push_back(temQuad2);

	Quad temQuad3;
	temQuad3.id = 2;
	temQuad3.addPoint(4); temQuad3.addPoint(0); temQuad3.addPoint(1); temQuad3.addPoint(5);
	surfaceQuads.push_back(temQuad3);

	Quad temQuad4;
	temQuad4.id = 3;
	temQuad4.addPoint(5); temQuad4.addPoint(4); temQuad4.addPoint(6); temQuad4.addPoint(7);
	surfaceQuads.push_back(temQuad4);

	Quad temQuad5;
	temQuad5.id = 4;
	temQuad5.addPoint(1); temQuad5.addPoint(5); temQuad5.addPoint(7); temQuad5.addPoint(3);
	surfaceQuads.push_back(temQuad5);

	Quad temQuad6;
	temQuad6.id = 5;
	temQuad6.addPoint(6); temQuad6.addPoint(2); temQuad6.addPoint(3); temQuad6.addPoint(7);
	surfaceQuads.push_back(temQuad6);
}
void subdivisionSurface::nextRound() {
	//for each point find its beloged  quads
	//for each point find its adjacent points
	cout << "find belonged quads" << endl;
	for (auto &it : surfacePoints) {
		vector<int> temQuads;
		it.clearAdjacent();
		//cout << "debug:"<<it.id <<endl;
		for (auto &it1 : surfaceQuads) {
			bool ok = false;
			for (auto it2 : it1.pointIDList) {
				if (it.id == it2) {
					temQuads.push_back(it1.id);
					ok = true;
					break;
				}
			}
			if (ok) {
				//cout << "debug ok" << ok << endl;
				int p = -1;
				for (int i = 0; i < it1.pointIDList.size(); i++) {
					if (it1.pointIDList[i] == it.id) {
						p = i;
						break;
					}
				}
				//cout << "debug ok" << ok << endl;
				if (p != -1 && it1.pointIDList.size()==4) {
					int p2 = it1.pointIDList[(p + 1) % 4];
					int p0 = it1.pointIDList[((p - 1)+4) % 4];
					it.addAdjacents(p0);
					it.addAdjacents(p2);
				}
				else {
					cout << "p!=-1 || it1.pointidlist.size!=4" << endl;
				}
			}
		}
		it.clearQuads();
		for (auto it1 : temQuads) {
			it.addQuad(it1);
		}
	}
	//showAdjacents();
	cout << "calc mid coord" << endl;
	for (auto &it : surfaceQuads) {
		Vec3f temsum(0.0f, 0.0f, 0.0f);
		int num = 0;
		for (auto it1 : it.pointIDList) {
			for (auto it2 : surfacePoints) {
				if (it2.id == it1) {
					num++;
					temsum += it2.pos;
					break;
				}
			}
		}
		if (num != 4) {
			cout << "surface not quad" << endl;
		}
		temsum /= Vec3f(4.0f, 4.0f, 4.0f);
		it.midCoord = temsum;
	}
	// start division
	cout << "start devision" << endl;
	vector<Point>temPointArr;
	vector<Quad>temQuadArr;
	for (auto it:surfacePoints) {
		temPointArr.push_back(Point(it.id,it.pos));
	}
	for (auto &it : surfaceQuads) {
		// for each quad
		auto curQuad = it;
		Point abcd, ab, bc, cd, da;
		bool b1 = false, b2 = false, b3 = false, b4 = false;
		Point newQuad;
		abcd.pos = it.midCoord;
		abcd.id = temPointArr.size();
		temPointArr.push_back(Point(abcd.id, abcd.pos));
		if (it.pointIDList.size() != 4) {
			cout << "size not 4" << endl;
		}
		ab.pos = getEdgePoint(it.pointIDList[0], it.pointIDList[1]);
		bc.pos = getEdgePoint(it.pointIDList[1], it.pointIDList[2]);
		cd.pos = getEdgePoint(it.pointIDList[2], it.pointIDList[3]);
		da.pos = getEdgePoint(it.pointIDList[3], it.pointIDList[0]);
		//cout << "#######" << endl;
		//ab.pos.printVect();
		//bc.pos.printVect();
		//cd.pos.printVect();
		//da.pos.printVect();
		//cout << "#########" << endl;
		for (auto &it1 : temPointArr) {
			if (sqrtf(dotMul(ab.pos-it1.pos, ab.pos - it1.pos)) < eps) {
				b1 = true, ab.id = it1.id;
			}
			if (sqrtf(dotMul(bc.pos-it1.pos, bc.pos - it1.pos)) < eps) {
				b2 = true, bc.id = it1.id;
			}
			if (sqrtf(dotMul(cd.pos-it1.pos, cd.pos - it1.pos)) < eps) {
				b3 = true, cd.id = it1.id;
			}
			if (sqrtf(dotMul(da.pos-it1.pos, da.pos - it1.pos)) < eps) {
				b4 = true, da.id = it1.id;
			}
		}
		if (!b1) {
			ab.id = temPointArr.size();
			temPointArr.push_back(Point(ab.id, ab.pos));
			//cout << "b1" << endl;
		}
		if (!b2) {
			bc.id = temPointArr.size();
			temPointArr.push_back(Point(bc.id, bc.pos));
			//cout << "b2" << endl;
		}
		if (!b3) {
			cd.id = temPointArr.size();
			temPointArr.push_back(Point(cd.id, cd.pos));
			//cout << "b3" << endl;
		}
		if (!b4) { 
			da.id = temPointArr.size();
			temPointArr.push_back(Point(da.id, da.pos));
			//cout << "b4" << endl;
		}

		Quad q1, q2, q3, q4;
		q1.id = temQuadArr.size();
		q1.addPoint(it.pointIDList[0]);
		q1.addPoint(ab.id); 
		q1.addPoint(abcd.id);
		q1.addPoint(da.id);
		temQuadArr.push_back(q1);

		q2.id = temQuadArr.size();
		q2.addPoint(it.pointIDList[1]); 
		q2.addPoint(bc.id); 
		q2.addPoint(abcd.id); 
		q2.addPoint(ab.id);
		temQuadArr.push_back(q2);

		q3.id = temQuadArr.size();
		q3.addPoint(it.pointIDList[2]);
		q3.addPoint(cd.id); 
		q3.addPoint(abcd.id); 
		q3.addPoint(bc.id);
		temQuadArr.push_back(q3);

		q4.id = temQuadArr.size();
		q4.addPoint(it.pointIDList[3]); 
		q4.addPoint(da.id); 
		q4.addPoint(abcd.id); 
		q4.addPoint(cd.id);
		temQuadArr.push_back(q4);
	}
	cout << "renew coords" << endl;
	// rebuild quad idlist
	for (auto &it : temPointArr) {
		for (auto &it1 : temQuadArr) {
			for (auto &it2 : it1.pointIDList) {
				if (it.id == it2) {
					it.addQuad(it1.id);
				}
			}
		}
	}
	// calc quad IDList

	// renew coords
	int preNum = surfacePoints.size();
	for (auto &it : temPointArr) {
		if (it.id < preNum) {
			int nn = it.quadIDList.size();
			if (nn < 3) {
				cout << "itquadlist<3:" << nn<<" "<<it.id<<endl;
			}
			//cout << "itquadlist:" << nn << " " << it.id << endl;
			float m1 = (1.0f*nn - 3.0f) / (1.0*nn);
			float m2 = 1.0 / (1.0f*nn);
			float m3 = 2.0 / (1.0f*nn);

			Vec3f x1 = Vec3f(m1, m1, m1)*it.pos;
			Vec3f x2 = Vec3f(m2, m2, m2)*getAvgFacePoints(it.id);
			Vec3f x3 = Vec3f(m3, m3, m3)*getCenterEdge(it.id);
			
			it.pos = x1 + x2 + x3;
		}
		else {
			continue;
		}
	}
	// repalce
	cout << "replace" << endl;
	surfacePoints.clear();
	for (auto it : temPointArr) {
		surfacePoints.push_back(it);
	}
	//cout << surfacePoints.size() << endl;
	surfaceQuads.clear();
	for (auto it : temQuadArr) {
		surfaceQuads.push_back(it);
	}
	//cout << "suface quads num" << surfaceQuads.size() << endl;
	//showQuads();

}
subdivisionSurface::~subdivisionSurface()
{
}
