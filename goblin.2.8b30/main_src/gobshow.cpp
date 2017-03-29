#include <goblin.h>


static goblinController *CT;


void myLogEventHandler(msgType msg,TModule mod,THandle OH,char *logText)
{
    CT -> PlainLogEventHandler(msg,mod,OH,logText);
}


int main(int ParamCount,const char *ParamStr[])
{
    CT = new goblinController();
    CT -> logEventHandler = &myLogEventHandler;
    goblinController* CT = new goblinController();
    CT -> traceLevel = 1;

    const char* fileIn = ParamStr[ParamCount-1];

    cout << endl;

    char* type = "unknown";
    try
    {
        goblinImport F(fileIn,*CT);
        type = F.Scan();
    }
    catch (...) {};

    abstractMixedGraph* G = NULL;

    if (strcmp(type,"dense_digraph")==0)
    {
        G = new denseDiGraph(fileIn,*CT);
    }
    else if (strcmp(type,"digraph")==0)
    {
        G = new sparseDiGraph(fileIn,*CT);
    }
    else if (strcmp(type,"dense_graph")==0)
    {
        G = new denseGraph(fileIn,*CT);
    }
    else if (strcmp(type,"graph")==0)
    {
        G = new sparseGraph(fileIn,*CT);
    }
    else if (strcmp(type,"dense_bigraph")==0)
    {
        G = new denseBiGraph(fileIn,*CT);
    }
    else if (strcmp(type,"bigraph")==0)
    {
        G = new sparseBiGraph(fileIn,*CT);
    }
    else if (strcmp(type,"balanced_fnw")==0)
    {
        G = new balancedFNW(fileIn,*CT);
    }
    else if (strcmp(type,"mixed_graph")==0)
    {
        G = new mixedGraph(fileIn,*CT);
    }
    else
    {
        cout << "...not a valid graph object: " << fileIn << endl << endl;
        exit(1);
    }

    cout << "...Graph read from " << fileIn << endl;

    CT -> displayMode = 2;
    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;
    G -> Display();

    delete G;

    cout << endl << "Allocated " << goblinMaxSize << " bytes." << endl << endl;

    delete CT;

    return 0;
}
