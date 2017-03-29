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
    catch (...) {}

    abstractDiGraph* H = NULL;
    if (strcmp(type,"dense_digraph")==0) H = new denseDiGraph(fileIn,*CT);
    else
        if (strcmp(type,"digraph")==0) H = new sparseDiGraph(fileIn,*CT);
        else
        {
            cout << "...not a valid graph object: " << fileIn << endl << endl;
            exit(1);
        }

    cout << "...Flow network read from " << fileIn << endl; 

    CT -> traceStep = H->N();

    cout << endl;
    TNode vs = NoNode;
    TNode vt = NoNode;
    int div = CT->FindParam(ParamCount,ParamStr,"-div");
    if (div)
    {
        vs = atoi(ParamStr[div+1]);
        cout << "Source node s := " << vs << endl;
        vt = atoi(ParamStr[div+2]);
        cout << "Target node t := " << vt << endl;

        if (vs==vt || vs>=H->N() || vt>=H->N())
        {
            cout << "...Bad choice of source or target node!" << endl << endl;
            return 1;
        }
    }
    else
    {
        cout << "No source node given." << endl;
    }

    CT -> methMCFST = 0;
    CT -> methMCF = 1;
    CT -> methMXF = 1;
    CT -> methPQ = 1;

    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;

    cout << endl << "Method selection:" << endl;
    cout << " Min-Cost st-Flow method (methMCFST): " << (int)CT->methMCFST << endl;
    cout << " Min-Cost b-Flow method (methMCF): " << (int)CT->methMCF << endl;
    cout << " Max-Flow method (methMXF): " << (int)CT->methMXF << endl;
    cout << " Heap method (methPQ): " << (int)CT->methPQ << endl;

    int ret = 0;
    int weighted = CT->FindParam(ParamCount,ParamStr,"-w");
    cout << endl;
    if (div)
    {
        if (weighted)
        {
            cout << "Computing maximum st-flow of minimum costs...";
            H -> MinCostSTFlow(vs,vt);
        }
        else
        {
            cout << "Computing maximum st-flow...";
            H -> MaxFlow(vs,vt);
        }
    }
    else
    {
        if (weighted)
        {
            cout << "Computing b-flow with minimum costs...";

            try
            {
                H -> MinCostBFlow();
                cout << endl << "...optimal b-flow found."; 
            }
            catch (ERRejected)
            {
                cout << endl << "...no feasible b-flow exists.";
                ret = 1;
            }

            H -> ReleaseLabels();
            H -> ReleasePredecessors();
        }
        else
        {
            cout << "Computing feasible b-flow...";

            if (H -> AdmissibleBFlow())
            {
                cout << endl << "...b-flow found."; 

                H -> ReleaseLabels();
                H -> ReleasePredecessors();
            }
            else
            {
                cout << endl << "...no feasible b-flow.";
                ret = 1;
            }
        }
    }

    if (ret==0)
    {
        cout << endl << endl << "Writing flow labels to " << fileOut << "..."; 
        int sh = CT->FindParam(ParamCount,ParamStr,"-sh");
        if (sh)
        {
            goblinExport F(fileOut);
            H -> WriteSubgraph(F);
        }
        else H -> Write(fileOut);
    }

    delete H;

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
