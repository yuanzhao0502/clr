/*
 * test_run_metis_clr.cpp
 *
 *  Created on: Jan 10, 2014
 *      Author: penpen926
 */

#include <test/test_header.h>

void test_run_metis_clr()
{
	params_t* params = new params_t();
	/* initialize the params data structure */
	params->ptype         = METIS_PTYPE_KWAY;
	params->objtype       = METIS_OBJTYPE_CUT;
	params->ctype         = METIS_CTYPE_SHEM;
	params->iptype        = -1;
	params->rtype         = -1;

    params->no2hop        = 0;
	params->minconn       = 0;
	params->contig        = 0;

	params->nooutput      = 0;
    params->wgtflag       = 3;

	params->ncuts         = 1;
	params->niter         = 10;

	params->dbglvl        = 0;
	params->balance       = 0;
	params->seed          = -1;
	params->dbglvl        = 0;

	params->tpwgtsfile    = NULL;

	params->filename      = NULL;
	params->nparts        = 2;

	params->ufactor       = -1;

	params->ubvecstr      = NULL;
	params->ubvec         = NULL;

    params->filename = new char[100];
    strcpy(params->filename,"/Users/mac-97-41/Work/cluster_local_restrictions/test/input/4elt.graph");
    run_metis_clr(params);
    plus(3,4);
}


