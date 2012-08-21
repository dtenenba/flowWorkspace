/*
 * GatingHierarchy.hpp
 *
 *  Created on: Mar 17, 2012
 *      Author: mike
 */

#ifndef GATINGHIERARCHY_HPP_
#define GATINGHIERARCHY_HPP_
#include <iostream>
#include <string>
#include <vector>
#include "populationTree.hpp"
#include "flowJoWorkspace.hpp"
#include "ncdfFlow.hpp"
#include <libxml/xpath.h>
using namespace std;
typedef map<string,VertexID> VertexID_map;
typedef vector<VertexID> VertexID_vec;



struct OurVertexPropertyWriter {

	OurVertexPropertyWriter(populationTree &g_) : g(g_) {}

    template <class Vertex>
    void operator() (std::ostream &out, Vertex u) {

    	out<<"[shape=record,label=\"{"<<g[u].getName()<<"|count:"<<g[u].fjStats["count"]<<"}\"]";


    }

    populationTree &g;
};

struct OurVertexPropertyWriterR {

	OurVertexPropertyWriterR(populationTree &g_) : g(g_) {}

    template <class Vertex>
    void operator() (std::ostream &out, Vertex u) {

    	out<<"[shape=record,label=\""<<g[u]->getName()<<"\"]";


    }

    populationTree &g;
};




/*GatingHierarchy is a tree that holds the gate definitions hierarchically,
 along with the transformation functions and compensation matrix,
 Once the one FCS file is associated,the tree can also hold indices that subset the events
 It can serves as a gating template when data is empty
 */

class GatingHierarchy{

	compensation comp;/*compensation is currently done in R due to the linear Algebra
						e[, cols] <- t(solve(t(spillover))%*%t(e[,cols]))
						we can try uBlas for this simple task, but when cid=="-1",we still need to
						do this in R since comp is extracted from FCS keyword (unless it can be optionally extracted from workspace keyword)
	 	 	 	 	  */
	flowData fdata;
	populationTree tree;
	bool isGated;
	bool isLoaded;

	ncdfFlow *nc;//a pointer to the global cdf data stored within gatingSet
	workspace * thisWs;

	trans_global_vec *gTrans;//pointer to the global trans stored in gs
	PARAM_VEC transFlag;
	trans_local trans;
public:

	unsigned short dMode;//debug mode passed from GatingSet


	/*remove the gate from */
//	void removeGate(unsigned short popId);
//	void removeGate(string popName);

	/*append the gate to the tree*/
	void addChild(VertexID parent,VertexID child);
	void addGate(gate& g,string popName);
	void addPopulation(VertexID parentID,wsNode * parentNode,bool isGating);
	VertexID addRoot(nodeProperties* rootNode);
	GatingHierarchy();
	~GatingHierarchy();

	GatingHierarchy(wsSampleNode curSampleNode,workspace * ws,bool isGating,ncdfFlow *,trans_global_vec * _gTrans,unsigned short dMode);


	flowData getData(VertexID nodeID);//from memory
	flowData getData(string string,VertexID nodeID);//from cdf
	void loadData(string);
	void loadData(const flowData &);

	void unloadData();


	compensation getCompensation();
	trans_local getLocalTrans(){return trans;}
	void printLocalTrans();//for the debugging purpose
	void transforming(bool);
	void gating();
	void gating(VertexID);
	POPINDICES boolGating(VertexID);
	void extendGate();
	void drawGraph(string out);
	VertexID getChildren(VertexID source,string childName);
	VertexID getNodeID(vector<string> gatePath);
	VertexID_vec getVertices(bool tsort);//return the node list in vertexID order or T order
	vector<string> getPopNames(bool tsort,bool isPath);
	VertexID_vec getParent(VertexID);
	VertexID_vec getChildren(VertexID);
	nodeProperties * getNodeProperty(VertexID);
	void setNcPtr(ncdfFlow *_nc){nc=_nc;}
	GatingHierarchy * clone(const trans_map & _trans,trans_global_vec * _gTrans);
};



#endif /* GATINGHIERARCHY_HPP_ */
