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

    cout << endl << "File access:" << endl;
    int pc = CT->FindParam(ParamCount,ParamStr,"-silent");
    if (pc==0)
    {
        CT->logStream       = new ofstream(logFile);
        CT->logEventHandler = &myLogEventHandler;
        cout << " Writing transscript to " << logFile << "..." << endl; 
    }

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
    else
    {
        cout << "...not a valid graph object: " << fileIn << endl << endl;
        exit(1);
    }

    CT -> traceStep = 1;

    int find1Tree = CT->FindParam(ParamCount,ParamStr,"-1");

    cout << " Graph read from " << fileIn << endl;

    cout << " Writing solution to "
        << fileOut << "..." << endl << endl;

    TNode root = NoNode;
    pc = CT->FindParam(ParamCount,ParamStr,"-r");
    if (pc>0)
    {
        root = atoi(ParamStr[pc+1]);
        cout << "Root node: " << root << endl;
    }

    CT -> methPQ = 1;

    G -> SetLayoutParameter(TokLayoutArcLabelFormat,"#5");

    CT -> Configure(ParamCount,ParamStr);
    if (CT->traceLevel>1) CT->traceLevel = 1;

    cout << "Method selection:" << endl;
    cout << " Min-Tree method (methMST): " << (int)CT->methMST << endl;
    cout << " Heap method (methPQ): " << (int)CT->methPQ << endl;

    if (find1Tree) cout << endl  << "Computing minimum 1-Tree...";
    else cout << endl << "Computing minimum spanning tree...";

    int connected = 1;
    try
    {
        if (find1Tree)
        {
            G -> MinTree(G->MST_DEFAULT,G->MST_ONE_CYCLE,root);
        }
        else
        {
            G -> MinTree(root);
        }
    }
    catch (ERRejected) {connected = 0;};

    cout << endl;

    if (connected)
    {
        G -> ReleaseLabels();

        int sh = CT->FindParam(ParamCount,ParamStr,"-sh");
        if (sh)
        {
            goblinExport F(fileOut);
            G -> WriteRegister(F,TokRegPredecessor);
        }
        else G -> Write(fileOut);

        cout << "...Minimum spanning tree found." << endl;
    }
    else cout << "...Graph is disconnected." << endl;

    delete G;

    cout << endl << "Allocated " << goblinMaxSize << " bytes." << endl << endl;

    if (CT->logStream != &clog)
    {
        delete CT->logStream;
        CT->logStream = &clog;
    }

    delete CT;
    delete[] fileIn;
    delete[] fileOut;
    delete[] logFile;

    return 1-connected;
}
