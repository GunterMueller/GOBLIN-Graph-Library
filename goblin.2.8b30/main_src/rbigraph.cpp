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

    int pc = CT.FindParam(ParamCount,ParamStr,"-n");
    TNode n1 = atol(ParamStr[pc+1]);
    TNode n2 = atol(ParamStr[pc+2]);
    TNode n = n1+n2;
    pc = CT.FindParam(ParamCount,ParamStr,"-m");
    TArc m = atol(ParamStr[pc+1]);

    char Regular = 0;
    TNode k = 0;
    pc = CT.FindParam(ParamCount,ParamStr,"-regular");
    if (pc>0)
    {
        Regular = 1;
        n1 = n2 = atoi(ParamStr[pc+1]);
        n = 2*n1;
        k = atoi(ParamStr[pc+2]);
        m=n1*k;
    }

    pc = CT.FindParam(ParamCount,ParamStr,"-dense");
    if (pc>0)
    {
        denseBiGraph G(n1,n2,managedObject::OPT_COMPLETE,goblinDefaultContext);

        G.Write(fileOut);
    }
    else
    {
        sparseBiGraph G(n1,n2,goblinDefaultContext);
        static_cast<sparseRepresentation*>(G.Representation()) -> SetCapacity(n,m);

        if (Regular) G.RandRegular(k);
        else G.RandomArcs(m);

        G.Write(fileOut);
    }

    cout << endl << endl;

    delete[] fileOut;

    return 0;
}
