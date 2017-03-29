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
    else CT->logIO = 0;

    char *type = "unknown";
    try
    {
        goblinImport F(fileIn,*CT);
        type = F.Scan();
    }
    catch (...) {};

    cout << " Reading graph from " << fileIn << "..." << endl;
    sparseGraph *GS = NULL;
    sparseDiGraph *GA = NULL;
    if (strcmp(type,"digraph")==0)
    {
        GA = new sparseDiGraph(fileIn,*CT);
        CT -> traceStep = GA->N();
    }
    else
    {
        if (strcmp(type,"graph")==0)
        {
            GS = new sparseGraph(fileIn,*CT);
            CT -> traceStep = GS->N();
        }
        else
        {
            cout << "...not a valid graph object: " << fileIn << endl << endl;
            exit(1);
        }
    }

    cout << " Writing output graph to " << fileOut << "..." << endl;

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

    cout << endl << "Computing chinese postman tour...";

    if (GS)
    {
        GS -> ChinesePostman(true);
        GS -> ReleaseLabels();
        GS -> ReleasePredecessors();

        int sh = CT->FindParam(ParamCount,ParamStr,"-sh");

        if (sh)
        {
            goblinExport F(fileOut);
            GS -> WriteSubgraph(F);
        }
        else GS -> Write(fileOut);

        delete GS;
    }
    else
    {
        GA -> ChinesePostman(true);
        GA -> ReleaseLabels();
        GA -> ReleasePredecessors();
        GA -> Write(fileOut);
        delete GA;
    }

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

    return 0;
}
