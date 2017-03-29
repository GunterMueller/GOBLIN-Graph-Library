
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, March 2001
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   goshGraph.cpp
/// \brief  Tcl shell command procedures for the GOBLIN library graph objects

#include "gosh.h"


int Goblin_Mixed_Graph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    mixedGraph* G = reinterpret_cast<mixedGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        int ret = Goblin_Sparse_Cmd(G,interp,argc,argv);

        if (ret==GOSH_OK)    return TCL_OK;
        if (ret==GOSH_ERROR) return TCL_ERROR;

        return Goblin_Generic_Graph_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Mixed_Graph (ClientData clientData) throw()
{
    mixedGraph* thisObject = reinterpret_cast<mixedGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Sparse_Graph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    sparseGraph* G = reinterpret_cast<sparseGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        int ret = Goblin_Sparse_Cmd(G,interp,argc,argv);

        if (ret==GOSH_OK)    return TCL_OK;
        if (ret==GOSH_ERROR) return TCL_ERROR;

        return Goblin_Undirected_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Sparse_Graph (ClientData clientData) throw()
{
    sparseGraph* thisObject = reinterpret_cast<sparseGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Dense_Graph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    denseGraph *G = reinterpret_cast<denseGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        return Goblin_Undirected_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Dense_Graph (ClientData clientData) throw()
{
    denseGraph* thisObject = reinterpret_cast<denseGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Sparse_Bigraph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    sparseBiGraph *G = reinterpret_cast<sparseBiGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        if (argc==4 && strcmp(argv[1],"node")==0 && strcmp(argv[3],"swap")==0)
        {
            TNode u = atol(argv[2]);
            TNode v = G -> SwapNode(u);
            sprintf(interp->result,"%lu",static_cast<unsigned long>(v));
            return TCL_OK;
        }

        int ret = Goblin_Sparse_Cmd(G,interp,argc,argv);

        if (ret==GOSH_OK)    return TCL_OK;
        if (ret==GOSH_ERROR) return TCL_ERROR;

        return Goblin_Bipartite_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Sparse_Bigraph (ClientData clientData) throw()
{
    sparseBiGraph* thisObject = reinterpret_cast<sparseBiGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Dense_Bigraph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    denseBiGraph *G = reinterpret_cast<denseBiGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        return Goblin_Bipartite_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Dense_Bigraph (ClientData clientData) throw()
{
    denseBiGraph* thisObject = reinterpret_cast<denseBiGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Sparse_Digraph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    sparseDiGraph *G = reinterpret_cast<sparseDiGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        int ret = Goblin_Sparse_Cmd(G,interp,argc,argv);

        if (ret==GOSH_OK)    return TCL_OK;
        if (ret==GOSH_ERROR) return TCL_ERROR;

        return Goblin_Directed_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Sparse_Digraph (ClientData clientData) throw()
{
    sparseDiGraph* thisObject = reinterpret_cast<sparseDiGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Dense_Digraph_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    denseDiGraph *G = reinterpret_cast<denseDiGraph*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        return Goblin_Directed_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Dense_Digraph (ClientData clientData) throw()
{
    denseDiGraph* thisObject = reinterpret_cast<denseDiGraph*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Balanced_FNW_Cmd (ClientData clientData,Tcl_Interp* interp,
    int argc,_CONST_QUAL_ char* argv[])
{
    balancedFNW *G = reinterpret_cast<balancedFNW*>(clientData);

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        if (strcmp(argv[1],"balancedFlow")==0)
        {
            TNode sourceNode = NoNode;

            int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);

            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                sourceNode = atol(argv[pos+1]);
            }

            if (CT->FindParam(argc,&argv[0],"-maximize",2))
            {
                G->MaxBalFlow(sourceNode);

                try
                {
                    sprintf(interp->result,"%f",G->FlowValue(sourceNode,sourceNode^1));
                    return TCL_OK;
                }
                catch (ERCheck)
                {
                    sprintf(interp->result,"Flow is corrupted");
                    return TCL_ERROR;
                }
            }
            else
            {
                TFloat ret = G -> MinCBalFlow(sourceNode);

                try
                {
                    G -> FlowValue(sourceNode,sourceNode^1);
                    sprintf(interp->result,"%f",ret);
                    return TCL_OK;
                }
                catch (ERCheck)
                {
                    interp->result = "Flow is corrupted";
                    return TCL_ERROR;
                }
            }
        }

        int ret = Goblin_Sparse_Cmd(G,interp,argc,argv);

        if (ret==GOSH_OK)    return TCL_OK;
        if (ret==GOSH_ERROR) return TCL_ERROR;

        return Goblin_Directed_Cmd(G,interp,argc,argv);
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }
}


int Goblin_Delete_Balanced_FNW (ClientData clientData) throw()
{
    balancedFNW* thisObject = reinterpret_cast<balancedFNW*>(clientData);
    goblinController* thisContext = &(thisObject->Context());

    if (destroyThread) return TCL_OK;

    if (thisContext==CT)
    {
        int savedTraceLevel = thisContext->traceLevel;
        thisContext->traceLevel = 0;
        delete thisObject;
        thisContext->traceLevel = savedTraceLevel;
    }
    else
    {
        thisContext->traceLevel = 0;
        delete thisObject;
        if (!thisContext->IsReferenced()) delete thisContext;
    }

    return TCL_OK;
}


int Goblin_Sparse_Cmd (abstractMixedGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (strcmp(argv[1],"reorder")==0)
    {
        if (argc!=4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"incidences")==0)
        {
            if (strcmp(argv[3],"-planar")==0)
            {
                if (G->PlanarizeIncidenceOrder())
                    interp->result = "1";
                else interp->result = "0";
                return GOSH_OK;
            }

            if (strcmp(argv[3],"-shuffle")==0)
            {
                G -> RandomizeIncidenceOrder();
                interp->result = "";
                return GOSH_OK;
            }

            if (strcmp(argv[3],"-geometric")==0)
            {
                G -> IncidenceOrderFromDrawing();
                interp->result = "";
                return GOSH_OK;
            }

            if (strcmp(argv[3],"-outerplanar")==0)
            {
                G -> GrowExteriorFace();
                interp->result = "";
                return GOSH_OK;
            }

            return GOSH_UNHANDLED;
        }

        if (strcmp(argv[2],"nodeIndices")==0)
        {
            sparseRepresentation* GR =
                static_cast<sparseRepresentation*>(G->Representation());

            TFloat* keyValue = new TFloat[G->N()];

            if (strcmp(argv[3],"-colours")==0)
            {
                for (TNode v=0;v<G->N();++v) keyValue[v] = G->NodeColour(v);
            }
            else if (strcmp(argv[3],"-demands")==0)
            {
                for (TNode v=0;v<G->N();++v) keyValue[v] = G->Demand(v);
            }
            else if (strcmp(argv[3],"-shuffle")==0)
            {
                for (TNode v=0;v<G->N();++v) keyValue[v] = TFloat(rand());
            }
            else
            {
                delete[] keyValue;
                interp->result = "Missing key value specification";
                return TCL_OK;
            }

            GR -> ReorderNodeIndices(keyValue);

            delete[] keyValue;
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"edgeIndices")==0)
        {
            sparseRepresentation* GR =
                static_cast<sparseRepresentation*>(G->Representation());

            TFloat* keyValue = new TFloat[G->M()];

            if (strcmp(argv[3],"-colours")==0)
            {
                for (TArc a=0;a<G->M();++a) keyValue[a] = G->EdgeColour(2*a);
            }
            else if (strcmp(argv[3],"-lengths")==0)
            {
                for (TArc a=0;a<G->M();++a) keyValue[a] = G->Length(2*a);
            }
            else if (strcmp(argv[3],"-shuffle")==0)
            {
                for (TArc a=0;a<G->M();++a) keyValue[a] = TFloat(rand());
            }
            else
            {
                delete[] keyValue;
                interp->result = "Missing key value specification";
                return TCL_OK;
            }

            GR -> ReorderEdgeIndices(keyValue);

            delete[] keyValue;
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s reorder %s",argv[0],argv[2]);
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"seriesParallel")==0)
    {
        unsigned options = G->ESP_DIRECTED;
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;

        if (CT->FindParam(argc,argv,"-embedding",2)) options |= G->ESP_EMBEDDING;

        if (CT->FindParam(argc,argv,"-undirected",2) || G->IsUndirected())
        {
            options ^= G->ESP_DIRECTED;
        }

        if (CT->FindParam(argc,argv,"-orient",2)) options |= G->ESP_ORIENT;

        if (CT->FindParam(argc,argv,"-layout",2)) options |= G->ESP_VISIBILITY;

        if (CT->FindParam(argc,argv,"-minor",2)) options |= G->ESP_MINOR;

        int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            sourceNode = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-targetNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            targetNode = atol(argv[pos+1]);
        }

        if (G->EdgeSeriesParallelMethod(abstractMixedGraph::TOptSeriesParallel(options),
                    sourceNode,targetNode)
           )
        {
            interp->result = "";
            return GOSH_OK;
        }
        else
        {
            interp->result = "Graph is not edge series parallel";
            return GOSH_ERROR;
        }
    }

    return GOSH_UNHANDLED;
}


int Goblin_Bipartite_Cmd (abstractBiGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (strcmp(argv[1],"#leftHand")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>((G->N1())));
        return TCL_OK;
    }

    if (strcmp(argv[1],"#rightHand")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>((G->N2())));
        return TCL_OK;
    }

    return Goblin_Undirected_Cmd(G,interp,argc,argv);
}


int Goblin_Undirected_Cmd (abstractGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<2)
    {
        interp->result = "Missing arguments";
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"explicitSubgraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new sparseGraph(*G,
            managedObject::OPT_SUB|managedObject::OPT_PARALLELS);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"metricGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        denseGraph *H = new metricGraph(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Dense_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"maximumMatching")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TFloat ret = G -> MaximumMatching();

        sprintf(interp->result,"%f",ret);
        return TCL_OK;
    }

    if (strcmp(argv[1],"minimumCostMatching")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (G -> MinCMatching())
        {
            sprintf(interp->result,"%f",G->Weight());
            return TCL_OK;
        }
        else
        {
            interp->result = "No such structure exists";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"edgeCover")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TFloat ret = G -> MinCEdgeCover();

        sprintf(interp->result,"%f",ret);
        return TCL_OK;
    }

    if (strcmp(argv[1],"tJoin")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        try
        {
            G -> MinCTJoin(demandNodes(*G));
            sprintf(interp->result,"%f",G->Weight());
            return TCL_OK;
        }
        catch (ERRejected)
        {
            interp->result = "No such structure exists";
            return TCL_ERROR;
        }
    }

    return Goblin_Generic_Graph_Cmd(G,interp,argc,argv);
}


int Goblin_Directed_Cmd (abstractDiGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<2)
    {
        interp->result = "Missing arguments";
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"explicitSubgraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new sparseDiGraph(*G,
            managedObject::OPT_SUB|managedObject::OPT_PARALLELS);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"transitiveClosure")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new transitiveClosure(*G,managedObject::OPT_SUB);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"intransitiveReduction")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new intransitiveReduction(*G,managedObject::OPT_SUB);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"linearFlowModel")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        mipInstance* XLP = static_cast<mipInstance*>(G->BFlowToLP());

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(XLP),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"splitGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        balancedFNW *H = new splitGraph(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Balanced_FNW_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Balanced_FNW);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"topSort")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TNode v = G->TopSort();

        if (v==NoNode)
        {
            interp->result = "*";
            return TCL_OK;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(v));
        return TCL_OK;
    }

    if (strcmp(argv[1],"criticalPath")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TNode v = G->CriticalPath();

        if (v==NoNode)
        {
            interp->result = "Graph is not a DAG";
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(v));
        return TCL_OK;
    }

    if (strcmp(argv[1],"treePacking")==0)
    {
        TNode rootNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-rootNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            rootNode = atol(argv[pos+1]);
        }

        TCap ret = G->TreePacking(rootNode);
        sprintf(interp->result,"%f",ret);
        return TCL_OK;
    }

    return Goblin_Generic_Graph_Cmd(G,interp,argc,argv);
}


int Goblin_Generic_Graph_Cmd (abstractMixedGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<2)
    {
        interp->result = "Missing arguments";
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"displayProxy")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        graphDisplayProxy *DP = new graphDisplayProxy(*G,CT->pixelWidth,CT->pixelHeight);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Graph_Display_Proxy_Cmd,reinterpret_cast<ClientData>(DP),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Graph_Display_Proxy);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"explicitSubgraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        mixedGraph *H = new mixedGraph(*G,
            managedObject::OPT_SUB|managedObject::OPT_PARALLELS);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"lineGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (CT->FindParam(argc,argv,"-planar",2))
        {
            sparseGraph *H = new planarLineGraph(*G);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,(ClientData)H,
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);
        }
        else if (CT->FindParam(argc,argv,"-directed",2))
        {
            mixedGraph *H = new lineGraph(*G,lineGraph::LG_DIRECTED);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);
        }
        else
        {
            mixedGraph *H = new lineGraph(*G,lineGraph::LG_UNDIRECTED);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"dualGraph")==0)
    {
        if (argc>4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (argc==3 || strcmp(argv[2],"-directed")!=0)
        {
            sparseGraph *H = new dualGraph(*G,(TOption)0);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);
        }
        else
        {
            sparseDiGraph *H = new directedDual(*G,(TOption)0);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"spreadOutRegular")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new spreadOutRegular(*G,(TOption)0);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"vertexTruncation")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new vertexTruncation(*G,(TOption)0);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"facetSeparation")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph* H = NULL;

        if (argc==4 && strcmp(argv[2],"-turnLeft")==0)
        {
            H = new facetSeparation(*G,facetSeparation::ROT_LEFT);
        }
        else if (argc==4 && strcmp(argv[2],"-turnRight")==0)
        {
            H = new facetSeparation(*G,facetSeparation::ROT_RIGHT);
        }
        else
        {
            H = new facetSeparation(*G,facetSeparation::ROT_NONE);
        }

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"mycielskianGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new mycielskianGraph(*G,mycielskianGraph::OPT_SUB);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"complementaryGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new complementaryGraph(*G,(TOption)0);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"underlyingGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseGraph *H = new sparseGraph(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"inducedSubgraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TNode nodeColour = 0;
        TArc  edgeColour = NoArc;

        int pos = CT->FindParam(argc,&argv[0],"-edgeColour",2);

        if (pos>0 && pos<argc-1)
        {
            edgeColour = TArc(atol(argv[pos+1]));
            nodeColour = NoNode;
        }

        pos = CT->FindParam(argc,&argv[0],"-nodeColour",2);

        if (pos>0 && pos<argc-1)
        {
            nodeColour = TNode(atol(argv[pos+1]));
        }

        mixedGraph *H = NULL;

        if (edgeColour==NoArc)
        {
            H = new inducedSubgraph(*G,colouredNodes(*G,nodeColour),TOption(0));
        }
        else if (nodeColour==NoNode)
        {
            H = new inducedSubgraph(*G,fullIndex<TNode>(G->N(),G->Context()),
                                        colouredArcs(*G,edgeColour),TOption(0));
        }
        else
        {
            H = new inducedSubgraph(*G,colouredNodes(*G,nodeColour),
                                        colouredArcs(*G,edgeColour),TOption(0));
        }

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"inducedOrientation")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new inducedOrientation(*G,managedObject::OPT_PARALLELS);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"inducedBigraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        TNode leftHandColour  = 0;
        TNode rightHandColour = 1;

        int pos = CT->FindParam(argc,&argv[0],"-nodeColours",2);

        if (pos>0 && pos<argc-2)
        {
            leftHandColour  = TNode(atol(argv[pos+1]));
            rightHandColour = TNode(atol(argv[pos+2]));
        }

        sparseBiGraph *H = new inducedBigraph(*G,colouredNodes(*G,leftHandColour),
            colouredNodes(*G,rightHandColour),TOption(0));

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Bigraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Bigraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"colourContraction")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        mixedGraph *H = new colourContraction(*G,TOption(0));

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"nodeSplitting")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new nodeSplitting(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"completeOrientation")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sparseDiGraph *H = new sparseDiGraph(*G,managedObject::OPT_PARALLELS);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"explicitSubdivision")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        mixedGraph *H = new explicitSubdivision(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"distanceGraph")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        denseDiGraph *H = new distanceGraph(*G);

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Dense_Digraph_Cmd,reinterpret_cast<ClientData>(H),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Digraph);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"integerStableSetModel")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        mipInstance* XLP =
            static_cast<mipInstance*>(G->StableSetToMIP());

        Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
            Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(XLP),
            (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"merge")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        goblinRootObject *X =
            (G->Context()).ObjectPointer(THandle(atol(argv[argc-1])));

        if (!X->IsGraphObject())
        {
            sprintf(interp->result,"Not a graph object ID: %s",argv[argc-1]);
            return TCL_ERROR;
        }

        abstractMixedGraph* Y = dynamic_cast<abstractMixedGraph*>(X);

        if (Y==NULL || !(Y->IsSparse()))
        {
            interp->result = "Unhandled object type";
            return TCL_ERROR;
        }

        int pos = CT->FindParam(argc,&argv[0],"-facets",2);

        if (pos>0 && pos<argc-1)
        {
            G -> FacetIdentification(*Y);
        }
        else
        {
            abstractMixedGraph::TMergeLayoutMode mergeLayoutMode = G->MERGE_OVERLAY;

            int pos = CT->FindParam(argc,&argv[0],"-right",2);

            if (pos>0 && pos<argc-1)
            {
                mergeLayoutMode = G->MERGE_ALIGN_RIGHT;
            }

            pos = CT->FindParam(argc,&argv[0],"-below",2);

            if (pos>0 && pos<argc-1)
            {
                mergeLayoutMode = G->MERGE_ALIGN_BELOW;
            }

            G -> AddGraphByNodes(*Y,mergeLayoutMode);
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"layout")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"point")==0)
        {
            if (argc<5)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            TNode p = TNode(atol(argv[3]));

            if (strcmp(argv[4],"placeAt")==0)
            {
                if (argc==5)
                {
                    interp->result = "Missing coordinate values";
                    return TCL_ERROR;
                }

                for (int i=5;i<argc;++i)
                {
                    TFloat pos = TFloat(atof(argv[i]));

                    G->Representation() -> SetC(p,TDim(i-5),pos);
                }

                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[4],"insertSuccessor")==0)
            {
                if (!G->IsSparse())
                {
                    interp->result = "Operation applies to sparse graphs only";
                    return TCL_ERROR;
                }

                sparseRepresentation* GR = static_cast<sparseRepresentation*>(G->Representation());

                sprintf(interp->result,"%lu",static_cast<unsigned long>(GR->InsertThreadSuccessor(p)));
                return TCL_OK;
            }

            if (strcmp(argv[4],"info")==0)
            {
                if (strcmp(argv[5],"-cx")==0)
                {
                    sprintf(interp->result,"%f",G->C(p,0));
                }
                else if (strcmp(argv[5],"-cy")==0)
                {
                    sprintf(interp->result,"%f",G->C(p,1));
                }
                else if (strcmp(argv[5],"-successor")==0)
                {
                    if (G->ThreadSuccessor(p)!=NoNode)
                    {
                        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->ThreadSuccessor(p)));
                    }
                    else interp->result = "*";
                }
                else if (strcmp(argv[5],"-hidden")==0)
                {
                    sprintf(interp->result,"%d",G->HiddenNode(p));
                }
                else
                {
                    sprintf(interp->result,"Unknown layout point attribute: %s",argv[5]);
                    return TCL_ERROR;
                }

                return TCL_OK;
            }
        }

        if (strcmp(argv[2],"alignWithOrigin")==0)
        {
            G -> Layout_AlignWithOrigin();
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"#points")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(G->L()));
            return TCL_OK;
        }

        if (strcmp(argv[2],"boundingBox")==0)
        {
            if (strcmp(argv[3],"freeze")==0)
            {
                G -> Layout_FreezeBoundingBox();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"default")==0)
            {
                G -> Layout_DefaultBoundingBox();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"release")==0)
            {
                G -> Layout_ReleaseBoundingBox();
                interp->result = "";
                return TCL_OK;
            }

            TDim coordinate = NoCoord;

            int pos = CT->FindParam(argc,&argv[0],"-coordinate",4);

            if (pos<=0 || pos>=argc-1)
            {
                interp->result = "Missing value for parameter \"-coordinate\"";
                return TCL_ERROR;
            }

            coordinate = TDim(atoi(argv[pos+1]));

            if (coordinate>=G->Dim())
            {
                interp->result = "Invalid coordinate index";
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"transform")==0)
            {
                pos = CT->FindParam(argc,&argv[0],"-range",4);

                if (pos<=0 || pos>=argc-2)
                {
                    interp->result = "Missing values for parameter \"-range\"";
                    return TCL_ERROR;
                }

                TFloat cMin = TFloat(atol(argv[pos+1]));
                TFloat cMax = TFloat(atol(argv[pos+2]));

                G -> Layout_TransformCoordinate(coordinate,cMin,cMax);
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"info")==0)
            {
                TFloat cMin = 0.0,cMax = 0.0;
                G -> Layout_GetBoundingInterval(coordinate,cMin,cMax);

                pos = CT->FindParam(argc,&argv[0],"-max",4);

                if (pos>0 && pos<argc)
                {
                    sprintf(interp->result,"%f",cMax);
                    return TCL_OK;
                }

                pos = CT->FindParam(argc,&argv[0],"-min",4);

                if (pos>0 && pos<argc)
                {
                    sprintf(interp->result,"%f",cMin);
                    return TCL_OK;
                }

                pos = CT->FindParam(argc,&argv[0],"-gridSize",4);

                if (pos>0 && pos<argc)
                {
                    TFloat spacing = 1.0;
                    G -> GetLayoutParameter(TokLayoutBendSpacing,spacing);
                    sprintf(interp->result,"%lu",static_cast<unsigned long>((cMax-cMin)/spacing));
                    return TCL_OK;
                }

                interp->result = "Missing parameter specification";
                return TCL_ERROR;
            }

            sprintf(interp->result,"Unknown option: %s layout boundingBox %s",argv[0],argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"configure")==0)
        {
            if (argc<5 || (argc%2)==0)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            int keyCount = 3;

            while (keyCount<argc)
            {
                if (   argv[keyCount][0]!='-'
                    || !G->SetLayoutParameter(&(argv[keyCount][1]),argv[keyCount+1]))
                {
                    sprintf(interp->result,"Invalid assignment: %s layout configure %s %s",
                        argv[0],argv[keyCount],argv[keyCount+1]);
                    return TCL_ERROR;
                }

                keyCount += 2;
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"info")==0)
        {
            if (argc<4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"-exists")==0)
            {
                if (G->Dim()>0 && G->CMax(0)>-100000 && G->CMax(1)>-100000)
                {
                    interp->result = "1";
                }
                else
                {
                    interp->result = "0";
                }
            }
            else if (!G->GetLayoutParameter(&(argv[3][1]),interp->result))
            {
                sprintf(interp->result,"Unknown layout parameter %s",argv[3]);
                return TCL_ERROR;
            }

            return TCL_OK;
        }

        int pos = CT->FindParam(argc,&argv[0],"-spacing",3);
        TFloat spacing = 0;

        if (pos>0)
        {
            if (pos<argc-1)
            {
                spacing = atof(argv[pos+1]);
            }
            else
            {
                interp->result = "Missing value for parameter \"-spacing\"";
                return TCL_ERROR;
            }
        }

        if (strcmp(argv[2],"arcRouting")==0)
        {
            if (!G->IsSparse())
            {
                interp->result = "Operation applies to sparse graphs only";
                return TCL_ERROR;
            }

            static_cast<sparseRepresentation*>(G->Representation()) -> Layout_ArcRouting(spacing);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"tree")==0)
        {
            TFloat dx = spacing;
            TFloat dy = spacing;

            pos = CT->FindParam(argc,&argv[0],"-dx",3);

            if (pos>0)
            {
                if (pos<argc-1)
                {
                    dx = atof(argv[pos+1]);
                }
                else
                {
                    interp->result = "Missing value for parameter \"-dx\"";
                    return TCL_ERROR;
                }
            }

            pos = CT->FindParam(argc,&argv[0],"-dy",3);

            if (pos>0)
            {
                if (pos<argc-1)
                {
                    dy = atof(argv[pos+1]);
                }
                else
                {
                    interp->result = "Missing value for parameter \"-dy\"";
                    return TCL_ERROR;
                }
            }

            abstractMixedGraph::TOptAlign mode =
                abstractMixedGraph::ALIGN_OPTIMIZE;

            if (CT->FindParam(argc,argv,"-left",3))
            {
                mode = abstractMixedGraph::ALIGN_LEFT;
            }
            else if (CT->FindParam(argc,argv,"-right",3))
            {
                mode = abstractMixedGraph::ALIGN_RIGHT;
            }
            else if (CT->FindParam(argc,argv,"-fdp",3))
            {
                mode = abstractMixedGraph::ALIGN_FDP;
            }
            else if (CT->FindParam(argc,argv,"-center",3))
            {
                mode = abstractMixedGraph::ALIGN_CENTER;
            }

            try
            {
                G -> Layout_PredecessorTree(mode,dx,dy);
            }
            catch (ERRejected) {}

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"fdp")==0)
        {
            bool restricted = false;

            if (CT->FindParam(argc,argv,"-preserve",3)) restricted = true;

            if (CT->FindParam(argc,argv,"-layered",3))
            {
                if (restricted)
                {
                    G -> Layout_ForceDirected(
                        abstractMixedGraph::FDP_LAYERED_RESTR,int(spacing));
                }
                else
                {
                    G -> Layout_ForceDirected(
                        abstractMixedGraph::FDP_LAYERED,int(spacing));
                }
            }
            else if (restricted)
            {
                G -> Layout_ForceDirected(
                        abstractMixedGraph::FDP_RESTRICTED,int(spacing));
            }
            else if (CT->FindParam(argc,argv,"-unrestricted",3))
            {
                G -> Layout_ForceDirected(
                        abstractMixedGraph::FDP_GEM,int(spacing));
            }
            else
            {
                G -> Layout_ForceDirected(
                        abstractMixedGraph::FDP_DEFAULT,int(spacing));
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"layered")==0)
        {
            TFloat dx = spacing;
            TFloat dy = spacing;

            pos = CT->FindParam(argc,&argv[0],"-dx",3);

            if (pos>0)
            {
                if (pos<argc-1)
                {
                    dx = atof(argv[pos+1]);
                }
                else
                {
                    G->GetLayoutParameter(TokLayoutBendSpacing,dx);
                }
            }

            pos = CT->FindParam(argc,&argv[0],"-dy",3);

            if (pos>0)
            {
                if (pos<argc-1)
                {
                    dy = atof(argv[pos+1]);
                }
                else
                {
                    G->GetLayoutParameter(TokLayoutBendSpacing,dy);
                }
            }

            int method = 0;

            pos = CT->FindParam(argc,&argv[0],"-colours",3);
            if (pos>0) method |= G->LAYERED_COLOURS;

            pos = CT->FindParam(argc,&argv[0],"-span",3);
            if (pos>0) method |= G->LAYERED_EDGE_SPAN;

            pos = CT->FindParam(argc,&argv[0],"-vertical",3);
            if (pos>0) method |= G->LAYERED_VERTICAL;

            pos = CT->FindParam(argc,&argv[0],"-sweep",3);
            if (pos>0) method |= G->LAYERED_SWEEP;

            pos = CT->FindParam(argc,&argv[0],"-align",3);
            if (pos>0) method |= G->LAYERED_ALIGN;

            pos = CT->FindParam(argc,&argv[0],"-fdp",3);
            if (pos>0) method |= G->LAYERED_FDP;

            pos = CT->FindParam(argc,&argv[0],"-horizontal",3);
            if (pos>0) method |= G->LAYERED_HORIZONTAL;

            if (method==0)
            {
                G -> Layout_Layered(G->LAYERED_DEFAULT,dx,dy);
            }
            else
            {
                G -> Layout_Layered(method,dx,dy);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"circular")==0)
        {
            if (CT->FindParam(argc,argv,"-predecessors",3))
            {
                 G -> Layout_CircularByPredecessors(spacing);
            }
            else if (CT->FindParam(argc,argv,"-colours",3))
            {
                 G -> Layout_CircularByColours(spacing);
            }
            else if (CT->FindParam(argc,argv,"-outerplanar",3))
            {
                if (!(G->Layout_Outerplanar(spacing)))
                {
                    interp->result = "No outerplanar embedding given";
                    return TCL_ERROR;
                }
            }
            else
            {
                G -> Layout_Circular(spacing);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"equilateral")==0)
        {
            G -> Layout_Equilateral(spacing);

            interp->result = "";
            return TCL_OK;
        }

        pos = CT->FindParam(argc,&argv[0],"-grid",3);
        int grid = int(spacing);

        if (pos>0)
        {
            if (pos<argc-1) grid = atol(argv[pos+1]);
            else
            {
                interp->result = "Missing value for parameter \"-grid\"";
                return TCL_ERROR;
            }
        }

        if (strcmp(argv[2],"plane")==0)
        {
            TArc aBasis = NoArc;

            pos = CT->FindParam(argc,&argv[0],"-basis",3);

            if (pos>0)
            {
                if (pos<argc-1) aBasis = atol(argv[pos+1]);
                else
                {
                    interp->result = "Missing value for parameter \"-basis\"";
                    return TCL_ERROR;
                }
            }

            if (CT->FindParam(argc,argv,"-convex",3))
            {
                G -> Layout_ConvexDrawing(aBasis,grid);
            }
            else
            {
                G -> Layout_StraightLineDrawing(aBasis,grid);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"orthogonal")==0)
        {
            if (CT->FindParam(argc,argv,"-tree",3))
            {
                TNode rootNode = NoNode;

                pos = CT->FindParam(argc,&argv[0],"-rootNode",3);

                if (pos>0)
                {
                    if (pos<argc-1) rootNode = atol(argv[pos+1]);
                    else
                    {
                        interp->result = "Missing value for parameter \"-rootNode\"";
                        return TCL_ERROR;
                    }
                }

                if (CT->FindParam(argc,argv,"-binary",3))
                {
                    G -> Layout_HorizontalVerticalTree(rootNode,grid);
                }
                else
                {
                    G -> Layout_KandinskyTree(rootNode,grid);
                }
            }
            else if (CT->FindParam(argc,argv,"-small",3))
            {
                G -> Layout_OrthogonalDeg4(
                        abstractMixedGraph::ORTHO_4PLANAR,grid);
            }
            else if (CT->FindParam(argc,argv,"-staircase",3))
            {
                G -> Layout_StaircaseTriconnected(NoArc,grid);
            }
            else if (CT->FindParam(argc,argv,"-planar",3))
            {
                G -> Layout_StaircaseDrawing(NoArc,grid);
            }
            else
            {
                G -> Layout_Kandinsky(abstractMixedGraph::ORTHO_DEFAULT,grid);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"visibility")==0)
        {
            if (CT->FindParam(argc,argv,"-giotto",3))
            {
                G -> Layout_VisibilityRepresentation(
                    abstractMixedGraph::ORTHO_VISIBILITY_GIOTTO,grid);
            }
            else if (CT->FindParam(argc,argv,"-raw",3))
            {
                G -> Layout_VisibilityRepresentation(
                    abstractMixedGraph::ORTHO_VISIBILITY_RAW,grid);
            }
            else if (CT->FindParam(argc,argv,"-seriesParallel",3))
            {
                abstractMixedGraph::TOptSeriesParallel options =
                    abstractMixedGraph::TOptSeriesParallel(
                        G->ESP_VISIBILITY|G->ESP_DIRECTED);
                G -> EdgeSeriesParallelMethod(options);
            }
            else
            {
                G -> Layout_VisibilityRepresentation(
                    abstractMixedGraph::ORTHO_VISIBILITY_TRIM,grid);
            }

            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s layout %s",argv[0],argv[2]);
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"extract")==0)
    {
        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"tree")==0)
        {
            TNode rootNode = NoNode;

            int pos = CT->FindParam(argc,&argv[0],"-rootNode",2);

            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                rootNode = atol(argv[pos+1]);
            }

            try
            {
                G -> ExtractTree(rootNode);
                interp->result = "1";
            }
            catch (ERCheck)
            {
                interp->result = "Invalid input data";
                return TCL_ERROR;
            }

            return TCL_OK;
        }

        if (strcmp(argv[2],"path")==0)
        {
            TNode sourceNode = NoNode;
            TNode targetNode = NoNode;

            int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);

            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                sourceNode = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-targetNode",2);

            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                targetNode = atol(argv[pos+1]);
            }

            try
            {
                G -> ExtractPath(sourceNode,targetNode);
                interp->result = "1";
            }
            catch (ERCheck)
            {
                interp->result = "Invalid input data";
                return TCL_ERROR;
            }

            return TCL_OK;
        }

        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"cycles")==0)
        {
            try
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->ExtractCycles()));
            }
            catch (ERCheck)
            {
                interp->result = "Invalid input data";
                return TCL_ERROR;
            }

            return TCL_OK;
        }

        try
        {
            interp->result = "1";

            if (strcmp(argv[2],"matching")==0)
            {
                G -> Extract1Matching();
                return TCL_OK;
            }

            if (strcmp(argv[2],"edgeCover")==0)
            {
                G -> ExtractEdgeCover();
                return TCL_OK;
            }

            if (strcmp(argv[2],"trees")==0)
            {
                G -> ExtractTrees();
                return TCL_OK;
            }

            if (strcmp(argv[2],"cut")==0)
            {
                G -> ExtractCut();
                return TCL_OK;
            }

            if (strcmp(argv[2],"bipartition")==0)
            {
                G -> ExtractBipartition();
                return TCL_OK;
            }

            if (strcmp(argv[2],"colours")==0)
            {
                G -> ExtractColours();
                return TCL_OK;
            }

            interp->result = "1";
        }
        catch (ERCheck)
        {
            interp->result = "Invalid input data";
            return TCL_ERROR;
        }
        catch (ERRejected)
        {
            interp->result = "Invalid input data";
            return TCL_ERROR;
        }

        sprintf(interp->result,"Unknown option: %s extract %s",argv[0],argv[2]);
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"delete")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        Tcl_DeleteCommand(interp,argv[0]);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"release")==0)
    {
        for (int i=2;i<argc;++i)
        {
            if (strcmp(argv[i],"-subgraph")==0)
            {
                G -> InitSubgraph();
            }
            else if (strcmp(argv[i],"-distance")==0)
            {
                G -> ReleaseLabels();
            }
            else if (strcmp(argv[i],"-predecessorArc")==0)
            {
                G -> ReleasePredecessors();
            }
            else if (strcmp(argv[i],"-nodeColour")==0)
            {
                G -> ReleaseNodeColours();
            }
            else if (strcmp(argv[i],"-edgeColour")==0)
            {
                G -> ReleaseEdgeColours();
            }
            else if (strcmp(argv[i],"-potential")==0)
            {
                G -> ReleasePotentials();
            }
            else if (strcmp(argv[i],"-partition")==0)
            {
                G -> ReleasePartition();
            }
            else
            {
                sprintf(interp->result,"Unknown register attribute: %s",argv[i]);
                return TCL_ERROR;
            }
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"#nodes")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->N()));
        return TCL_OK;
    }

    if (strcmp(argv[1],"#edges")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->M()));
        return TCL_OK;
    }

    if (strcmp(argv[1],"info")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        bool keyFound = true;

        if (strcmp(argv[2],"-sourceNode")==0)
        {
            TNode s = G->DefaultSourceNode();

            if (s==NoNode) interp->result = "*";
            else sprintf(interp->result,"%lu",static_cast<unsigned long>(s));
        }
        else if (strcmp(argv[2],"-targetNode")==0)
        {
            TNode t = G->DefaultTargetNode();

            if (t==NoNode) interp->result = "*";
            else sprintf(interp->result,"%lu",static_cast<unsigned long>(t));
        }
        else if (strcmp(argv[2],"-rootNode")==0)
        {
            TNode r = G->DefaultRootNode();

            if (r==NoNode) interp->result = "*";
            else sprintf(interp->result,"%lu",static_cast<unsigned long>(r));
        }
        else if (strcmp(argv[2],"-metricType")==0)
        {
            sprintf(interp->result,"%d",G->MetricType());
        }
        else if (strcmp(argv[2],"-sparse")==0)
        {
            sprintf(interp->result,"%s", (G->IsSparse()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-directed")==0)
        {
            sprintf(interp->result,"%s", (G->IsDirected()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-undirected")==0)
        {
            sprintf(interp->result,"%s", (G->IsUndirected()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-bipartite")==0)
        {
            sprintf(interp->result,"%s", (G->IsBipartite()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-planar")==0)
        {
            sprintf(interp->result,"%s", (G->IsPlanar()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-chordal")==0)
        {
            sprintf(interp->result,"%s", (G->IsChordal()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-co-chordal")==0)
        {
            sprintf(interp->result,"%s", (G->IsChordal(abstractMixedGraph::PERFECT_COMPLEMENT)) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-balanced")==0)
        {
            sprintf(interp->result,"%s", (G->IsBalanced()) ? "1" : "0");
        }
        else if (strcmp(argv[2],"-graphObject")==0)
        {
            interp->result = "1";
        }
        else if (strcmp(argv[2],"-cardinality")==0)
        {
            sprintf(interp->result,"%f",G->Cardinality());
        }
        else if (strcmp(argv[2],"-edgeLength")==0)
        {
            sprintf(interp->result,"%f",G->Length());
        }
        else if (strcmp(argv[2],"-subgraphWeight")==0)
        {
            sprintf(interp->result,"%f",G->Weight());
        }
        else
        {
            keyFound = false;
        }

        if (keyFound) return TCL_OK;

        // Do not complain, message 'info' is interpreted again
        return Goblin_Generic_Cmd(G,interp,argc,argv);
    }

    if (strcmp(argv[1],"explicitParallels")==0)
    {
        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        static_cast<sparseRepresentation*>(G->Representation()) -> ExplicitParallels();
        interp->result = "";
        return TCL_OK;
    }
 
    if (strcmp(argv[1],"random")==0)
    {
        graphRepresentation* GR = G->Representation();

        if (argc<3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"edges")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-numEdges",3);
            TArc numEdges = 1;

            if (pos>1 && pos<argc-1)
            {
                numEdges = atol(argv[pos+1]);
            }

            G -> RandomArcs(numEdges);
        }
        else if (strcmp(argv[2],"eulerian")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-numEdges",3);

            if (pos>1 && pos<argc-1)
            {
                G -> RandEulerian(TNode(atol(argv[pos+1])));
            }
        }
        else if (strcmp(argv[2],"regular")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-degree",3);

            if (pos>1 && pos<argc-1)
            {
                G -> RandRegular(TNode(atol(argv[pos+1])));
            }
        }

        int pos = CT->FindParam(argc,&argv[0],"-edgeLength",2);

        if (pos>1 && pos<argc)
        {
            for (TArc a=0;a<G->M();a++)
            {
                GR -> SetLength(2*a,TFloat(G->Context().SignedRand()));
            }
        }

        pos = CT->FindParam(argc,&argv[0],"-upperBound",2);

        if (pos>1 && pos<argc)
        {
            for (TArc a=0;a<G->M();a++)
            {
                GR -> SetUCap(2*a,G->LCap(2*a)+1+TCap(G->Context().UnsignedRand()));
            }
        }

        pos = CT->FindParam(argc,&argv[0],"-lowerBound",2);

        if (pos>1 && pos<argc)
        {
            for (TArc a=0;a<G->M();a++)
            {
                TCap oldCap = G->LCap(2*a);
                TCap newCap = TCap(TCap(G->Context().UnsignedRand()));

                if (newCap>oldCap)
                {
                    GR -> SetUCap(2*a,G->UCap(2*a)+newCap-oldCap);
                    GR -> SetLCap(2*a,newCap);
                }
                else
                {
                    GR -> SetLCap(2*a,newCap);
                    GR -> SetUCap(2*a,G->UCap(2*a)+newCap-oldCap);
                }
            }
        }

        pos = CT->FindParam(argc,&argv[0],"-geometry",2);

        if (pos>1 && pos<argc)
        {
            TDim dNew = 2;
            TDim dOld = GR->Dim();

            if (dNew>dOld) dOld = dNew;

            for (TDim i=0;i<dOld;++i)
            {
                if (i<dNew)
                {
                    for (TNode v=0;v<G->N();v++)
                    {
                        GR -> SetC(v,i,TFloat(G->Context().SignedRand()));
                    }
                }
                else
                {
                    GR -> ReleaseCoordinate(i);
                }
            }
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"write")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        G -> Write(argv[2]);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"shortestPath")==0)
    {
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            sourceNode = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-targetNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            targetNode = atol(argv[pos+1]);
        }

        try
        {
            if (CT->FindParam(argc,&argv[0],"-residual",2)>0)
            {
                G -> ShortestPath(abstractMixedGraph::SPX_DEFAULT,
                                  abstractMixedGraph::SPX_REDUCED,
                                  residualArcs(*G),
                                  sourceNode,targetNode);

                // Export the distance labels to the node potentials
                // When calling the shortest path solver again, the
                // distance labels refer to the reduced length labels
                // but the predecessors define a shortest path tree for
                // the original edge lengths
                G -> RawPotentials();
                G -> UpdatePotentials(InfFloat);
            }
            else
            {
                G -> ShortestPath(sourceNode,targetNode);
            }

            if (targetNode!=NoNode && G->Dist(targetNode)<InfFloat)
            {
                sprintf(interp->result,"%f",G->Dist(targetNode));
                return TCL_OK;
            }
            else
            {
                interp->result = "*";
                return TCL_OK;
            }
        }
        catch (...) {}

        interp->result = "Unable to assign distance labels";
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"connectivity")==0)
    {
        bool edgeConnnectivity = false;
        bool strongConnnectivity = false;
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;
        TCap retCap = InfCap;

        int pos = CT->FindParam(argc,&argv[0],"-edge",2);

        if (pos>0)
        {
            edgeConnnectivity = true;
        }

        pos = CT->FindParam(argc,&argv[0],"-strong",2);

        if (pos>0)
        {
            strongConnnectivity = true;
        }

        pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            sourceNode = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-targetNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            targetNode = atol(argv[pos+1]);
        }

        if (edgeConnnectivity)
        {
            if (strongConnnectivity)
            {
                retCap = G->StrongEdgeConnectivity(sourceNode,targetNode);
            }
            else
            {
                retCap = G->EdgeConnectivity(sourceNode,targetNode);
            }
        }
        else
        {
            pos = CT->FindParam(argc,&argv[0],"-vertex",2);
            abstractMixedGraph::TOptNodeSplitting mode =
                (pos>0) ? G->MCC_UNIT_CAPACITIES : G->MCC_MAP_DEMANDS;

            if (strongConnnectivity)
            {
                retCap = G->StrongNodeConnectivity(sourceNode,targetNode,mode);
            }
            else
            {
                retCap = G->NodeConnectivity(sourceNode,targetNode,mode);
            }
        }

        sprintf(interp->result,"%f",retCap);
        return TCL_OK;
    }

    if (strcmp(argv[1],"components")==0)
    {
        bool strongConnnectivity = false;
        TCap kappa = 1;

        int pos = CT->FindParam(argc,&argv[0],"-kappa",2);
        if (pos>0 && pos<argc-1)
        {
            kappa = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-strong",2);
        if (pos>0)
        {
            strongConnnectivity = true;
        }

        if (strongConnnectivity)
        {
            sprintf(interp->result,"%d",G->StronglyEdgeConnected(kappa));
        }
        else
        {
            sprintf(interp->result,"%d",G->EdgeConnected(kappa));
        }

        return TCL_OK;
    }

    if (strcmp(argv[1],"bipolarOrientation")==0)
    {
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;
        TArc rootArc = NoArc;
        bool feasible = false;

        int pos = CT->FindParam(argc,&argv[0],"-rootArc",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            rootArc = atol(argv[pos+1]);
        }
        else
        {
            pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);
            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                sourceNode = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-targetNode",2);
            if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
            {
                targetNode = atol(argv[pos+1]);
            }
        }

        if (CT->FindParam(argc,&argv[0],"-decompose",2)>0)
        {
            feasible = G->STNumbering(rootArc,sourceNode,targetNode);
        }
        else
        {
            feasible = G->ImplicitSTOrientation(sourceNode,targetNode);
        }

        if (feasible)
        {
            interp->result = "";
            return TCL_OK;
        }
        else
        {
            interp->result = "Graph is not 2-connected";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"nodeColouring")==0)
    {
        TNode chi = 0;

        int pos = CT->FindParam(argc,&argv[0],"-threshold",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            TNode threshold = atol(argv[pos+1]);

            if (threshold<=0)
            {
                sprintf(interp->result,"Invalid bound specification: %s",argv[pos+1]);
                return TCL_ERROR;
            }

            chi = G->NodeColouring(threshold);
        }
        else chi = G->NodeColouring();

        if (chi>0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(chi));
            return TCL_OK;
        }
        else
        {
            interp->result = "No such structure exists";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"edgeColouring")==0)
    {
        TNode chi = 0;

        int pos = CT->FindParam(argc,&argv[0],"-threshold",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            TNode threshold = atol(argv[pos+1]);

            if (threshold<=0)
            {
                sprintf(interp->result,"Invalid bound specification: %s",argv[pos+1]);
                return TCL_ERROR;
            }

            chi = G->EdgeColouring(threshold);
        }
        else chi = G->EdgeColouring();

        if (chi>0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(chi));
            return TCL_OK;
        }
        else
        {
            interp->result = "No such structure exists";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"cliqueCover")==0)
    {
        TNode chi = 0;

        int pos = CT->FindParam(argc,&argv[0],"-threshold",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            TNode threshold = atol(argv[pos+1]);

            if (threshold<=0)
            {
                sprintf(interp->result,"Invalid bound specification: %s",argv[pos+1]);
                return TCL_ERROR;
            }

            chi = G->CliqueCover(threshold);
        }
        else chi = G->CliqueCover();

        if (chi>0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(chi));
            return TCL_OK;
        }
        else
        {
            interp->result = "No such structure exists";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"stableSet")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->StableSet()));
        return TCL_OK;
    }

    if (strcmp(argv[1],"maximumClique")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->Clique()));
        return TCL_OK;
    }

    if (strcmp(argv[1],"vertexCover")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->VertexCover()));
        return TCL_OK;
    }

    if (strcmp(argv[1],"eulerianCycle")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%d",G->EulerianCycle());
        return TCL_OK;
    }

    if (strcmp(argv[1],"feedbackArcSet")==0)
    {
        sprintf(interp->result,"%f",G->FeedbackArcSet());
        return TCL_OK;
    }

    if (strcmp(argv[1],"hamiltonianCycle")==0)
    {
        TNode rootNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-rootNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            rootNode = atol(argv[pos+1]);
        }

        TFloat ret = G->TSP(rootNode);

        if (ret!=InfFloat)
        {
            sprintf(interp->result,"%f",ret);
            return TCL_OK;
        }
        else
        {
            interp->result = "Graph is non-Hamiltonian";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"spanningTree")==0)
    {
        abstractMixedGraph::TOptMST characteristic = G->MST_PLAIN;
        TNode rootNode = NoNode;

        if (CT->FindParam(argc,&argv[0],"-maximize",2))
        {
            characteristic = G->MST_MAX;
        }

        if (CT->FindParam(argc,&argv[0],"-cycle",2))
        {
            characteristic = abstractMixedGraph::TOptMST(
                                characteristic | G->MST_ONE_CYCLE);
        }

        int pos = CT->FindParam(argc,&argv[0],"-rootNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            rootNode = atol(argv[pos+1]);
        }

        TFloat ret = G->MinTree(G->MST_DEFAULT,characteristic,rootNode);

        if (ret!=InfFloat)
        {
            sprintf(interp->result,"%f",ret);
            return TCL_OK;
        }
        else
        {
            interp->result = "Graph is disconnected";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"steinerTree")==0)
    {
        TNode rootNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-rootNode",2);

        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            rootNode = atol(argv[pos+1]);
        }

        TFloat ret = G->SteinerTree(demandNodes(*G),rootNode);

        if (ret!=InfFloat)
        {
            sprintf(interp->result,"%f",ret);
            return TCL_OK;
        }
        else
        {
            interp->result = "Terminal nodes are disconnected";
            return TCL_ERROR;
        }
    }

    if (strcmp(argv[1],"networkFlow")==0)
    {
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            sourceNode = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-targetNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            targetNode = atol(argv[pos+1]);
        }


        if (CT->FindParam(argc,&argv[0],"-feasible",2))
        {
            if (G->AdmissibleBFlow())
            {
                interp->result = "";
                return TCL_OK;
            }
            else
            {
                interp->result = "No such structure exists";
                return TCL_ERROR;
            }
        }
        else if (CT->FindParam(argc,&argv[0],"-maximize",2))
        {
            TFloat flowValue = G->MaxFlow(sourceNode,targetNode);

            try
            {
                sprintf(interp->result,"%f",flowValue);
                return TCL_OK;
            }
            catch (ERCheck)
            {
                sprintf(interp->result,"Flow is corrupted");
                return TCL_ERROR;
            }
        }
        else if (sourceNode==NoNode && targetNode==NoNode)
        {
            try
            {
                TFloat ret = G->MinCostBFlow();
                sprintf(interp->result,"%f",ret);
                return TCL_OK;
            }
            catch (ERRejected)
            {
                interp->result = "No such structure exists";
                return TCL_ERROR;
            }
        }
        else
        {
            try
            {
                TFloat ret = G -> MinCostSTFlow(sourceNode,targetNode);
                sprintf(interp->result,"%f",ret);
                return TCL_OK;
            }
            catch (ERCheck)
            {
                interp->result = "Flow is corrupted";
                return TCL_ERROR;
            }
        }
    }

    if (strcmp(argv[1],"chinesePostman")==0)
    {
        bool adjustUCap = false;

        if (CT->FindParam(argc,&argv[0],"-adjust",2))
        {
            adjustUCap = true;
        }

        G -> ChinesePostman(adjustUCap);
        sprintf(interp->result,"%f",G->Weight());
        return TCL_OK;
    }

    if (strcmp(argv[1],"maximumEdgeCut")==0)
    {
        TNode sourceNode = NoNode;
        TNode targetNode = NoNode;

        int pos = CT->FindParam(argc,&argv[0],"-sourceNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            sourceNode = atol(argv[pos+1]);
        }

        pos = CT->FindParam(argc,&argv[0],"-targetNode",2);
        if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
        {
            targetNode = atol(argv[pos+1]);
        }

        sprintf(interp->result,"%f",G->MaxCut(sourceNode,targetNode));

        G -> InitSubgraph();

        for (TArc a=0;a<G->M();a++)
        {
            if (G->NodeColour(G->StartNode(2*a))!=G->NodeColour(G->EndNode(2*a)) &&
                (G->Orientation(2*a)==0 || G->NodeColour(G->StartNode(2*a))==0))
            {
                G -> SetSub(2*a,G->UCap(2*a));
            }
            else
            {
                G -> SetSub(2*a,G->LCap(2*a));
            }
        }

        return TCL_OK;
    }


    if (strcmp(argv[1],"max")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"-nodeDemand")==0)
        {
            if (G->MaxDemand()!=InfCap)
            {
                if (G->MaxDemand()!=(long int)(G->MaxDemand()))
                    sprintf(interp->result,"%f",G->MaxDemand());
                else  sprintf(interp->result,"%ld",(long int)G->MaxDemand());
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[2],"-lowerBound")==0)
        {
            if (G->MaxLCap()!=(long int)(G->MaxLCap()))
                sprintf(interp->result,"%f",G->MaxLCap());
            else  sprintf(interp->result,"%ld",(long int)G->MaxLCap());
        }
        else if (strcmp(argv[2],"-upperBound")==0)
        {
            if (G->MaxUCap()!=InfCap)
            {
                if (G->MaxUCap()!=(long int)(G->MaxUCap()))
                    sprintf(interp->result,"%f",G->MaxUCap());
                else  sprintf(interp->result,"%ld",(long int)G->MaxUCap());
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[2],"-edgeLength")==0)
        {
            if (G->MaxLength()!=InfFloat)
            {
                if (G->MaxLength()!=(long int)(G->MaxLength()))
                    sprintf(interp->result,"%f",G->MaxLength());
                else  sprintf(interp->result,"%ld",(long int)G->MaxLength());
            }
            else interp->result = "*";
        }
        else
        {
            sprintf(interp->result,"Unknown graph attribute: %s",argv[2]);
            return TCL_ERROR;
        }

        return TCL_OK;
    }


    if (strcmp(argv[1],"constant")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"-nodeDemand")==0)
        {
            sprintf(interp->result,"%d",G->CDemand());
        }
        else if (strcmp(argv[2],"-lowerBound")==0)
        {
            sprintf(interp->result,"%d",G->CLCap());
        }
        else if (strcmp(argv[2],"-upperBound")==0)
        {
            sprintf(interp->result,"%d",G->CUCap());
        }
        else if (strcmp(argv[2],"-edgeLength")==0)
        {
            sprintf(interp->result,"%d",G->CLength());
        }
        else
        {
            sprintf(interp->result,"Unknown graph attribute: %s",argv[2]);
            return TCL_ERROR;
        }

        return TCL_OK;
    }


    if (strcmp(argv[1],"configure")==0)
    {
        for (int i=2;i<argc-1;i+=2)
        {
            if (strcmp(argv[i],"-upperBound")==0)
            {
                TCap uu = (strcmp(argv[i+1],"*")!=0) ? TCap(atof(argv[i+1])) : InfCap;

                G->Representation() -> SetCUCap(uu);
            }
            else if (strcmp(argv[i],"-lowerBound")==0)
            {
                TCap ll = TCap(atof(argv[i+1]));

                G->Representation() -> SetCLCap(ll);
            }
            else if (strcmp(argv[i],"-edgeLength")==0)
            {
                TFloat ll = (strcmp(argv[i+1],"*")!=0) ? TFloat(atof(argv[i+1])) : InfFloat;

                G->Representation() -> SetCLength(ll);
            }
            else if (strcmp(argv[i],"-nodeDemand")==0)
            {
                TCap bb = TCap(atof(argv[i+1]));

                G->Representation() -> SetCDemand(bb);
            }
            else if (strcmp(argv[i],"-sourceNode")==0)
            {
                TNode v = (strcmp(argv[i+1],"*")!=0) ? TNode(atol(argv[i+1])) : NoNode;

                G -> SetSourceNode(v);
            }
            else if (strcmp(argv[i],"-targetNode")==0)
            {
                TNode v = (strcmp(argv[i+1],"*")!=0) ? TNode(atol(argv[i+1])) : NoNode;

                G -> SetTargetNode(v);
            }
            else if (strcmp(argv[i],"-rootNode")==0)
            {
                TNode v = (strcmp(argv[i+1],"*")!=0) ? TNode(atol(argv[i+1])) : NoNode;

                G -> SetRootNode(v);
            }
            else if (strcmp(argv[i],"-metricType")==0)
            {
                abstractMixedGraph::TMetricType metricType = abstractMixedGraph::METRIC_DISABLED;

                if (strlen(argv[i+1])==1)
                {
                    metricType = abstractMixedGraph::TMetricType(atol(argv[i+1]));
                }
                else if (strcmp(argv[i+1],"disabled")==0)
                {
                    metricType = abstractMixedGraph::METRIC_DISABLED;
                }
                else if (strcmp(argv[i+1],"manhattan")==0)
                {
                    metricType = abstractMixedGraph::METRIC_MANHATTAN;
                }
                else if (strcmp(argv[i+1],"euclidian")==0)
                {
                    metricType = abstractMixedGraph::METRIC_EUCLIDIAN;
                }
                else if (strcmp(argv[i+1],"maximum")==0)
                {
                    metricType = abstractMixedGraph::METRIC_MAXIMUM;
                }
                else if (strcmp(argv[i+1],"spheric")==0)
                {
                    metricType = abstractMixedGraph::METRIC_SPHERIC;
                }
                else
                {
                    sprintf(interp->result,"Unknown metric type: %s",argv[i+1]);
                    return TCL_ERROR;
                }

                G->Representation() -> SetMetricType(metricType);
            }
            else if (strcmp(argv[i],"-exteriorArc")==0)
            {
                TArc a = (strcmp(argv[i+1],"*")!=0) ? TArc(atol(argv[i+1])) : NoArc;
                G -> MarkExteriorFace(a);
                interp->result = "";
                return GOSH_OK;
            }
            else
            {
                sprintf(interp->result,"Unknown graph attribute: %s",argv[i]);
                return TCL_ERROR;
            }
        }

        interp->result = "";
        return TCL_OK;
    }


    if (strcmp(argv[1],"adjacency")==0)
    {
        if (argc!=4)
        {
            interp->result = "Missing end nodes";
            return TCL_ERROR;
        }

        TNode u = (TArc)atol(argv[2]);
        TNode v = (TArc)atol(argv[3]);
        TArc a = G->Adjacency(u,v);

        if (a==NoArc) interp->result = "*";
        else sprintf(interp->result,"%lu",static_cast<unsigned long>(a));

        return TCL_OK;
    }

    if (strcmp(argv[1],"node")==0)
        return Goblin_Node_Cmd(G,interp,argc,argv);

    if (strcmp(argv[1],"arc")==0)
        return Goblin_Arc_Cmd(G,interp,argc,argv);

    return Goblin_Generic_Cmd(G,interp,argc,argv);
}


int Goblin_Node_Cmd (abstractMixedGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<3)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    if (strcmp(argv[2],"insert")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->InsertNode()));
        return TCL_OK;
    }

    if (argc<4)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    TNode v = atol(argv[2]);

    if (strcmp(argv[3],"delete")==0)
    {
        G->DeleteNode(v);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"info")==0)
    {
        if (strcmp(argv[4],"-firstIncidence")==0)
        {
            if (G->First(v)!=NoArc)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->First(v)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-nodeDemand")==0)
        {
            sprintf(interp->result,"%g",static_cast<double>(G->Demand(v)));
        }
        else if (strcmp(argv[4],"-cx")==0)
        {
            sprintf(interp->result,"%f",static_cast<double>(G->C(v,0)));
        }
        else if (strcmp(argv[4],"-cy")==0)
        {
            sprintf(interp->result,"%f",static_cast<double>(G->C(v,1)));
        }
        else if (strcmp(argv[4],"-distance")==0)
        {
            if (G->Dist(v)!=InfFloat)
            {
                sprintf(interp->result,"%g",static_cast<double>(G->Dist(v)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-potential")==0)
        {
            sprintf(interp->result,"%g",static_cast<double>(G->Pi(v)));
        }
        else if (strcmp(argv[4],"-nodeColour")==0)
        {
            if (G->NodeColour(v)!=NoNode)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->NodeColour(v)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-predecessorArc")==0)
        {
            if (G->Pred(v)!=NoArc)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->Pred(v)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-degree")==0)
        {
            sprintf(interp->result,"%g",G->Deg(v));
        }
        else if (strcmp(argv[4],"-hidden")==0)
        {
            sprintf(interp->result,"%d",G->HiddenNode(v));
        }
        else
        {
            sprintf(interp->result,"Unknown node attribute: %s",argv[4]);
            return TCL_ERROR;
        }

        return TCL_OK;
    }

    if (strcmp(argv[3],"configure")==0)
    {
        for (int i=4;i<argc-1;i+=2)
        {
            if (strcmp(argv[i],"-firstIncidence")==0)
            {
                if (!G->IsSparse())
                {
                    interp->result = "Operation applies to sparse graphs only";
                    return TCL_ERROR;
                }

                TArc a2 = TArc(atol(argv[i+1]));
                static_cast<sparseRepresentation*>(G->Representation()) -> SetFirst(v,a2);
            }
            else if (strcmp(argv[i],"-nodeDemand")==0)
            {
                TCap bb = TCap(atof(argv[i+1]));

                G->Representation() -> SetDemand(v,bb);
            }
            else if (strcmp(argv[i],"-distance")==0)
            {
                TFloat dd = (strcmp(argv[i+1],"*")!=0) ? TFloat(atof(argv[i+1])) : InfFloat;

                G -> SetDist(v,dd);
            }
            else if (strcmp(argv[i],"-potential")==0)
            {
                TFloat pp = TFloat(atof(argv[i+1]));

                G -> SetPotential(v,pp);
            }
            else if (strcmp(argv[i],"-nodeColour")==0)
            {
                TNode cc = (strcmp(argv[i+1],"*")!=0) ? TNode(atoi(argv[i+1])) : NoNode;

                G -> SetNodeColour(v,cc);
            }
            else if (strcmp(argv[i],"-predecessorArc")==0)
            {
                TArc aa = (strcmp(argv[i+1],"*")!=0) ? TArc(atoi(argv[i+1])) : NoArc;

                G -> SetPred(v,aa);
            }
            else
            {
                sprintf(interp->result,"Unknown node attribute: %s",argv[i]);
                return TCL_ERROR;
            }
        }

        interp->result = "";
        return TCL_OK;
    }

    sprintf(interp->result,"Unknown option: %s node %s",argv[0],argv[2]);
    return TCL_ERROR;
}


int Goblin_Arc_Cmd (abstractMixedGraph *G,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<4)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    if (strcmp(argv[2],"insert")==0)
    {
        if (argc!=5)
        {
            interp->result = "Missing end nodes";
            return TCL_ERROR;
        }

        TNode u = TArc(atol(argv[3]));
        TNode v = TArc(atol(argv[4]));

        sprintf(interp->result,"%lu",static_cast<unsigned long>(G->InsertArc(u,v)));
        return TCL_OK;
    }

    TArc a = (TArc)atol(argv[2]);

    if (strcmp(argv[3],"delete")==0)
    {
        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        static_cast<sparseRepresentation*>(G->Representation()) -> DeleteArc(a);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"contract")==0)
    {
        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        static_cast<sparseRepresentation*>(G->Representation()) -> ContractArc(a);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"straightLine")==0)
    {
        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        static_cast<sparseRepresentation*>(G->Representation()) -> ReleaseEdgeControlPoints(a);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"reverse")==0)
    {
        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        static_cast<sparseRepresentation*>(G->Representation()) -> FlipArc(a);
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"info")==0)
    {
        if (strcmp(argv[4],"-righthandArc")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(G->Right(a,G->StartNode(a))));
        }
        else if (strcmp(argv[4],"-endNode")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(G->EndNode(a)));
        }
        else if (strcmp(argv[4],"-startNode")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(G->StartNode(a)));
        }
        else if (strcmp(argv[4],"-directed")==0)
        {
            sprintf(interp->result,"%d",G->Orientation(a));
        }
        else if (strcmp(argv[4],"-upperBound")==0)
        {
            if (G->UCap(a)!=InfCap)
            {
                sprintf(interp->result,"%g",static_cast<double>(G->UCap(a)));
            }
            else  sprintf(interp->result,"*");
        }
        else if (strcmp(argv[4],"-lowerBound")==0)
        {
            sprintf(interp->result,"%g",static_cast<double>(G->LCap(a)));
        }
        else if (strcmp(argv[4],"-edgeLength")==0)
        {
            if (G->Length(a)!=InfFloat)
            {
                sprintf(interp->result,"%g",static_cast<double>(G->Length(a)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-edgeColour")==0)
        {
            if (G->EdgeColour(a)!=NoArc)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->EdgeColour(a)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-subgraph")==0)
        {
            sprintf(interp->result,"%g",static_cast<double>(G->Sub(a)));
        }
        else if (strcmp(argv[4],"-labelAnchorPoint")==0)
        {
            if (G->ArcLabelAnchor(a)!=NoNode)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->ArcLabelAnchor(a)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-portNode")==0)
        {
            if (G->PortNode(a)!=NoNode)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(G->PortNode(a)));
            }
            else interp->result = "*";
        }
        else if (strcmp(argv[4],"-hidden")==0)
        {
            sprintf(interp->result,"%d",G->HiddenArc(a));
        }
        else
        {
            sprintf(interp->result,"Unknown node attribute: %s",argv[4]);
            return TCL_ERROR;
        }

        return TCL_OK;
    }

    if (strcmp(argv[3],"configure")==0)
    {
        for (int i=4;i<argc-1;i+=2)
        {
            if (strcmp(argv[i],"-upperBound")==0)
            {
                TCap uu = (strcmp(argv[i+1],"*")!=0) ? TCap(atof(argv[i+1])) : InfCap;

                G->Representation() -> SetUCap(a,uu);
            }
            else if (strcmp(argv[i],"-lowerBound")==0)
            {
                TCap ll = TCap(atof(argv[i+1]));

                G->Representation() -> SetLCap(a,ll);
            }
            else if (strcmp(argv[i],"-edgeLength")==0)
            {
                TFloat ll = (strcmp(argv[i+1],"*")!=0) ? TFloat(atof(argv[i+1])) : InfFloat;

                G->Representation() -> SetLength(a,ll);
            }
            else if (strcmp(argv[i],"-righthandArc")==0)
            {
                if (!G->IsSparse())
                {
                    interp->result = "Operation applies to sparse graphs only";
                    return TCL_ERROR;
                }

                TArc a2 = TArc(atol(argv[i+1]));
                static_cast<sparseRepresentation*>(G->Representation()) -> SetRight(a,a2);
            }
            else if (strcmp(argv[i],"-edgeColour")==0)
            {
                TArc cc = (strcmp(argv[i+1],"*")!=0) ? TArc(atoi(argv[i+1])) : NoArc;

                G -> SetEdgeColour(a,cc);
            }
            else if (strcmp(argv[i],"-directed")==0)
            {
                char oo = char(atol(argv[i+1]));
                G->Representation() -> SetOrientation(a,oo);
            }
            else if (strcmp(argv[i],"-subgraph")==0)
            {
                TFloat multiplicity = (TFloat)atof(argv[i+1]);
                G->SetSub(a,multiplicity);
            }
            else
            {
                sprintf(interp->result,"Unknown arc attribute: %s",argv[i]);
                return TCL_ERROR;
            }
        }

        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[3],"provide")==0)
    {
        if (argc!=5)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (!G->IsSparse())
        {
            interp->result = "Operation applies to sparse graphs only";
            return TCL_ERROR;
        }

        sparseRepresentation* GR = static_cast<sparseRepresentation*>(G->Representation());

        if (strcmp(argv[4],"-labelAnchorPoint")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(GR->ProvideArcLabelAnchor(a)));
            return TCL_OK;
        }

        if (strcmp(argv[4],"-portNode")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(GR->ProvidePortNode(a)));
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown layout point type: %s",argv[4]);
        return TCL_ERROR;
    }

    sprintf(interp->result,"Unknown option: %s arc %s",argv[0],argv[2]);
    return TCL_ERROR;
}
