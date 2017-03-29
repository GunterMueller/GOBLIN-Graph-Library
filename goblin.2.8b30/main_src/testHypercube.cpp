
// --------------------------------------------------------------------------
//  Example code for generating a hypercube graph by using a recurrency rule,
//  drawing the hypercube and exporting the final object to a file
// --------------------------------------------------------------------------


#include <sparseDigraph.h>


int main(int ParamCount,const char* ParamStr[])
{
    // Obtain the output filename from the final command line parameter
    const char* fileName = ParamStr[ParamCount-1];
    unsigned l = strlen(fileName);
    char* fileOut = new char[l+5];
    strcat(strcpy(fileOut,fileName),".gob");

    // Read an occasional "-dim xxx" command line parameter
    int pc = goblinDefaultContext.FindParam(ParamCount,ParamStr,"-dim");
    unsigned long dim = (pc>0 && pc<ParamCount-1) ? atol(ParamStr[pc+1]) : 5;

    // Start with a singleton node digraph
    sparseDiGraph* GBase = new sparseDiGraph(TNode(1));

    for (unsigned long i=0;i<dim;++i)
    {
        // Induction step: Start with a plain copy of GBase
        sparseDiGraph* GNext = new sparseDiGraph(*GBase,GBase->OPT_CLONE);

        // Add another disjoint copy of GBase
        GNext -> AddGraphByNodes(*GBase);

        // Connect both copies of GBase with arcs,
        // connecting both nodes with the same preimage in GBase,
        // and directed from the first to the second copy
        TNode n0 = GBase->N();
        for (TNode j=0;j<n0;++j)
        {
            GNext -> InsertArc(j,n0+j,1,0,0);
        }

        delete GBase;
        GBase = GNext;
    }

    if (dim>0)
    {
        // GBase has unique source and a unique target node
        TFloat val = GBase->MaxFlow(0,GBase->N()-1);

        if (val!=dim)
        {
            // This cannot apply by the construction of GBase
            cout << "Unexpected flow value:" << val << "\n";
            return -1;
        }
    }

    // Write the final graph to a file
    GBase->Write(fileOut);

    delete[] fileOut;

    return 0;
}
