#include <goblin.h>


int main(int ParamCount,const char* ParamStr[])
{
    goblinController &CT = goblinDefaultContext;
    CT.logWarn = 0;
    CT.logRes = 0;

    CT.randGeometry = 0;
    CT.randLength = 0;
    CT.randUCap = 0;
    CT.randLCap = 0;
    CT.randParallels = 0;

    CT.Configure(ParamCount,ParamStr);

    const char* fileName = ParamStr[ParamCount-1];
    unsigned l = strlen(fileName);
    char* fileOut = new char[l+5];
    strcat(strcpy(fileOut,fileName),".gob");

    int pc;
    pc = CT.FindParam(ParamCount,ParamStr,"-n");
    TNode n = atol(ParamStr[pc+1]);
    pc = CT.FindParam(ParamCount,ParamStr,"-m");
    TArc m = atol(ParamStr[pc+1]);

    char Eulerian = 0;
    pc = CT.FindParam(ParamCount,ParamStr,"-euler");
    if (pc>0) Eulerian = 1;

    pc = CT.FindParam(ParamCount,ParamStr,"-dense");
    if (pc>0)
    {
        denseDiGraph G(n,managedObject::OPT_COMPLETE,goblinDefaultContext);

        G.Write(fileOut);
    }
    else
    {
        sparseDiGraph G(n,goblinDefaultContext);
        static_cast<sparseRepresentation*>(G.Representation()) -> SetCapacity(n,m);

        if (Eulerian) G.RandEulerian(m);
        else G.RandomArcs(m);

        G.Write(fileOut);
    }

    cout << endl << endl;

    delete[] fileOut;

    return 0;
}
