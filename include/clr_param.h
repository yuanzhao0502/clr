/*
 * clr_param.h
 *
 *  Created on: Feb 17, 2014
 *      Author: mac-97-41
 */

#ifndef CLR_PARAM_H_
#define CLR_PARAM_H_
#include<metis_def.h>
#include<metis_struct.h>
//This enum defines the different types of mu function
typedef enum{
	DENSITY,
	ANTIDENSITY
} mu_func;
//This class stores the parameters
class Param{
	params_t* metis_params;
	mu_func muFuncType;
	double metisRounds;
	double maxflowRounds;
	bool priorP;
	std::string idFile;

	//A default constructor for Param
public:
	Param(){
		metis_params = new params_t();

		//give default values to params_t object
		metis_params->ptype         = METIS_PTYPE_KWAY;
		metis_params->objtype       = METIS_OBJTYPE_CUT;
		metis_params->ctype         = METIS_CTYPE_SHEM;
		metis_params->iptype        = -1;
		metis_params->rtype         = -1;

		metis_params->no2hop        = 0;
		metis_params->minconn       = 0;
		metis_params->contig        = 0;

		metis_params->nooutput      = 0;
		metis_params->wgtflag       = 3;

		metis_params->ncuts         = 1;
		metis_params->niter         = 10;

		metis_params->dbglvl        = 0;
		metis_params->balance       = 0;
		metis_params->seed          = -1;
		metis_params->dbglvl        = 0;

		metis_params->tpwgtsfile    = NULL;

		metis_params->filename      = NULL;
		metis_params->nparts        = 2;

		metis_params->ufactor       = -1;

		metis_params->ubvecstr      = NULL;
		metis_params->ubvec         = NULL;

		metis_params->filename = new char[100];
		strcpy(metis_params->filename,"./graph.txt");

		//init the rest of Param by default values
		muFuncType = DENSITY;
		metisRounds = 10;
		maxflowRounds = 10;
		priorP = true;
		idFile = "./id.txt";

	}

	//constructor with CSR graph format and id file
	Param(const std::string& graphFile, const std::string& idFile){
		metis_params = new params_t();
		//give default values to params_t object
		metis_params->ptype         = METIS_PTYPE_KWAY;
		metis_params->objtype       = METIS_OBJTYPE_CUT;
		metis_params->ctype         = METIS_CTYPE_SHEM;
		metis_params->iptype        = -1;
		metis_params->rtype         = -1;

		metis_params->no2hop        = 0;
		metis_params->minconn       = 0;
		metis_params->contig        = 0;

		metis_params->nooutput      = 0;
		metis_params->wgtflag       = 3;

		metis_params->ncuts         = 1;
		metis_params->niter         = 10;

		metis_params->dbglvl        = 0;
		metis_params->balance       = 0;
		metis_params->seed          = -1;
		metis_params->dbglvl        = 0;

		metis_params->tpwgtsfile    = NULL;

		metis_params->filename      = NULL;
		metis_params->nparts        = 2;

		metis_params->ufactor       = -1;

		metis_params->ubvecstr      = NULL;
		metis_params->ubvec         = NULL;

		metis_params->filename = new char[100];
		metis_params->filename = const_cast<char*>(graphFile.c_str());

		//init the rest of Param by default values
		muFuncType = DENSITY;
		metisRounds = 10;
		maxflowRounds = 10;
		priorP = true;
		//Is the statement below giving a copy of idFile?
		this->idFile = idFile;
	}


	/**************************************
	 * This method returns a pointer to params_t object
	 **************************************/
	params_t* get_metis_params(){
		return metis_params;
	}
};
#endif /* CLR_STRUCTS_H_ */

