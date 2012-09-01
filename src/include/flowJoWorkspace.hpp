/*
 * flowJoWorkspace.hpp
 *
 *  Created on: Mar 15, 2012
 *      Author: wjiang2
 */

#ifndef FLOWJOWORKSPACE_HPP_
#define FLOWJOWORKSPACE_HPP_

#include "workspace.hpp"
#include "transformation.hpp"
#include <sstream>


class flowJoWorkspace:public workspace{
//	friend std::ostream & operator<<(std::ostream &os, const flowJoWorkspace &gh);
	friend class boost::serialization::access;
private:
	string versionList;

	template<class Archive>
		    void serialize(Archive &ar, const unsigned int version)
		    {
        		ar & boost::serialization::base_object<workspace>(*this);
				ar & versionList;
		    }
public:

	 vector <string> getSampleID(unsigned short);
	 virtual PARAM_VEC getTransFlag(wsSampleNode sampleNode);
     wsRootNode getRoot(wsSampleNode sampleNode);
     wsPopNodeSet getSubPop(wsNode * node);
     nodeProperties * to_popNode(wsRootNode &);
     nodeProperties * to_popNode(wsPopNode &,bool isParseGate);
     string getSampleName(wsSampleNode &);
     void parseVersionList();
     unsigned short getVersionMin();

};





#endif /* FLOWJOWORKSPACE_HPP_ */
