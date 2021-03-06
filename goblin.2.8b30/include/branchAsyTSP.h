
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, September 2001
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   branchAsyTSP.h
/// \brief  #branchAsyTSP class interface

#ifndef _BRANCH_ASY_TSP_H_
#define _BRANCH_ASY_TSP_H_

#include "branchScheme.h"
#include "sparseDigraph.h"


/// \addtogroup tsp
/// @{

/// \brief  Branch & bound implementation for asymmetric TSP

class branchAsyTSP : public branchNode<TArc,TFloat>
{
public:

    abstractMixedGraph &G; // The original graph
    sparseDiGraph *X;      // The copy which is manipulated
    THandle H;             // ... for searching this copy
    TNode selected;        // Cardinality of the partial tour
    TNode root;            // The excluded node of the tree subproblem
    abstractMixedGraph::TRelaxTSP relaxationMethod;

    branchAsyTSP(abstractMixedGraph& _G,TNode _root,
        abstractMixedGraph::TRelaxTSP method,int nCandidates) throw();
    branchAsyTSP(branchAsyTSP& Node) throw();
    ~branchAsyTSP() throw();

    unsigned long   Size() const throw();
    unsigned long   Allocated() const throw();

    void  SetCandidateGraph(int nCandidates) throw();     // Compute candidate arcs

    TArc            SelectVariable() throw();        // Select variable to branch with
    TBranchDir      DirectionConstructive(TArc i) throw(ERRange);
    TBranchDir      DirectionExhaustive(TArc i) throw(ERRange);
                        // Select branching direction

    branchNode<TArc,TFloat>* Clone() throw();    // Generate a copy of the branch node
    void            Raise(TArc i) throw(ERRange);
    void            Raise(TArc a,bool) throw(ERRange); // Raise lower variable bound
    void            Lower(TArc i) throw(ERRange);
    void            Lower(TArc a,bool) throw(ERRange); // Lower upper variable bound
    void            CheckNode(TNode) throw(ERRange); // Detect deg 2 nodes and restrict
    TFloat          SolveRelaxation() throw();   // Returns the objective value of the relaxed
                                                 // subproblem and a corresponding solution
    TObjectSense    ObjectSense() const throw();
    TFloat          Infeasibility() const throw();

    bool            Feasible() throw();          // Is the relaxed optimum feasible ?
    TFloat          LocalSearch() throw();       // Refine the B&B solution
    void            SaveSolution() throw();      // Copy the current solution to the original instance

};

/// @}

#endif
