#include <goblin.h>


static goblinController *CT;


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
    char* bFile = new char[l+5];
    strcat(strcpy(bFile,fileName),".bdg");
    char* aFile = new char[l+5];
    strcat(strcpy(aFile,fileName),".adg");

    cout << endl;
    int silent = CT->FindParam(ParamCount,ParamStr,"-silent");
    if (!silent)
    {
        CT->logStream       = new ofstream(logFile);
        CT->logEventHandler = &myLogEventHandler;
        cout << "Writing transscript to " << logFile << "..." << endl; 
    }

    char* type = "unknown";
    try
    {
        goblinImport F(fileIn,*CT);
        type = F.Scan();
    }
    catch (...) {};

    abstractGraph* G = NULL;

    if (strcmp(type,"dense_bigraph")==0)
    {
        G = new denseBiGraph(fileIn,*CT);
    }
    else if (strcmp(type,"bigraph")==0)
    {
        G = new sparseBiGraph(fileIn,*CT);
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

    cout << "...Graph read from " << fileIn << endl;

    CT -> traceStep = G->N();

    TCap *pDeg = NULL;
    TCap *pLower = NULL;
    int a = CT->FindParam(ParamCount,ParamStr,"-deg");
    if (a)
    {
        goblinImport* F = new goblinImport(bFile);
        F -> Scan("demand");
        pDeg = F->GetTCapTuple(G->N());
        cout << "...Upper degrees read from " << bFile << endl;

        F = new goblinImport(aFile);
        F -> Scan("demand");
        pLower = F->GetTCapTuple(G->N());
        cout << "...Lower degrees read from " << aFile << endl;
    }

    CT -> methMinCBalFlow = 1;
    CT -> methPrimalDual = 1;
    CT -> methMCFST = 0;
    CT -> methMCF = 1;
    CT -> methMaxBalFlow = 2;
    CT -> methBNS = 0;
    CT -> methMXF = 1;
    CT -> methDSU = 1;
    CT -> methPQ = 1;

    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;

    cout << endl << "Method selection:" << endl;
    cout << " Min-Cost Balanced Flow method (methMinCBalFlow): " << (int)CT->methMinCBalFlow << endl;
    cout << " Primal-Dual method (methPrimalDual): " << (int)CT->methPrimalDual << endl;
    cout << " Min-Cost st-Flow method (methMCFST): " << (int)CT->methMCFST << endl;
    cout << " Min-Cost Circulation method (methMCF): " << (int)CT->methMCF << endl;
    cout << " Maximum balanced flow method (methMaxBalFlow): " << (int)CT->methMaxBalFlow << endl;
    cout << " Balanced network search method (methBNS): " << (int)CT->methBNS << endl;
    cout << " Max-Flow method (methMXF): " << (int)CT->methMXF << endl;
    cout << " Union-Find method (methDSU): " << (int)CT->methDSU << endl;
    cout << " Heap method (methPQ): " << (int)CT->methPQ << endl;

    int ret = 0;
    int weighted = CT->FindParam(ParamCount,ParamStr,"-w");
    if (weighted)
    {
        cout << endl << "Computing perfect matching with minimum costs...";

        if ((pLower!=NULL && G->MinCMatching(pLower,pDeg)) ||
            (pLower==NULL && G->MinCMatching())              )
        {
            cout << endl << "...perfect matching found" << endl
                << endl << "Weight of the solution: " << G->Weight();
        }
        else
        {
            ret = 1;
            cout << endl << "...no perfect matching exists";
        }
    }
    else 
    {
        cout << endl << "Computing matching with minimum deficiency...";

        if ((pLower!=NULL && G->MaximumMatching(pLower,pDeg)) ||
            (pLower==NULL && G->MaximumMatching())              )
        {
            cout << endl << "...perfect matching found";
        }
        else
        {
            cout << endl << "...no perfect matching exists";
        }
    }

    if (ret==0)
    {
        cout << endl << endl << "Writing subgraph to " << fileOut << "...";
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
    delete[] aFile;
    delete[] bFile;

    return ret;
}
