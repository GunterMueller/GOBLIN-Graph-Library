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

    cout << endl; 
    int silent = CT->FindParam(ParamCount,ParamStr,"-silent");
    if (!silent)
    {
        CT->logStream       = new ofstream(logFile);
        CT->logEventHandler = &myLogEventHandler;
        cout << "Writing transscript to " << logFile << "..." << endl; 
    }

    char *type = "unknown";
    try
    {
        goblinImport F(fileIn,*CT);
        type = F.Scan();
    }
    catch (...) {}

    abstractBalancedFNW *G = NULL;
    TNode vs = NoNode;

    if (strcmp(type,"digraph")==0)
    {
        sparseDiGraph *H = new sparseDiGraph(fileIn,*CT);

        G = new splitGraph(*H);
        vs = G->N()-1;

        G -> SetLabel(H->Label());
        delete H;
        CT -> SetMaster(G->Handle());
    }
    else if (strcmp(type,"balanced_fnw")==0)
    {
        G = new balancedFNW(fileIn,*CT);

        int s = CT->FindParam(ParamCount,ParamStr,"-s");
        if (s) vs = atoi(ParamStr[s+1]);

        if (vs>=G->N())
        {
            cout << "...Bad choice of source node!" << endl << endl;
            return 1;
        }

        cout << "Source node s := " << vs << endl;
    }
    else
    {
        cout << "...not a valid graph object: " << fileIn << endl << endl;
        exit(1);
    }

    cout << "...flow network read from " << fileIn << endl; 

    CT -> traceStep = G->N();

    CT -> methMinCBalFlow = 1;
    CT -> methPrimalDual = 1;
    CT -> methMCFST = 0;
    CT -> methMXF = 1;
    CT -> methMCF = 1;
    CT -> methDSU = 1;
    CT -> methPQ = 1;

    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;

    cout << endl << "Method selection:" << endl;
    cout << " Min-Cost Balanced Flow method (methMinCBalFlow): " << (int)CT->methMinCBalFlow << endl;
    cout << " Primal-Dual method (methPrimalDual): " << (int)CT->methPrimalDual << endl;
    cout << " Min-Cost st-Flow method (methMCFST): " << (int)CT->methMCFST << endl;
    cout << " Min-Cost Circulation method (methMCF): " << (int)CT->methMCF << endl;
    cout << " Max-Flow method (methMXF): " << (int)CT->methMXF << endl;
    cout << " Union-Find method (methDSU): " << (int)CT->methDSU << endl;
    cout << " Heap method (methPQ): " << (int)CT->methPQ << endl;

    int ret = 0;
    int weighted = CT->FindParam(ParamCount,ParamStr,"-w");
    cout << endl;
    if (weighted)
    {
        cout << "Computing maximum balanced st-flow of minimum costs...";
        G -> MinCBalFlow(vs);
    }
    else
    {
        cout << "Computing maximum balanced st-flow...";
        G -> MaxBalFlow(vs);
    }

    if (ret==0)
    {
        cout << endl << endl << "Writing flow labels to " << fileOut << "..."; 
        int sh = CT->FindParam(ParamCount,ParamStr,"-sh");
        if (sh)
        {
            goblinExport F(fileOut);
            G -> WriteSubgraph(F);
        }
        else G -> Write(fileOut);
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
