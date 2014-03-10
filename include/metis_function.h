/*
 * metis_functions.h
 *
 *  Created on: Jan 3, 2014
 *      Author: jasmin
 */
#include <metis_struct.h>
#include <metis_def.h>


#ifndef METIS_FUNCTIONS_H_
#define METIS_FUNCTIONS_H_

#ifdef _WINDLL
#define METIS_API(type) __declspec(dllexport) type __cdecl
#elif defined(__cdecl)
#define METIS_API(type) type __cdecl
#else
#define METIS_API(type) type
#endif


#ifdef __cplusplus
extern "C" {
#endif

METIS_API(int) METIS_PartGraphRecursive(idx_t *nvtxs, idx_t *ncon, idx_t *xadj,
                  idx_t *adjncy, idx_t *vwgt, idx_t *vsize, idx_t *adjwgt,
                  idx_t *nparts, real_t *tpwgts, real_t *ubvec, idx_t *options,
                  idx_t *edgecut, idx_t *part);

METIS_API(int) METIS_PartGraphKway(idx_t *nvtxs, idx_t *ncon, idx_t *xadj,
                  idx_t *adjncy, idx_t *vwgt, idx_t *vsize, idx_t *adjwgt,
                  idx_t *nparts, real_t *tpwgts, real_t *ubvec, idx_t *options,
                  idx_t *edgecut, idx_t *part);

METIS_API(int) METIS_MeshToDual(idx_t *ne, idx_t *nn, idx_t *eptr, idx_t *eind,
                  idx_t *ncommon, idx_t *numflag, idx_t **r_xadj, idx_t **r_adjncy);

METIS_API(int) METIS_MeshToNodal(idx_t *ne, idx_t *nn, idx_t *eptr, idx_t *eind,
                  idx_t *numflag, idx_t **r_xadj, idx_t **r_adjncy);

METIS_API(int) METIS_PartMeshNodal(idx_t *ne, idx_t *nn, idx_t *eptr, idx_t *eind,
                  idx_t *vwgt, idx_t *vsize, idx_t *nparts, real_t *tpwgts,
                  idx_t *options, idx_t *objval, idx_t *epart, idx_t *npart);

METIS_API(int) METIS_PartMeshDual(idx_t *ne, idx_t *nn, idx_t *eptr, idx_t *eind,
                  idx_t *vwgt, idx_t *vsize, idx_t *ncommon, idx_t *nparts,
                  real_t *tpwgts, idx_t *options, idx_t *objval, idx_t *epart,
                  idx_t *npart);

METIS_API(int) METIS_NodeND(idx_t *nvtxs, idx_t *xadj, idx_t *adjncy, idx_t *vwgt,
                  idx_t *options, idx_t *perm, idx_t *iperm);

METIS_API(int) METIS_Free(void *ptr);

METIS_API(int) METIS_SetDefaultOptions(idx_t *options);


/* These functions are used by ParMETIS */

METIS_API(int) METIS_NodeNDP(idx_t nvtxs, idx_t *xadj, idx_t *adjncy, idx_t *vwgt,
                   idx_t npes, idx_t *options, idx_t *perm, idx_t *iperm,
                   idx_t *sizes);

METIS_API(int) METIS_ComputeVertexSeparator(idx_t *nvtxs, idx_t *xadj, idx_t *adjncy,
                   idx_t *vwgt, idx_t *options, idx_t *sepsize, idx_t *part);

METIS_API(int) METIS_NodeRefine(idx_t nvtxs, idx_t *xadj, idx_t *vwgt, idx_t *adjncy,
                   idx_t *where, idx_t *hmarker, real_t ubfactor);






//Added by Peng. The header

METIS_API(graph_t ) *ReadGraph(params_t *params);
METIS_API(void) ReadTPwgts(params_t *params, idx_t ncon);

METIS_API(idx_t) IsConnected(graph_t *graph, idx_t report);
METIS_API(void) FreeGraph(graph_t **r_graph) ;
METIS_API(idx_t*) run_metis_clr(params_t* params);

METIS_API(int) plus(int a, int b);

#ifdef __cplusplus
}
#endif


#endif /* METIS_FUNCTIONS_H_ */
