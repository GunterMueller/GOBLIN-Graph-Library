
// --------------------------------------------------------------------------
//  Example code for generating a binary tree by using a recurrency rule,
//  drawing the tree and exporting the final object to a file
// --------------------------------------------------------------------------


#include <sparseDigraph.h>


class binaryTree : public sparseDiGraph
{
public:

    binaryTree(TNode depth,goblinController& _CT = goblinDefaultContext) throw();
};


binaryTree::binaryTree(TNode depth,goblinController &_CT) throw() :
    managedObject(_CT),
    sparseDiGraph(TNode(1),_CT)
{
    // Digraph is a singleton node yet. This node keeps
    // its index (=0) in what follows. It will form the
    // root node of the tree composed from two smaller
    // binaray trees

    // Recurrency base: No further nodes/arcs are needed
    if (depth==0) return;

    // Apply the recurrency rule
    binaryTree TR(depth-1,_CT);

    // Not a must-be: Make known the final number of nodes
    // and arcs to prevent from iterated reallocations
    X.SetCapacity(2*TR.N()+1,2*TR.N()+2);

    // Add two disjoint copies of TR to the graph under construction
    // All node and arc indices are shifted by a constant amount
    AddGraphByNodes(TR);
    AddGraphByNodes(TR);

    // Link the overall root node to the root nodes of the copies of TR
    InsertArc(0,1);
    InsertArc(0,TR.N()+1);
}


int main(int ParamCount,const char* ParamStr[])
{
    // Obtain the output filename from the final command line parameter
    const char* fileName = ParamStr[ParamCount-1];
    unsigned l = strlen(fileName);
    char* fileOut = new char[l+5];
    strcat(strcpy(fileOut,fileName),".gob");

    // Read an occasional "-depth xxx" command line parameter
    int pc = goblinDefaultContext.FindParam(ParamCount,ParamStr,"-depth");
    TNode depth = (pc>0 && pc<ParamCount-1) ? atol(ParamStr[pc+1]) : 5;

    binaryTree T(depth);

    // Draw T by horizontal and vertical lines. The root node (having index = 0)
    // is placed in the upper left corner, and for every non-leave node, its
    // child nodes are below and right of the parent node
    T.Layout_HorizontalVerticalTree(
        0,   // The root node index
        10); // The grid size

    // Write T to a file
    T.Write(fileOut);

    delete[] fileOut;

    return 0;
}
