/*
 * Copyright 1997, Regents of the University of Minnesota
 *
 * struct.h
 *
 * This file contains data structures for ILU routines.
 *
 * Started 9/26/95
 * George
 *
 * $Id: struct.h 13900 2013-03-24 15:27:07Z karypis $
 */

#ifndef _LIBMETIS_STRUCT_H_
#define _LIBMETIS_STRUCT_H_

#include <metis_def.h>

/*! This data structure stores cut-based k-way refinement info about an
    adjacent subdomain for a given vertex. */
/*************************************************************************/
typedef struct cnbr_t {
  idx_t pid;            /*!< The partition ID */
  idx_t ed;             /*!< The sum of the weights of the adjacent edges
                             that are incident on pid */
} cnbr_t;


/*************************************************************************/
/*! The following data structure stores holds information on degrees for k-way
    partition */
/*************************************************************************/
typedef struct ckrinfo_t {
 idx_t id;              /*!< The internal degree of a vertex (sum of weights) */
 idx_t ed;            	/*!< The total external degree of a vertex */
 idx_t nnbrs;          	/*!< The number of neighboring subdomains */
 idx_t inbr;            /*!< The index in the cnbr_t array where the nnbrs list 
                             of neighbors is stored */
} ckrinfo_t;


/*************************************************************************/
/*! This data structure stores volume-based k-way refinement info about an
    adjacent subdomain for a given vertex. */
/*************************************************************************/
typedef struct vnbr_t {
  idx_t pid;            /*!< The partition ID */
  idx_t ned;            /*!< The number of the adjacent edges
                             that are incident on pid */
  idx_t gv;             /*!< The gain in volume achieved by moving the
                             vertex to pid */
} vnbr_t;


/*************************************************************************/
/*! The following data structure holds information on degrees for k-way
    vol-based partition */
/*************************************************************************/
typedef struct vkrinfo_t {
 idx_t nid;             /*!< The internal degree of a vertex (count of edges) */
 idx_t ned;            	/*!< The total external degree of a vertex (count of edges) */
 idx_t gv;            	/*!< The volume gain of moving that vertex */
 idx_t nnbrs;          	/*!< The number of neighboring subdomains */
 idx_t inbr;            /*!< The index in the vnbr_t array where the nnbrs list 
                             of neighbors is stored */
} vkrinfo_t;


/*************************************************************************/
/*! The following data structure holds information on degrees for k-way
    partition */
/*************************************************************************/
typedef struct nrinfo_t {
 idx_t edegrees[2];  
} nrinfo_t;


/*************************************************************************/
/*! This data structure holds a graph */
/*************************************************************************/
typedef struct graph_t {
  idx_t nvtxs, nedges;	/* The # of vertices and edges in the graph */
  idx_t ncon;		/* The # of constrains */ 
  idx_t *xadj;		/* Pointers to the locally stored vertices */
  idx_t *vwgt;		/* Vertex weights */
  idx_t *vsize;		/* Vertex sizes for min-volume formulation */
  idx_t *adjncy;        /* Array that stores the adjacency lists of nvtxs */
  idx_t *adjwgt;        /* Array that stores the weights of the adjacency lists */

  idx_t *tvwgt;         /* The sum of the vertex weights in the graph */
  real_t *invtvwgt;     /* The inverse of the sum of the vertex weights in the graph */


  /* These are to keep track control if the corresponding fields correspond to
     application or library memory */
  int free_xadj, free_vwgt, free_vsize, free_adjncy, free_adjwgt;

  idx_t *label;

  idx_t *cmap;

  /* Partition parameters */
  idx_t mincut, minvol;
  idx_t *where, *pwgts;
  idx_t nbnd;
  idx_t *bndptr, *bndind;

  /* Bisection refinement parameters */
  idx_t *id, *ed;

  /* K-way refinement parameters */
  ckrinfo_t *ckrinfo;   /*!< The per-vertex cut-based refinement info */
  vkrinfo_t *vkrinfo;   /*!< The per-vertex volume-based refinement info */

  /* Node refinement information */
  nrinfo_t *nrinfo;

  struct graph_t *coarser, *finer;
} graph_t;


/*************************************************************************/
/*! This data structure holds a mesh */
/*************************************************************************/
typedef struct mesh_t {
  idx_t ne, nn;	        /*!< The # of elements and nodes in the mesh */
  idx_t ncon;           /*!< The number of element balancing constraints (element weights) */

  idx_t *eptr, *eind;   /*!< The CSR-structure storing the nodes in the elements */
  idx_t *ewgt;          /*!< The weights of the elements */
} mesh_t;




/*************************************************************************/
/*! This data structure stores the various command line arguments */
/*************************************************************************/
typedef struct {
  idx_t ptype;
  idx_t objtype;
  idx_t ctype;
  idx_t iptype;
  idx_t rtype;

  idx_t no2hop;
  idx_t minconn;
  idx_t contig;

  idx_t nooutput;

  idx_t balance;
  idx_t ncuts;
  idx_t niter;

  idx_t gtype;
  idx_t ncommon;

  idx_t seed;
  idx_t dbglvl;

  idx_t nparts;

  idx_t nseps;
  idx_t ufactor;
  idx_t pfactor;
  idx_t compress;
  idx_t ccorder;

  char *filename;
  char *outfile;
  char *xyzfile;
  char *tpwgtsfile;
  char *ubvecstr;

  idx_t wgtflag;
  idx_t numflag;
  real_t *tpwgts;
  real_t *ubvec;

  real_t iotimer;
  real_t parttimer;
  real_t reporttimer;

  size_t maxmemory;
} params_t;

#endif
