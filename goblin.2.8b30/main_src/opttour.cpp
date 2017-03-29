#include <goblin.h>


static goblinController* CT;


void myLogEventHandler(msgType msg,TModule mod,THandle OH,char *logText)
{
    CT -> PlainLogEventHandler(msg,mod,OH,logText);
}


int main(int ParamCount,const char *ParamStr[])
{
    CT = new goblinController();
    CT -> traceLevel = 1;

    const char* fileName = ParamStr[ParamCount-1];
    unsigned l = strlen(fileName);
    char* fileIn = new char[l+5];
    strcat(strcpy(fileIn,fileName),".gob");
    char* fileOut = new char[l+5];
    strcat(strcpy(fileOut,fileName),".rst");
    char* logFile = new char[l+5];
    strcat(strcpy(logFile,fileName),".log");

    cout << endl << "File access:" << endl;
    int pc = CT->FindParam(ParamCount,ParamStr,"-silent");
    if (pc==0)
    {
        CT->logStream       = new ofstream(logFile);
        CT->logEventHandler = &myLogEventHandler;
        cout << " Writing transscript to " << logFile << "..." << endl; 
    }

    char *type = "unknown";
    try
    {
        goblinImport F(fileIn,*CT);
        type = F.Scan();
    }
    catch (...) {};

    abstractMixedGraph *G = NULL;

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
    else
    {
        cout << "...not a valid graph object: " << fileIn << endl << endl;
        exit(1);
    }

    CT -> traceStep = G->N();
    cout << " ...Graph read from " << fileIn << endl;

    cout << " Writing predecessor labels to " << fileOut << "..." << endl;

    TNode root = NoNode;
    pc = CT->FindParam(ParamCount,ParamStr,"-r");
    if (pc>0)
    {
        root = atoi(ParamStr[pc+1]);
        cout << endl << " Root node: " << root << endl;
    }

    CT -> methMinCBalFlow = 1;
    CT -> methPrimalDual = 1;
    CT -> methMCFST = 0;
    CT -> methMXF = 1;
    CT -> methMCF = 1;
    CT -> methDSU = 1;
    CT -> methPQ = 1;
    CT -> methMST = 1;

    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;

    cout << endl << "Method selection:" << endl;
    cout << " TSP heuristic (methHeurTSP): " << (int)CT->methHeurTSP << endl;
    cout << " Min-Cost Balanced Flow method (methMinCBalFlow): " << (int)CT->methMinCBalFlow << endl;
    cout << " Primal-Dual method (methPrimalDual): " << (int)CT->methPrimalDual << endl;
    cout << " Min-Cost st-Flow method (methMCFST): " << (int)CT->methMCFST << endl;
    cout << " Min-Cost Circulation method (methMCF): " << (int)CT->methMCF << endl;
    cout << " Max-Flow method (methMXF): " << (int)CT->methMXF << endl;
    cout << " Union-Find method (methDSU): " << (int)CT->methDSU << endl;
    cout << " Heap method (methPQ): " << (int)CT->methPQ << endl;
    cout << " Min-Tree method (methMST): " << (int)CT->methMST << endl;

    cout << endl << "Computing random tour...";
    char ret = 1;
    try
    {
        TFloat opt = G->TSP(root);
        cout << endl << "...Tour of length " << opt << " found.";
        G -> ReleaseLabels();
        int sh = CT->FindParam(ParamCount,ParamStr,"-sh");
        if (sh)
        {
            goblinExport F(fileOut);
            G -> WriteRegister(F,TokRegPredecessor);
        }
        else G -> Write(fileOut);
        ret = 0;
    }
    catch (ERRejected)
    {
        cout << endl << "...No hamiltonian cycle found.";
    }

    delete G;

    cout << endl << endl << "Allocated " << goblinMaxSize << " bytes." << endl << endl;

    if (CT->logStream != &clog)
    {
        delete CT->logStream;
        CT->logStream = &clog;
    }

    delete CT;
    delete[] fileIn;
    delete[] fileOut;
    delete[] logFile;

    return ret;
}
