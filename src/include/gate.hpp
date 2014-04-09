/*
 * gate.hpp
 *
 *  Created on: Mar 16, 2012
 *      Author: wjiang2
 */

#ifndef GATE_HPP_
#define GATE_HPP_
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include "flowData.hpp"
#include "transformation.hpp"


using namespace std;


typedef struct{
	vector<string> path;
	char op;
	bool isNot;
	template<class Archive>
				    void serialize(Archive &ar, const unsigned int version)
				    {

						ar & BOOST_SERIALIZATION_NVP(path);
						ar & BOOST_SERIALIZATION_NVP(op);
						ar & BOOST_SERIALIZATION_NVP(isNot);
				    }
	} BOOL_GATE_OP;


#define GATING_SET_LEVEL 1
#define GATING_HIERARCHY_LEVEL 2
#define POPULATION_LEVEL 3
#define GATE_LEVEL 4

#define POLYGONGATE 1
#define RANGEGATE 2
#define BOOLGATE 3
#define ELLIPSEGATE 4
#define RECTGATE 5
#define LOGICALGATE 6

#define AND 1
#define OR 2
#define ANDNOT 3
#define ORNOT 4

struct vertices_vector{
	vector<double> x;
	vector<double> y;
};

class vertices_valarray{
public:
	valarray<double> x;
	valarray<double> y;
public:
	void resize(unsigned nSize){
		x.resize(nSize);
		y.resize(nSize);
	}
	vertices_valarray(){};
	vertices_valarray(vector<coordinate> vertices){

			unsigned nSize=vertices.size();
			resize(nSize);
			for(unsigned i=0;i<nSize;i++)
			{
				x[i]=vertices.at(i).x;
				y[i]=vertices.at(i).y;
			}

	}
	vertices_vector toVector(){
		vertices_vector res;
		for(unsigned i=0;i<x.size();i++)
			res.x.push_back(x[i]);
		for(unsigned i=0;i<y.size();i++)
			res.y.push_back(y[i]);

		return res;
	}
	void print(){
		cout<<"x:";
		for(unsigned i=0;i<x.size();i++)
				cout<<x[i]<<",";
//		cout<<"x:";
//		for(unsigned i=0;i<x.size();i++)
//				cout<<x[i]<<",";

	}
};


class paramRange
{

	friend class boost::serialization::access;
private:

	string name;
	double min, max;
	template<class Archive>
						void serialize(Archive &ar, const unsigned int version)
						{

							ar & BOOST_SERIALIZATION_NVP(name);
							ar & BOOST_SERIALIZATION_NVP(min);
							ar & BOOST_SERIALIZATION_NVP(max);
						}
public:
	paramRange(double _min,double _max,string _name){min=_min;max=_max;name=_name;};
	paramRange(){};
	vertices_valarray toValarray();
	void setName(string _n){name=_n;};
	string getName(){return name;}
	vector<string> getNameArray(){
			vector<string> res;
			res.push_back(name);
			return res;
		};
	double getMin(){return min;};
	void setMin(double _v){min=_v;};
	double getMax(){return max;};
	void setMax(double _v){max=_v;};
};
class paramPoly
{
//	friend std::ostream & operator<<(std::ostream &os, const paramPoly &gh);
	friend class boost::serialization::access;
private:


	vector<string> params;//params.at(0) is x, params.at(1) is y axis
	vector<coordinate> vertices;

	template<class Archive>
					void serialize(Archive &ar, const unsigned int version)
					{

						ar & BOOST_SERIALIZATION_NVP(params);
						ar & BOOST_SERIALIZATION_NVP(vertices);
					}
public:
	vector<coordinate> getVertices(){return vertices;};
	void setVertices(vector<coordinate> _v){vertices=_v;};
	vector<string>  getNameArray(){return params;};
	void setName(vector<string> _params){params=_params;};
	vertices_valarray toValarray();
	string xName(){return params.at(0);};
	string yName(){return params.at(1);};
};


/*
 * TODO:possibly implement getCentroid,getMajorAxis,getMinorAxis for all gate types
 */
class gate {
//	friend std::ostream & operator<<(std::ostream &os, const gate &gh);
	friend class boost::serialization::access;

protected:
	bool neg;
	bool isTransformed;
	bool isGained;
private:
	template<class Archive>
			void save(Archive &ar, const unsigned int version) const
			{

				ar & BOOST_SERIALIZATION_NVP(neg);
				ar & BOOST_SERIALIZATION_NVP(isTransformed);
				if(version>0)
					ar & BOOST_SERIALIZATION_NVP(isGained);

			}
	template<class Archive>
			void load(Archive &ar, const unsigned int version)
			{

				ar & BOOST_SERIALIZATION_NVP(neg);
				ar & BOOST_SERIALIZATION_NVP(isTransformed);
				if(version>0)
					ar & BOOST_SERIALIZATION_NVP(isGained);
				else{
					isGained = false;
				}

			}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
	/*
	 * exact string returned by std::type_info::name() is compiler-dependent
	 * so we can't rely on RTTI. instead we return the gate type by API
	 * However it is against the motivation for nodeProperty to use base gate pointer
	 * the very reason of this gate abstraction was to make gatingheirarhcy being agnostic
	 * about the gate type. The reason we are doing it is a compromise to the needs of R API getGate
	 */
	virtual ~gate(){};
	virtual unsigned short getType()=0;
	virtual vector<BOOL_GATE_OP> getBoolSpec(){throw(domain_error("undefined getBoolSpec function!"));};
	virtual vector<bool> gating(flowData &){throw(domain_error("undefined gating function!"));};
	virtual void extend(flowData &,float,unsigned short){throw(domain_error("undefined extend function!"));};
	virtual void gain(map<string,float> &,unsigned short){throw(domain_error("undefined gain function!"));};
	virtual vector<string> getParamNames(){throw(domain_error("undefined getParam function!"));};
	virtual vertices_valarray getVertices(){throw(domain_error("undefined getVertices function!"));};
	virtual void transforming(trans_local &,unsigned short dMode){throw(domain_error("undefined transforming function!"));};
//	virtual gate * create()=0;
	virtual gate * clone()=0;

	virtual bool isNegate(){return neg;};
	virtual void setNegate(bool _neg){neg=_neg;};
	virtual bool Transformed(){return isTransformed;};
	virtual void setTransformed(bool _isTransformed){isTransformed=_isTransformed;};
};
BOOST_CLASS_VERSION(gate,1)

class rangeGate:public gate {
	friend class boost::serialization::access;

private:
	paramRange param;
	template<class Archive>
			void serialize(Archive &ar, const unsigned int version)
			{
				ar & boost::serialization::make_nvp("gate",boost::serialization::base_object<gate>(*this));
				ar & BOOST_SERIALIZATION_NVP(param);

			}
public:
	rangeGate();
	unsigned short getType(){return RANGEGATE;}
	vector<bool> gating(flowData &);
	void extend(flowData &,float,unsigned short);
	void gain(map<string,float> &,unsigned short);
	void transforming(trans_local &,unsigned short dMode);
	paramRange getParam(){return param;};
	vector<string> getParamNames(){return param.getNameArray();};
	void setParam(paramRange _param){param=_param;};
	vertices_valarray getVertices(){return param.toValarray();};
	rangeGate * clone(){return new rangeGate(*this);};

};

/*
 * TODO:using #include <boost/multi_array.hpp> instead to make it easier to convert to R data structure hopefully.
 *
 */
class polygonGate:public gate {
	friend class boost::serialization::access;
protected:
	paramPoly param;
private:
	template<class Archive>
			void serialize(Archive &ar, const unsigned int version)
			{
				ar & boost::serialization::make_nvp("gate",boost::serialization::base_object<gate>(*this));
				ar & BOOST_SERIALIZATION_NVP(param);

			}
public:
	polygonGate();
	virtual unsigned short getType(){return POLYGONGATE;}
	virtual void extend(flowData &,float,unsigned short);
	virtual void gain(map<string,float> &,unsigned short);
	virtual vector<bool> gating(flowData &);
	virtual void transforming(trans_local &,unsigned short dMode);
	virtual vertices_valarray getVertices(){return param.toValarray();};
	void setParam(paramPoly _param){param=_param;};
	virtual paramPoly getParam(){return param;};
	virtual vector<string> getParamNames(){return param.getNameArray();};
	virtual polygonGate * clone(){return new polygonGate(*this);};
};
/*
 * rectgate is a special polygon requires simpler gating routine
 * it doesn't overload getType member function, which means it is exposed to R
 * as a regular polygonGate
 */
class rectGate:public polygonGate {
	friend class boost::serialization::access;

private:
	template<class Archive>
			void serialize(Archive &ar, const unsigned int version)
			{
				ar & boost::serialization::make_nvp("polygonGate",boost::serialization::base_object<polygonGate>(*this));

			}
public:
	vector<bool> gating(flowData &);
	unsigned short getType(){return RECTGATE;}
	rectGate * clone(){return new rectGate(*this);};
};
/*
 * TODO: doing the gating without interpolating it into polygon
 */
class ellipseGate:public polygonGate {
	friend class boost::serialization::access;
private:
	//four antipodal points of ellipse
	vector<coordinate> antipodal_vertices;
	template<class Archive>
			void serialize(Archive &ar, const unsigned int version)
			{
				ar & boost::serialization::make_nvp("polygonGate",boost::serialization::base_object<polygonGate>(*this));
				ar & BOOST_SERIALIZATION_NVP(antipodal_vertices);

			}

public:
	vector<coordinate> getAntipodal(){return antipodal_vertices;};
	void setAntipodal(vector<coordinate> _v){antipodal_vertices=_v;};
	unsigned short getType(){return ELLIPSEGATE;}
	void extend(flowData &,float,unsigned short);
	void gain(map<string,float> &,unsigned short);
	void toPolygon(unsigned);
	void transforming(trans_local &,unsigned short dMode);
	ellipseGate * clone(){return new ellipseGate(*this);};

};
/*
 * instead of defining the gating function here in boolGate
 * we put the gating logic in GatingHierarchy gating function
 * because it is needs to access indices from reference nodes
 * which actually belong to GatingHierarchy object.
 * And gate classes are sits in more abstract level than GatingHierarchy in the C++ class tree,
 * thus GatingHierarchy data structure should be invisible to gate.
 */
class boolGate:public gate {
	friend class boost::serialization::access;


public:
	boolGate();
	vector<BOOL_GATE_OP> boolOpSpec;//the gatePaths with the their logical operators
private:
	template<class Archive>
				void serialize(Archive &ar, const unsigned int version)
				{
					ar & boost::serialization::make_nvp("gate",boost::serialization::base_object<gate>(*this));
					ar & BOOST_SERIALIZATION_NVP(boolOpSpec);

				}
public:
	vector<BOOL_GATE_OP> getBoolSpec(){return boolOpSpec;};
	unsigned short getType(){return BOOLGATE;}
	boolGate * clone(){return new boolGate(*this);};
};


#endif /* GATE_HPP_ */
