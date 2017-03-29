
// --------------------------------------------------------------------------
//  Sample code for generating DAGs, computing a shortest path tree,
//  drawing this tree and exporting the final object to a file
// --------------------------------------------------------------------------


#include <sparseDigraph.h>


int main(int ParamCount,const char* ParamStr[])
{
    // Controller object has some methods which are used independently from
    // graph objects. 
    goblinController &CT = goblinDefaultContext;

    // Specify instance dimensions at command prompt
    int pc = CT.FindParam(ParamCount,ParamStr,"-n");
    TNode n = (pc>0) ? atol(ParamStr[pc+1]) : 1000;

    pc = CT.FindParam(ParamCount,ParamStr,"-m");
    TArc m = (pc>0) ? atol(ParamStr[pc+1]) : 5000;

    pc = CT.FindParam(ParamCount,ParamStr,"-r");
    unsigned r = (pc>0) ? atol(ParamStr[pc+1]) : 1;

    pc = CT.FindParam(ParamCount,ParamStr,"-l");
    bool layout = (pc>0) ? true : false;


    // Interesting code starts here

    for (unsigned i = 0; i<r; i++)
    {
        // Instanciate a directed graph object
        sparseDiGraph G(n,CT);

        // Specify final dimensions to prevent from unnecessary reallocations
        static_cast<sparseRepresentation*>(G.Representation()) -> SetCapacity(n,m);

        // Relevant flags for the random generator which is called whenever
        // arcs are inserted into the graph object
        CT.randLength = 1;
        CT.randUCap = 0;
        CT.randLCap = 0;

        for (TNode v=1;v<n;v++)
        {
            // Make sure that all nodes are reachable from root (index = 0)
            G.InsertArc(v-1,v);
        }

        while (G.M()<m)
        {
            // Add only nodes from lower to higher indices to obtain a DAG.
            // This simple procedure may cause parallel arcs
            TNode u = TNode(CT.Rand(n-1));
            TNode v = u+1 + TNode(CT.Rand(n-u-1));
            G.InsertArc(u,v);
        }

        // Choose a random target node other than the root (index = 0)
        TNode t = 1 + TNode(CT.Rand(n-1));

        // Knowing that the graph is acyclic, call DAGSearch() or,
        // equivalently, ShortestPath(0,t) with CT.methSPX = 4.
        // For other values of methSPX, more general methods are applied
        // which also work for DAGs but with some decay of efficiency.
        TNode x = G.DAGSearch(G.DAG_SPTREE,nonBlockingArcs(G),0,t);

        if (x!=NoNode)
        {
            // This cannot apply by the construction of G
            cout << "Graph is recurrent\n";
            return -1;
        }

        // Layout the shortest path tree for displaying with GOBLET.
        // The result is an "upward" drawing of the shortest path tree
        // rather than the entire graph. That is, non-tree arcs can point back.
        if (layout) G.Layout_PredecessorTree(G.ALIGN_CENTER);

        // Write graph to file for further processing with GOBLET
        // (or just to flood your hard disk)
        const char* fileName = ParamStr[ParamCount-1];
        unsigned l = strlen(fileName);
        char* fileOut = new char[l+5];
        strcat(strcpy(fileOut,fileName),".gob");

        G.Write(fileOut);

        delete[] fileOut;
    }

    return 0;
}
