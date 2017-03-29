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
    char* gobIn = new char[l+5];
    strcat(strcpy(gobIn,fileName),".gob");
    char* fileIn = new char[l+5];
    strcat(strcpy(fileIn,fileName),".mps");
    char* basFile = new char[l+5];
    strcat(strcpy(basFile,fileName),".bas");
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

    mipFactory* theMipFactory = (mipFactory*)(CT->pMipFactory);
    mipInstance* XLP  = NULL;

    int ret = 0;
    try
    {
        int gob = CT->FindParam(ParamCount,ParamStr,"-gi");
        if (gob)
        {
            XLP = theMipFactory->ReadInstance(gobIn,*CT);
            cout << "...Problem read from " << gobIn << endl;
        }
        else
        {
            XLP = theMipFactory->NewInstance(1,1,1,mipInstance::MINIMIZE,*CT);
            XLP -> ReadMPSFile(fileIn);
            cout << "...Problem read from " << fileIn << endl;

            int bas = CT->FindParam(ParamCount,ParamStr,"-b");
            if (bas)
            {
                try
                {
                    XLP -> ReadBASFile(basFile);
                    cout << "...Basis read from " << basFile << endl;
                }
                catch (...)
                {
                    cout << "...BAS file import failed" << endl;
                }
            }
        }

        CT -> traceStep = 1;

        CT -> Configure(ParamCount,ParamStr);
        if (CT->traceLevel>1) CT->traceLevel = 1;

        cout << endl << "Number of rows: " << XLP->K() << endl;
        cout << "Number of columns: " << XLP->L() << endl;

        cout << endl << "Method selection:" << endl;
        cout << " LP Solver (methLP): " << CT->methLP << endl;
        cout << " Pricing rule (methLPPricing): " << CT->methLPPricing << endl;
        cout << " Quotient test (methLPQTest): " << CT->methLPQTest << endl;
        cout << " Initial basis (methLPStart): " << CT->methLPStart << endl;

        int feasible = CT->FindParam(ParamCount,ParamStr,"-f");
        if (feasible)
        {
            cout << endl << "Checking for feasibility...";

            if (XLP -> StartDual())
                cout << endl << "...Problem is feasible";
            else
            {
                ret = 1;
                cout << endl << "...Problem is infeasible";
            }
        }
        else 
        {
            cout << endl << "Solving...";

            if (XLP -> SolveLP())
                cout << endl << "...Optimal solution found" << endl
                    << endl << "Objective: " << XLP->ObjVal();
            else
            {
                ret = 1;
                cout << endl << "...Problem is infeasible";
            }
        }

        if (ret==0)
        {
            int gob = CT->FindParam(ParamCount,ParamStr,"-go");
            if (gob)
            {
                cout << endl << endl << "Writing basis to " << fileOut << "...";
                XLP -> Write(fileOut);
            }
            else
            {
                cout << endl << endl << "Writing basis to " << basFile << "...";
                XLP -> Write(basFile,mipInstance::BAS_CPLEX);
            }
        }

        delete XLP;

        cout << endl << endl << "Allocated " << goblinMaxSize << " bytes." << endl << endl;
    }
    catch (...)
    {
        cout << endl << endl << "...File import failed" << endl;
        ret = 1;
    }

    if (CT->logStream != &clog)
    {
        delete CT->logStream;
        CT->logStream = &clog;
    }

    delete CT;
    delete[] gobIn;
    delete[] fileIn;
    delete[] fileOut;
    delete[] basFile;
    delete[] logFile;

    return ret;
}
