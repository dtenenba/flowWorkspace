/*
 * ncdfFlow.hpp
 *
 * this entire class is deprecated: we don't want to keep a separate view of ncdfFlowSet in c++
 */
#ifndef NCDFFLOW_HPP_
#define NCDFFLOW_HPP_
#include <string>
#include "hdf5.h"
#include <R.h>
#include <stdlib.h>
#define DATASETNAME "/exprsMat"
#include <netcdf.h>
#include "/home/wjiang2/rglab/workspace/flowWorkspace/src/include/flowData.hpp"
using namespace std;

class ncdfFlow{
	friend std::ostream & operator<<(std::ostream &os, const ncdfFlow &nc);
	friend class boost::serialization::access;
protected:
	string fileName;
	vector<string> sampleNames;
	vector<string> params;
private:
	template<class Archive>
			void serialize(Archive &ar, const unsigned int version)
			{

					ar & fileName;
					ar & sampleNames;
					ar & params;
			}
public:
	ncdfFlow();
	ncdfFlow(string _fileName);
	void fileName_set(string _fileName);
	void params_set(vector<string> _params);
	void sample_set(vector<string> _sampleNames);
	vector<string> params_get();
	string fileName_get();
//	float * readSlice(unsigned int sampleID);
	flowData readflowData(unsigned int sampleID);
	flowData readflowData(string sampleName);
	void writeflowData(flowData & fdata);
};

class hdfFlow:public ncdfFlow{
public:
	flowData readflowData(string sampleName);
	flowData readflowData(unsigned int sampleID);
//	void writeflowData(flowData & fdata);
};
#endif /* NCDFFLOW_HPP_ */
