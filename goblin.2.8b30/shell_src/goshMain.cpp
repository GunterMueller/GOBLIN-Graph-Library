
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, March 2001
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   goshMain.cpp
/// \brief  Tcl shell command procedures for the GOBLIN library root object

#include "gosh.h"
#include "messenger.h"


goblinController *CT = NULL;
bool destroyThread = false;

static goblinMessenger *MSG = NULL;
static long unsigned goblinInterprCounter = 0;
static char* transscriptName = NULL;
static Tcl_Interp *masterInterp = NULL;
static char returnCode[255] = "";
static int errorCode = TCL_OK;


TThreadData goblinThreadData[MAX_NUM_THREADS];


void Goblin_InitThreadData() throw()
{
    for (unsigned i=0;i<MAX_NUM_THREADS;i++)
    {
        goblinThreadData[i].threadID = 0;
    }
}


unsigned Goblin_ReserveThreadIndex() throw()
{
    for (unsigned i=0;i<MAX_NUM_THREADS;i++)
    {
        if (goblinThreadData[i].threadID==0) return i;
    }

    return MAX_NUM_THREADS;
}


unsigned Goblin_MyThreadIndex() throw()
{
    pthread_t thisThreadID = pthread_self();

    for (unsigned i=0;i<MAX_NUM_THREADS;i++)
    {
        if (thisThreadID==goblinThreadData[i].threadID) return i;
    }

    return MAX_NUM_THREADS;
}


void Goblin_FreeThreadData() throw()
{
    pthread_t thisThreadID = pthread_self();

    for (unsigned i=0;i<MAX_NUM_THREADS;i++)
    {
        if (thisThreadID==goblinThreadData[i].threadID)
        {
            goblinThreadData[i].threadID = 0;
            return;
        }
    }

    return;
}


void Goblin_Config(goblinController& context) throw()
{
    char* configName = new char[strlen(getenv("HOME"))+9];
    sprintf(configName,"%s/.goshrc",getenv("HOME"));
    ifstream test(configName);

    if (test)
    {
        try
        {
            context.ReadConfiguration(configName);

            sprintf(CT->logBuffer,
                 "...configuration file \"%s\" read",configName);
            CT -> LogEntry(LOG_IO,NoHandle,CT->logBuffer);
        }
        catch (...) {}
    }

    delete[] configName;
}


void Goblin_TerminateHandler()
{
    CT -> logLevel = 0;

    sprintf(CT->logBuffer,"...Unhandled exception, caught by terminate handler");
    CT -> LogEntry(LOG_SHELL,NoHandle,CT->logBuffer);

    longjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer,1);
}


int Goblin_Init(Tcl_Interp *interp)
{
    set_terminate(Goblin_TerminateHandler);

    if (goblinInterprCounter==0)
    {
        Goblin_InitThreadData();
        goblinThreadData[Goblin_ReserveThreadIndex()].threadID = pthread_self();

        transscriptName = new char[strlen(getenv("HOME"))+10];
        sprintf(transscriptName,"%s/gosh.log",getenv("HOME"));
        masterInterp = interp;
        CT = new goblinController(goblinDefaultContext);
        CT->logStream         = new ofstream(transscriptName,ios::app);
        CT->traceLevel        = 0;
        CT->traceEventHandler = &Goblin_TraceEventHandler;
        CT->logEventHandler   = &Goblin_LogEventHandler;
        CT->stopSignalHandler = &Goblin_StopSignalHandler;
        MSG = new goblinMessenger(100,*CT);

        Goblin_Config(goblinDefaultContext);
        Goblin_Config(*CT);
    }
    else CT -> LogEntry(LOG_SHELL,NoHandle,"Starting GOSH interpreter...");

    goblinInterprCounter++;

    Tcl_CreateCommand(interp,"goblin",Goblin_Cmd,(ClientData)NULL,
                      (Tcl_CmdDeleteProc *)Goblin_Delete);

    Tcl_SetVar(interp,"goblinMasterEvent","0",0);

    Tcl_LinkVar(interp,"goblinZoom",(char*)&(CT->displayZoom),TCL_LINK_DOUBLE);
    Tcl_LinkVar(interp,"goblinPixelWidth",(char*)&(CT->pixelWidth),TCL_LINK_DOUBLE);
    Tcl_LinkVar(interp,"goblinPixelHeight",(char*)&(CT->pixelHeight),TCL_LINK_DOUBLE);
    Tcl_LinkVar(interp,"goblinLegenda",(char*)&(CT->legenda),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinWallpaper",(char*)&(CT->wallpaper),
        TCL_LINK_STRING | TCL_LINK_READ_ONLY);

    Tcl_LinkVar(interp,"goblinDisplayMode",(char*)&(CT->displayMode),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinTraceLevel",(char*)&(CT->traceLevel),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinTraceData",(char*)&(CT->traceData),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinTraceStep",(char*)&(CT->traceStep),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinThreshold",(char*)&(CT->threshold),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinTraceCounter",(char*)&(CT->fileCounter),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinCommLevel",(char*)&(CT->commLevel),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinBreakLevel",(char*)&(CT->breakLevel),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogLevel",(char*)&(CT->logLevel),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinLogMethods",(char*)&(CT->logMeth),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogMemory",(char*)&(CT->logMem),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogManipulate",(char*)&(CT->logMan),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogIO",(char*)&(CT->logIO),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogResults",(char*)&(CT->logRes),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogWarnings",(char*)&(CT->logWarn),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogTimers",(char*)&(CT->logTimers),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinLogGaps",(char*)&(CT->logGaps),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinMethFailSave",(char*)&(CT->methFailSave),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethDSU",(char*)&(CT->methDSU),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethPQ",(char*)&(CT->methPQ),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethModLength",(char*)&(CT->methModLength),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinMethSPX",(char*)&(CT->methSPX),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMST",(char*)&(CT->methMST),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMXF",(char*)&(CT->methMXF),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMCC",(char*)&(CT->methMCC),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMCFST",(char*)&(CT->methMCFST),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMCF",(char*)&(CT->methMCF),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethNWPricing",(char*)&(CT->methNWPricing),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMaxBalFlow",(char*)&(CT->methMaxBalFlow),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethBNS",(char*)&(CT->methBNS),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMinCBalFlow",(char*)&(CT->methMinCBalFlow),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethPrimalDual",(char*)&(CT->methPrimalDual),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethCandidates",(char*)&(CT->methCandidates),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethColour",(char*)&(CT->methColour),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethHeurTSP",(char*)&(CT->methHeurTSP),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethRelaxTSP1",(char*)&(CT->methRelaxTSP1),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethRelaxTSP2",(char*)&(CT->methRelaxTSP2),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethMaxCut",(char*)&(CT->methMaxCut),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinMethSolve",(char*)&(CT->methSolve),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethLocal",(char*)&(CT->methLocal),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMaxBBIterations",(char*)&(CT->maxBBIterations),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMaxBBNodes",(char*)&(CT->maxBBNodes),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinMethLP",(char*)&(CT->methLP),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethLPPricing",(char*)&(CT->methLPPricing),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethLPQTest",(char*)&(CT->methLPQTest),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethLPStart",(char*)&(CT->methLPStart),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinMethFDP",(char*)&(CT->methFDP),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethPlanarity",(char*)&(CT->methPlanarity),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethOrthogonal",(char*)&(CT->methOrthogonal),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinMethOrthoRefine",(char*)&(CT->methOrthoRefine),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinRandMin",(char*)&(CT->randMin),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandMax",(char*)&(CT->randMax),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandUCap",(char*)&(CT->randUCap),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandLCap",(char*)&(CT->randLCap),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandLength",(char*)&(CT->randLength),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandParallels",(char*)&(CT->randParallels),TCL_LINK_INT);
    Tcl_LinkVar(interp,"goblinRandGeometry",(char*)&(CT->randGeometry),TCL_LINK_INT);

    Tcl_LinkVar(interp,"goblinExtension",(char*)&(CT->objectExtension),
                TCL_LINK_STRING|TCL_LINK_READ_ONLY);

    Tcl_CreateCommand(interp,"exit",Goblin_Exit,(ClientData)NULL,
                      (Tcl_CmdDeleteProc *)NULL);

    char version[10];
    sprintf(version,"%d.%d",CT->MajorVersion(),CT->MinorVersion());
    Tcl_PkgProvide(interp,"goblin",version);

    return TCL_OK;
}


// Propagate event in a thread to Tcl scripts by value changes of the
// variable goblinMasterEvent

static bool eventToggle = false;

void Goblin_MasterEventHandler()
{
    if (!eventToggle)
        Tcl_SetVar(masterInterp,"goblinMasterEvent","0",0);
    else
        Tcl_SetVar(masterInterp,"goblinMasterEvent","1",0);

    eventToggle = !eventToggle;
}


void Goblin_TraceEventHandler(char* traceFileName)
{
    if (   CT->commLevel<3
        || CT->logLevel>CT->breakLevel
        || !(CT->SolverRunning())
       )
    {
        return;
    }

    MSG -> SignalTraceEvent(traceFileName);
    Goblin_MasterEventHandler();
    MSG -> TraceSemTake();
}


void Goblin_LogEventHandler(msgType msg,TModule mod,THandle OH,char* logText)
{
    CT -> DefaultLogEventHandler(msg,mod,OH,logText);
    MSG -> MsgAppend(msg,mod,OH,CT->logLevel,logText);
    Goblin_MasterEventHandler();
}


bool Goblin_StopSignalHandler()
{
    return (MSG->SolverRunning() || CT->commLevel==0);
}


// Entry point for threaded script evaluation

void* Goblin_Thread(void* scriptName)
{
    while (Goblin_MyThreadIndex()>=MAX_NUM_THREADS) {};

    Tcl_Interp* interp2 = Tcl_CreateInterp();
    Goblin_Init(interp2);

    MSG -> SolverSignalStarted();
    sprintf(CT->logBuffer,"Evaluating \"%s\"...",(char*)scriptName);
    CT -> LogEntry(LOG_IO,NoHandle,CT->logBuffer);

    #if defined(_TIMERS_)

    CT -> globalTimer[TimerSolve] -> Enable();

    #endif

    if (Tcl_EvalFile(interp2,(char*)scriptName)==TCL_ERROR)
    {
        sprintf(CT->logBuffer,"...Thread exit status: %s",interp2->result);
        CT -> LogEntry(LOG_IO,NoHandle,CT->logBuffer);
    }

    #if defined(_TIMERS_)

    CT -> globalTimer[TimerSolve] -> Disable();

    #endif

    delete[] (char*)scriptName;
    MSG -> SolverSignalIdle();

    destroyThread = true;
    Tcl_DeleteInterp(interp2);
    destroyThread = false;

    #if defined(_TIMERS_)

    sprintf(CT->logBuffer,"Thread execution time: %.0f ms",
        CT->globalTimer[TimerSolve]->PrevTime());
    CT -> LogEntry(LOG_TIMERS,NoHandle,CT->logBuffer);

    #endif

    CT -> LogEntry(LOG_SHELL,NoHandle,"");

    Goblin_FreeThreadData();
    pthread_exit(NULL);

    return NULL;
}


// Tcl command evaluations start here

int Goblin_Cmd (ClientData clientData,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[])
{
    if (argc<2)
    {
        WrongNumberOfArguments(interp,argc,argv);
        return TCL_ERROR;
    }

    if (setjmp(goblinThreadData[Goblin_MyThreadIndex()].jumpBuffer) != 0)
    {
        return Goblin_Propagate_Exception(interp);
    }
    else try
    {
        int pos = CT->FindParam(argc,&argv[0],"-nodes",2);
        TNode n = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 1;

        if (strcmp(argv[1],"mixedGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            mixedGraph *G = new mixedGraph(n,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"sparseGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            sparseGraph *G = new sparseGraph(n,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"sparseBiGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            pos = CT->FindParam(argc,&argv[0],"-leftNodes",2);

            TNode n1 = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : (n+1)/2;

            pos = CT->FindParam(argc,&argv[0],"-rightNodes",2);

            TNode n2 = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : n/2;

            sparseBiGraph *G = new sparseBiGraph(n1,n2,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Bigraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Bigraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"sparseDiGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            sparseDiGraph *G = new sparseDiGraph(n,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"denseGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            denseGraph* G = new denseGraph(n,(TOption)0,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Dense_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"denseBiGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            pos = CT->FindParam(argc,&argv[0],"-leftNodes",2);
            TNode n1 = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : (n+1)/2;

            pos = CT->FindParam(argc,&argv[0],"-rightNodes",2);
            TNode n2 = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : n/2;

            denseBiGraph* G = new denseBiGraph(n1,n2,(TOption)0,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Dense_Bigraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Bigraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"denseDiGraph")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            denseDiGraph* G = new denseDiGraph(n,(TOption)0,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Dense_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Digraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"openGrid")==0)
        {
            TNode nRows = 4;
            TNode nColumns = 3;
            openGrid::TOptGrid shape = openGrid::GRID_SQUARE;

            int pos = CT->FindParam(argc,&argv[0],"-rows",2);

            if (pos>0 && pos<argc-1)
            {
                nRows = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-columns",2);

            if (pos>0 && pos<argc-1)
            {
                nColumns = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-triangular",2);

            if (pos>0 && pos<argc-1)
            {
                shape = openGrid::GRID_TRIANGULAR;
            }

            pos = CT->FindParam(argc,&argv[0],"-square",2);

            if (pos>0 && pos<argc-1)
            {
                shape = openGrid::GRID_SQUARE;
            }

            pos = CT->FindParam(argc,&argv[0],"-hexagonal",2);

            if (pos>0 && pos<argc-1)
            {
                shape = openGrid::GRID_HEXAGONAL;
            }

            sparseGraph* G = new openGrid(nRows,nColumns,shape,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"gridCompletion")==0)
        {
            TNode nRows = 4;
            gridCompletion::TOptShape shape = gridCompletion::SHAPE_SQUARE;

            int pos = CT->FindParam(argc,&argv[0],"-rows",2);

            if (pos>0 && pos<argc-1)
            {
                nRows = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-columns",2);

            if (pos>0 && pos<argc-1)
            {
                nRows = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-triangular",2);

            if (pos>0 && pos<argc-1)
            {
                shape = gridCompletion::SHAPE_TRIANGULAR;
            }

            pos = CT->FindParam(argc,&argv[0],"-square",2);

            if (pos>0 && pos<argc-1)
            {
                shape = gridCompletion::SHAPE_SQUARE;
            }

            pos = CT->FindParam(argc,&argv[0],"-hexagonal",2);

            if (pos>0 && pos<argc-1)
            {
                shape = gridCompletion::SHAPE_HEXAGONAL;
            }

            sparseGraph* G = new gridCompletion(nRows,shape,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"polarGrid")==0)
        {
            TNode nRows = 1;
            int pos = CT->FindParam(argc,&argv[0],"-rows",2);

            if (pos>0 && pos<argc-1)
            {
                nRows = atol(argv[pos+1]);
            }

            TNode nColumns = 3;
            pos = CT->FindParam(argc,&argv[0],"-columns",2);

            if (pos>0 && pos<argc-1)
            {
                nColumns = atol(argv[pos+1]);
            }

            TNode nPoles = 0;
            pos = CT->FindParam(argc,&argv[0],"-poles",2);

            if (pos>0 && pos<argc-1)
            {
                nPoles = atol(argv[pos+1]);
            }

            polarGrid::TOptPolar facetType = polarGrid::POLAR_DEFAULT;
            pos = CT->FindParam(argc,&argv[0],"-triangular",2);

            if (pos>0 && pos<argc-1)
            {
                facetType = polarGrid::POLAR_TRIANGULAR;
            }

            pos = CT->FindParam(argc,&argv[0],"-quadrilateral",2);

            if (pos>0 && pos<argc-1)
            {
                facetType = polarGrid::POLAR_SQUARE;
            }

            polarGrid::TOptPolar embed2D = polarGrid::POLAR_DEFAULT;
            pos = CT->FindParam(argc,&argv[0],"-tube",2);

            if (pos>0 && pos<argc-1)
            {
                embed2D = polarGrid::POLAR_TUBE;
            }

            pos = CT->FindParam(argc,&argv[0],"-cone",2);

            if (pos>0 && pos<argc-1)
            {
                embed2D = polarGrid::POLAR_CONE;
            }

            pos = CT->FindParam(argc,&argv[0],"-hemisphere",2);

            if (pos>0 && pos<argc-1)
            {
                embed2D = polarGrid::POLAR_HEMISPHERE;
            }

            pos = CT->FindParam(argc,&argv[0],"-sphere",2);

            if (pos>0 && pos<argc-1)
            {
                embed2D = polarGrid::POLAR_SPHERE;
            }

            pos = CT->FindParam(argc,&argv[0],"-disc",2);

            if (pos>0 && pos<argc-1)
            {
                embed2D = polarGrid::POLAR_DISC;
            }

            sparseGraph* G = new polarGrid(nRows,nColumns,nPoles,facetType,embed2D,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"toroidalGrid")==0)
        {
            unsigned short height = 9;
            unsigned short width = 15;
            unsigned short hSkew = 0;
            short vSkew = 0;
            toroidalGrid::TOptTorus facetType = toroidalGrid::TORUS_QUADRILATERAL;

            int pos = CT->FindParam(argc,&argv[0],"-lineParameters",2);

            if (pos>0 && pos<argc-4)
            {
                hSkew  = atol(argv[pos+1]);
                height = atol(argv[pos+2]);
                vSkew  = atol(argv[pos+3]);
                width  = atol(argv[pos+4]);
            }

            pos = CT->FindParam(argc,&argv[0],"-height",2);

            if (pos>0 && pos<argc-1)
            {
                height = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-width",2);

            if (pos>0 && pos<argc-1)
            {
                width = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-triangular",2);

            if (pos>0 && pos<argc-1)
            {
                facetType = toroidalGrid::TORUS_TRIANGULAR;
            }

            pos = CT->FindParam(argc,&argv[0],"-quadrilateral",2);

            if (pos>0 && pos<argc-1)
            {
                facetType = toroidalGrid::TORUS_QUADRILATERAL;
            }

            pos = CT->FindParam(argc,&argv[0],"-hexagonal",2);

            if (pos>0 && pos<argc-1)
            {
                facetType = toroidalGrid::TORUS_HEXAGONAL;
            }

            sparseGraph* G = new toroidalGrid(hSkew,height,vSkew,width,facetType,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"moebiusLadder")==0)
        {
            TNode numNodes = RequireNodes(interp,argc,argv);

            if (numNodes==NoNode) return TCL_ERROR;

            sparseGraph* G = new moebiusLadder(numNodes,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"generalizedPetersen")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-perimeter",2);
            TNode perimeter = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 5;

            pos = CT->FindParam(argc,&argv[0],"-skew",2);
            TNode skew = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 2;

            sparseGraph* G = new generalizedPetersen(perimeter,skew,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"regularTree")==0)
        {
            n = RequireNodes(interp,argc,argv);

            if (n==NoNode) return TCL_ERROR;

            int pos = CT->FindParam(argc,&argv[0],"-depth",2);
            TNode depth = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 3;

            pos = CT->FindParam(argc,&argv[0],"-deg",2);
            TNode deg = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 2;

            sparseDiGraph* G = new regularTree(depth,deg,n,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"butterflyGraph")==0)
        {
            TNode length = 3;
            TNode base   = 2;

            int pos = CT->FindParam(argc,&argv[0],"-length",2);

            if (pos>0 && pos<argc-1) length = atol(argv[pos+1]);

            pos = CT->FindParam(argc,&argv[0],"-base",2);

            if (pos>0 && pos<argc-1) base = atol(argv[pos+1]);

            sparseDiGraph* G = new butterflyGraph(length,base,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"cyclicButterfly")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-length",2);
            TNode length = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 3;

            pos = CT->FindParam(argc,&argv[0],"-base",2);
            TNode base = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 2;

            sparseDiGraph* G = new cyclicButterfly(length,base,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"sierpinskiTriangle")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-depth",2);
            TNode depth = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 3;

            sparseGraph* G = new sierpinskiTriangle(depth,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"mycielskianGraph")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-depth",2);
            TNode depth = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 2;

            sparseGraph *G = new mycielskianGraph(depth,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"triangularGraph")==0)
        {
            int pos = CT->FindParam(argc,&argv[0],"-cardinality",2);
            TNode cardinality = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 4;

            sparseGraph* G = new triangularGraph(cardinality,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"intersectionGraph")==0)
        {
            TNode groundSetCardinality = 5;
            TNode subsetCardinality = 2;
            TNode lowerMeetCardinality = 0;
            TNode upperMeetCardinality = 0;

            int pos = CT->FindParam(argc,&argv[0],"-groundSet",2);

            if (pos>0 && pos<argc-1)
            {
                groundSetCardinality = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-subset",2);

            if (pos>0 && pos<argc-1)
            {
                subsetCardinality = atol(argv[pos+1]);
            }

            pos = CT->FindParam(argc,&argv[0],"-intersection",2);

            if (pos>0 && pos<argc-2)
            {
                lowerMeetCardinality = atol(argv[pos+1]);
                upperMeetCardinality = atol(argv[pos+2]);
            }

            sparseGraph* G = new intersectionGraph(groundSetCardinality,subsetCardinality,
                                             lowerMeetCardinality,upperMeetCardinality,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"permutationGraph")==0)
        {
            TNode numNodes = RequireNodes(interp,argc,argv);

            if (numNodes==NoNode) return TCL_ERROR;

            sparseGraph* G = new permutationGraph(numNodes,NULL,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"thresholdGraph")==0)
        {
            TNode numNodes = RequireNodes(interp,argc,argv);

            if (numNodes==NoNode) return TCL_ERROR;

            int pos = CT->FindParam(argc,&argv[0],"-threshold",2);
            TFloat threshold = (pos>0 && pos<argc-1) ? atof(argv[pos+1]) : 0;

            pos = CT->FindParam(argc,&argv[0],"-min",2);
            long minRange = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : 0;

            pos = CT->FindParam(argc,&argv[0],"-max",2);
            long maxRange = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : minRange+long(numNodes);

            sparseGraph* G = new thresholdGraph(numNodes,threshold,minRange,maxRange,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"intervalGraph")==0)
        {
            TNode numNodes = RequireNodes(interp,argc,argv);

            if (numNodes==NoNode) return TCL_ERROR;

            int pos = CT->FindParam(argc,&argv[0],"-range",2);
            TIndex range = (pos>0 && pos<argc-1) ? atol(argv[pos+1]) : TIndex(numNodes);
            sparseGraph* G = new intervalGraph(numNodes,range,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-1]),
                Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"ilp")==0 ||
            strcmp(argv[1],"lp")==0 ||
            strcmp(argv[1],"mip")==0)
        {
            if (goblinController::pMipFactory==NULL)
            {
                interp->result = "No LP solver loaded";
                return TCL_ERROR;
            }

            if (argc!=5 && argc!=6)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            TIndex k = TIndex(atol(argv[3]));
            TIndex l = TIndex(atol(argv[4]));
            TIndex nz = k*l/10+1;

            if (argc==6) nz = TIndex(atol(argv[5]));

            mipFactory* theMipFactory =
                reinterpret_cast<mipFactory*>(
                    const_cast<char*>(goblinController::pMipFactory));
            mipInstance* XLP  =
                theMipFactory->NewInstance(k,l,nz,mipInstance::MINIMIZE,*CT);

            Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(XLP),
                (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"read")==0)
        {
            if (argc<4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            goblinController* thisContext = CT;

            // Files contain only those configuration parameters which differ
            // from the default values. So it is required to restore these
            // default values before loading a file.
            if (CT->FindParam(argc,argv,"-spawn",2))
            {
                thisContext = new goblinController(goblinDefaultContext);
            }
            else
            {
                Goblin_Config(*thisContext);
            }


            int formatIndex = CT->FindParam(argc,argv,"-format",2);

            const char* formatName =
                (formatIndex>0 && formatIndex<argc-2) ?
                argv[formatIndex+1] : "goblin";

            managedObject* X = thisContext->
                ImportByFormatName(argv[argc-1],formatName);

            if (!X)
            {
                interp->result = "Unknown format specification";
                return TCL_ERROR;
            }


            if (X->IsMixedILP())
            {
                mipInstance* XLP = dynamic_cast<mipInstance*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                    Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(XLP),
                    (Tcl_CmdDeleteProc *)Goblin_Delete_Ilp);
            }
            else if (dynamic_cast<abstractMixedGraph*>(X)->IsDirected())
            {
                if (dynamic_cast<abstractMixedGraph*>(X)->IsBalanced())
                {
                    balancedFNW* G = dynamic_cast<balancedFNW*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Balanced_FNW_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Balanced_FNW);
                }
                else if (dynamic_cast<abstractMixedGraph*>(X)->IsSparse())
                {
                    sparseDiGraph* G = dynamic_cast<sparseDiGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Digraph);
                }
                else
                {
                    denseDiGraph* G = dynamic_cast<denseDiGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Dense_Digraph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Digraph);
                }
            }
            else if (dynamic_cast<abstractMixedGraph*>(X)->IsBipartite())
            {
                if (dynamic_cast<abstractMixedGraph*>(X)->IsSparse())
                {
                    sparseBiGraph* G = dynamic_cast<sparseBiGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Sparse_Bigraph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Bigraph);
                }
                else
                {
                    denseBiGraph* G = dynamic_cast<denseBiGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Dense_Bigraph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Bigraph);
                }
            }
            else if (dynamic_cast<abstractMixedGraph*>(X)->IsUndirected())
            {
                if (dynamic_cast<abstractMixedGraph*>(X)->IsSparse())
                {
                    sparseGraph* G = dynamic_cast<sparseGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Sparse_Graph);
                }
                else
                {
                    denseGraph* G = dynamic_cast<denseGraph*>(X);

                    Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                        Goblin_Dense_Graph_Cmd,reinterpret_cast<ClientData>(G),
                        (Tcl_CmdDeleteProc *)Goblin_Delete_Dense_Graph);
                }
            }
            else
            {
                mixedGraph* G = dynamic_cast<mixedGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[argc-2]),
                    Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(G),
                    (Tcl_CmdDeleteProc *)Goblin_Delete_Mixed_Graph);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"alias")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            goblinRootObject *X = CT->ObjectPointer(THandle(atol(argv[3])));

            if (dynamic_cast<mipInstance*>(X))
            {
                mipInstance *Y = dynamic_cast<mipInstance*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Ilp_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (!X->IsGraphObject())
            {
                sprintf(interp->result,"Not a graph object ID: %s",argv[3]);
                return TCL_ERROR;
            }

            if (dynamic_cast<mixedGraph*>(X))
            {
                mixedGraph *Y = dynamic_cast<mixedGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Mixed_Graph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<sparseGraph*>(X))
            {
                sparseGraph *Y = dynamic_cast<sparseGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Sparse_Graph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<denseGraph*>(X))
            {
                denseGraph *Y = dynamic_cast<denseGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Dense_Graph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<sparseBiGraph*>(X))
            {
                sparseBiGraph *Y = dynamic_cast<sparseBiGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Sparse_Bigraph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<denseBiGraph*>(X))
            {
                denseBiGraph *Y = dynamic_cast<denseBiGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Dense_Bigraph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<sparseDiGraph*>(X))
            {
                sparseDiGraph *Y = dynamic_cast<sparseDiGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Sparse_Digraph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<denseDiGraph*>(X))
            {
                denseDiGraph *Y = dynamic_cast<denseDiGraph*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Dense_Digraph_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            if (dynamic_cast<balancedFNW*>(X))
            {
                balancedFNW *Y = dynamic_cast<balancedFNW*>(X);

                Tcl_CreateCommand(interp,const_cast<char*>(argv[2]),
                    Goblin_Balanced_FNW_Cmd,reinterpret_cast<ClientData>(Y),NULL);

                interp->result = "";
                return TCL_OK;
            }

            interp->result = "Unknown object type";
            return TCL_ERROR;
        }

        if (argc>5)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"export")==0)
        {
            if (strcmp(argv[2],"settings")==0)
            {
                char* configName = new char[strlen(getenv("HOME"))+9];
                sprintf(configName,"%s/.goshrc",getenv("HOME"));

                try
                {
                    goblinExport* F = new goblinExport(configName,*CT);
                    F -> WriteConfiguration(*CT,goblinExport::CONF_FULL);
                    delete F;
                    sprintf(CT->logBuffer,
                         "...configuration file \"%s\" written",configName);
                    CT -> LogEntry(LOG_IO,NoHandle,CT->logBuffer);
                }
                catch (...) {}

                delete[] configName;

                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin export %s",argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"echo")==0)
        {
            static THandle LH = NoHandle;

            if (strcmp(argv[2],"-nonewline")==0)
            {
                if (LH!=NoHandle) CT->LogAppend(LH,(char*)argv[3]);
                else LH = CT->LogStart(MSG_ECHO,NoHandle,(char*)argv[3]);
            }
            else 
            {
                if (LH!=NoHandle)
                {
                    CT->LogEnd(LH,(char*)argv[2]);
                    LH = NoHandle;
                }
                else CT->LogEntry(MSG_ECHO,NoHandle,(char*)argv[2]);
            }

            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[1],"solver")==0)
        {
            if (strcmp(argv[2],"thread")==0)
            {
                if (argc!=4)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                unsigned threadIndex = Goblin_ReserveThreadIndex();

                if (threadIndex>=MAX_NUM_THREADS)
                {
                    interp->result = "No more thread handle available";
                    return TCL_ERROR;
                }

                MSG -> SolverSignalPending();
                char* scriptName = new char[strlen(argv[3])+1];
                strcpy(scriptName,argv[3]);
                pthread_create(&goblinThreadData[threadIndex].threadID,
                                NULL,Goblin_Thread,(void*)scriptName);

                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"return")==0)
            {
                if (argc!=4)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                while (MSG->SolverIdle()) {};

                sprintf(returnCode,"%s",argv[3]);
                errorCode = TCL_OK;
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"throw")==0)
            {
                if (argc!=4)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                while (MSG->SolverIdle()) {};

                sprintf(returnCode,"%s",argv[3]);
                errorCode = TCL_ERROR;
                interp->result = "";
                return TCL_OK;
            }

            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"stop")==0)
            {
                MSG -> SolverSignalStop();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"idle")==0)
            {
                if (MSG->SolverIdle())
                    interp->result = "1";
                else interp->result = "0";

                return TCL_OK;
            }

            if (strcmp(argv[2],"running")==0)
            {
                if (MSG->SolverRunning())
                    interp->result = "1";
                else interp->result = "0";

                return TCL_OK;
            }

            if (strcmp(argv[2],"result")==0)
            {
                Tcl_SetResult(interp,returnCode,TCL_VOLATILE);
                return errorCode;
            }

            if (strcmp(argv[2],"progress")==0)
            {
                #if defined(_PROGRESS_)

                sprintf(interp->result,"%g",
                    static_cast<double>(CT->ProgressCounter()));

                #else

                interp->result = "1.0";

                #endif

                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin solver %s",argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"messenger")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"getLine")==0)
            {
                try
                {
                    unsigned long lineNumber = atol(argv[3]);
                    MSG->GetLineByNumber(transscriptName,interp->result, TCL_RESULT_SIZE,lineNumber);
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Could not access transcript file";
                    return TCL_ERROR;
                }
        }

            if (strcmp(argv[2],"read")==0)
            {
                try
                {
                    unsigned long lineNumber = atol(argv[3]);
                    MSG->LoadBuffer(transscriptName,lineNumber);
                    interp->result = "";
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Could not access transcript file";
                    return TCL_ERROR;
                }
            }

            if (argc!=3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"#bufferSize")==0)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(MSG->GetBufferSize()));
                return TCL_OK;
            }

            if (strcmp(argv[2],"#numLines")==0)
            {
                sprintf(interp->result,"%lu",static_cast<unsigned long>(MSG->GetNumLines(transscriptName)));
                return TCL_OK;
            }

            if (strcmp(argv[2],"restart")==0)
            {
                MSG -> Restart();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"reset")==0)
            {
                MSG -> MsgReset();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[2],"eof")==0)
            {
                interp->result = const_cast<char*>((MSG->MsgEndOfBuffer()) ? "1" : "0");
                return TCL_OK;
            }

            if (strcmp(argv[2],"void")==0)
            {
                interp->result = const_cast<char*>((MSG->MsgVoid()) ? "1" : "0");
                return TCL_OK;
            }

            if (strcmp(argv[2],"skip")==0)
            {
                try
                {
                    MSG -> MsgSkip();
                    interp->result = "";
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"text")==0)
            {
                try
                {
                    MSG->MsgText(interp->result, TCL_RESULT_SIZE);
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"class")==0)
            {
                try
                {
                    msgType ret = MSG->MsgClass();
                    sprintf(interp->result,"%d",ret);
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"module")==0)
            {
                try
                {
                    TModule ret = MSG->MsgModule();
                    sprintf(interp->result,"%lu",static_cast<unsigned long>(ret));
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"handle")==0)
            {
                try
                {
                    THandle ret = MSG->MsgHandle();
                    sprintf(interp->result,"%lu",static_cast<unsigned long>(ret));
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"level")==0)
            {
                try
                {
                    int ret = MSG->MsgLevel();
                    sprintf(interp->result,"%d",ret);
                    return TCL_OK;
                }
                catch (ERRejected)
                {
                    interp->result = "Message queue is empty";
                    return TCL_ERROR;
                }
            }

            if (strcmp(argv[2],"filename")==0)
            {
                if (argc!=3)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                size_t numChars = MSG->TraceFilename(interp->result, TCL_RESULT_SIZE);

                if (numChars==0)
                {
                    interp->result = "Tcl result size overflow";
                    return TCL_ERROR;
                }
                else if (numChars>=TCL_RESULT_SIZE)
                {
                    interp->result = "No more queued trace files";
                    return TCL_ERROR;
                }

                return TCL_OK;
            }

            if (strcmp(argv[2],"blocked")==0)
            {
                interp->result = const_cast<char*>((MSG->TraceEvent()) ? "1" : "0");
                return TCL_OK;
            }

            if (strcmp(argv[2],"unblock")==0)
            {
                MSG -> TraceUnblock();
                interp->result = "";
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin messenger %s",argv[2]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"configure")==0)
        {
            CT->Configure(argc,(const char**)argv);
            interp->result = "";
            return TCL_OK;
        }

        #if defined(_TIMERS_)

        if (strcmp(argv[1],"timer")==0)
        {
            if (argc<3)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[2],"reset")==0)
            {
                CT -> ResetTimers();
                interp->result = "";
                return TCL_OK;
            }

            unsigned i = atol(argv[2]);

            if (i>=NoTimer)
            {
                sprintf(interp->result,"No such timer: %u",i);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"child")==0)
            {
                if (argc!=5)
                {
                    WrongNumberOfArguments(interp,argc,argv);
                    return TCL_ERROR;
                }

                unsigned j = atol(argv[4]);

                if (j>=NoTimer)
                {
                    sprintf(interp->result,"No such Timer: %u",j);
                    return TCL_ERROR;
                }

                sprintf(interp->result,"%g",
                    static_cast<double>(CT->globalTimer[i]->ChildTime(TTimer(j))));
                return TCL_OK;
            }

            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"reset")==0)
            {
                CT -> globalTimer[i] -> Reset();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"enable")==0)
            {
                CT -> globalTimer[i] -> Enable();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"disable")==0)
            {
                CT -> globalTimer[i] -> Disable();
                interp->result = "";
                return TCL_OK;
            }

            if (strcmp(argv[3],"label")==0)
            {
                sprintf(interp->result,"%s",
                    listOfTimers[i].timerName);
                return TCL_OK;
            }

            if (strcmp(argv[3],"acc")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(CT->globalTimer[i]->AccTime()));
                return TCL_OK;
            }

            if (strcmp(argv[3],"av")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(CT->globalTimer[i]->AvTime()));
                return TCL_OK;
            }

            if (strcmp(argv[3],"max")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(CT->globalTimer[i]->MaxTime()));
                return TCL_OK;
            }

            if (strcmp(argv[3],"min")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(CT->globalTimer[i]->MinTime()));
                return TCL_OK;
            }

            if (strcmp(argv[3],"prev")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(CT->globalTimer[i]->PrevTime()));
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin timer <index> %s",argv[3]);
            return TCL_ERROR;
        }

        #endif

        if (strcmp(argv[1],"module")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            unsigned i = atol(argv[2]);

            if (i>=NoModule)
            {
                sprintf(interp->result,"No such module: %d",i);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"name")==0)
            {
                sprintf(interp->result,"%s",listOfModules[i].moduleName);
                return TCL_OK;
            }

            if (strcmp(argv[3],"timer")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].moduleTimer));
                return TCL_OK;
            }

            if (strcmp(argv[3],"implementor1")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].implementor1));
                return TCL_OK;
            }

            if (strcmp(argv[3],"implementor2")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].implementor2));
                return TCL_OK;
            }

            if (strcmp(argv[3],"encoding_date")==0)
            {
                sprintf(interp->result,"%s",listOfModules[i].encodingDate);
                return TCL_OK;
            }

            if (strcmp(argv[3],"revision_date")==0)
            {
                sprintf(interp->result,"%s",listOfModules[i].revisionDate);
                return TCL_OK;
            }

            if (strcmp(argv[3],"original_reference")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].originalReference));
                return TCL_OK;
            }

            if (strcmp(argv[3],"authors_reference")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].authorsReference));
                return TCL_OK;
            }

            if (strcmp(argv[3],"text_book")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfModules[i].textBook));
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin module <index> %s",argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"author")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            unsigned i = atol(argv[2]);

            if (i>=NoAuthor)
            {
                sprintf(interp->result,"No such author: %d",i);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"name")==0)
            {
                sprintf(interp->result,"%s",listOfAuthors[i].name);
                return TCL_OK;
            }

            if (strcmp(argv[3],"affiliation")==0)
            {
                sprintf(interp->result,"%s",listOfAuthors[i].affiliation);
                return TCL_OK;
            }

            if (strcmp(argv[3],"e_mail")==0)
            {
                sprintf(interp->result,"%s",listOfAuthors[i].e_mail);
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin author <index> %s",argv[3]);
            return TCL_ERROR;
        }

        if (strcmp(argv[1],"reference")==0)
        {
            if (argc!=4)
            {
                WrongNumberOfArguments(interp,argc,argv);
                return TCL_ERROR;
            }

            unsigned i = atol(argv[2]);

            if (i>=NoReference)
            {
                sprintf(interp->result,"No such reference: %d",i);
                return TCL_ERROR;
            }

            if (strcmp(argv[3],"key")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].refKey);
                return TCL_OK;
            }

            if (strcmp(argv[3],"authors")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].authors);
                return TCL_OK;
            }

            if (strcmp(argv[3],"title")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].title);
                return TCL_OK;
            }

            if (strcmp(argv[3],"type")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].type);
                return TCL_OK;
            }

            if (strcmp(argv[3],"collection")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].collection);
                return TCL_OK;
            }

            if (strcmp(argv[3],"editors")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].editors);
                return TCL_OK;
            }

            if (strcmp(argv[3],"volume")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfReferences[i].volume));
                return TCL_OK;
            }

            if (strcmp(argv[3],"publisher")==0)
            {
                sprintf(interp->result,"%s",listOfReferences[i].publisher);
                return TCL_OK;
            }

            if (strcmp(argv[3],"year")==0)
            {
                sprintf(interp->result,"%lu",
                    static_cast<unsigned long>(listOfReferences[i].year));
                return TCL_OK;
            }

            sprintf(interp->result,"Unknown option: goblin reference <index> %s",argv[3]);
            return TCL_ERROR;
        }

        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        #ifdef _HEAP_MON_

        if (strcmp(argv[1],"size")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(goblinHeapSize));
            return TCL_OK;
        }

        if (strcmp(argv[1],"maxsize")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(goblinMaxSize));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#allocs")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(goblinNAllocs));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#objects")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(goblinNObjects));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#fragments")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(goblinNFragments));
            return TCL_OK;
        }

        #endif

        if (strcmp(argv[1],"#timers")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(NoTimer));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#authors")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(NoAuthor));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#modules")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(NoModule));
            return TCL_OK;
        }

        if (strcmp(argv[1],"#references")==0)
        {
            sprintf(interp->result,"%lu",static_cast<unsigned long>(NoReference));
            return TCL_OK;
        }

        if (strcmp(argv[1],"restart")==0)
        {
            CT -> ResetCounters();
            CT -> ResetTimers();
            delete CT->logStream;
            CT->logStream = new ofstream(transscriptName);
            MSG -> Restart();

            interp->result = "";
            return TCL_OK;
        }
    }
    catch (...)
    {
        return Goblin_Propagate_Exception(interp);
    }

    sprintf(interp->result,"Unknown option: goblin %s",argv[1]);
    return TCL_ERROR;
}


int Goblin_Delete (ClientData clientData) throw()
{
    goblinInterprCounter--;

    if (goblinInterprCounter==0)
    {
        CT->logEventHandler = NULL;
        CT -> logStream -> flush();
        delete CT->logStream;
        CT->logStream = &clog;
        MSG -> Restart();
        delete MSG;
        delete CT;
        delete[] transscriptName;

    }
    else CT -> LogEntry(LOG_SHELL,NoHandle,"...GOSH interpreter halted");

    return TCL_OK;
}


int Goblin_Generic_Cmd (managedObject *X,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw(ERRejected,ERRange)
{
    if (argc<2)
    {
        interp->result = "Missing arguments";
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"master")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        CT -> SetMaster(X->Handle());
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"handle")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        sprintf(interp->result,"%ld",X->Handle());
        return TCL_OK;
    }

    if (strcmp(argv[1],"trace")==0)
    {
        if (argc!=2)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        X -> Display();
        interp->result = "";
        return TCL_OK;
    }

    if (strcmp(argv[1],"info")==0)
    {
        if (argc!=3)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"-mipObject")==0   ||
            strcmp(argv[2],"-graphObject")==0 ||
            strcmp(argv[2],"-sparse")==0      ||
            strcmp(argv[2],"-directed")==0    ||
            strcmp(argv[2],"-undirected")==0  ||
            strcmp(argv[2],"-bipartite")==0   ||
            strcmp(argv[2],"-balanced")==0
           )
        {
            interp->result = "0";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown object attribute: %s",argv[2]);
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"set")==0)
    {
        if (argc!=4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"name")==0)
        {
            X -> SetLabel((char*)argv[3]);
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown option: %s set %s",argv[0],argv[2]);
        return TCL_ERROR;
    }

    if (strcmp(argv[1],"export")==0)
    {
        if (argc<4)
        {
            WrongNumberOfArguments(interp,argc,argv);
            return TCL_ERROR;
        }

        if (strcmp(argv[2],"ascii")==0)
        {
            TOption opt = 0;

            if (argc>4) opt = atol(argv[4]);

            X -> ExportToAscii(argv[3],opt);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"goblet")==0 || strcmp(argv[2],"tk")==0)
        {
            X -> ExportToTk(argv[3]);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"xfig")==0)
        {
            X -> ExportToXFig(argv[3]);
            interp->result = "";
            return TCL_OK;
        }

        if (strcmp(argv[2],"dot")==0)
        {
            X -> ExportToDot(argv[3]);
            interp->result = "";
            return TCL_OK;
        }

        sprintf(interp->result,"Unknown export format: %s",argv[2]);
        return TCL_ERROR;
    }

    sprintf(interp->result,"Unknown option: %s",argv[1]);
    return TCL_ERROR;
}


int Goblin_Propagate_Exception (Tcl_Interp* interp) throw()
{
    if (CT->savedErrorMsgType != NO_MSG)
    {
        sprintf(interp->result,"%s - %s",
            CT->savedErrorMethodName,CT->savedErrorDescription);

        CT->savedErrorMsgType = NO_MSG;
    }
    else
    {
        CT->Error(MSG_WARN,NoHandle,"Goblin_Propagate_Exception",
            "An unknown exception has occured");
        sprintf(interp->result,"An unknown exception has occured");
    }

    CT -> logLevel = 0;
    return TCL_ERROR;
}


void WrongNumberOfArguments(Tcl_Interp* interp,int argc,_CONST_QUAL_ char* argv[]) throw()
{
    sprintf(interp->result,"Wrong number of arguments for command \"%s",argv[0]);

    for (int i=1;i<argc;++i)
    {
        sprintf(interp->result + strlen(interp->result)," %s",argv[i]);
    }

    sprintf(interp->result + strlen(interp->result),"\"");
}


TNode RequireNodes(Tcl_Interp* interp,int argc,_CONST_QUAL_ char* argv[]) throw()
{
    int pos = CT->FindParam(argc,&argv[0],"-nodes",2);

    if (pos>0 && pos<argc-1 && strcmp(argv[pos+1],"*")!=0)
    {
        return atol(argv[pos+1]);
    }
    else
    {
        interp->result = "Missing number of graph nodes";
        return NoNode;
    }
}


int Goblin_Exit (ClientData clientData,Tcl_Interp* interp,int argc,
    _CONST_QUAL_ char* argv[]) throw()
{
    Tcl_DeleteCommand(interp,"goblin");

    Goblin_FreeThreadData();

    exit(0);

    return TCL_OK;
}


managedObject *Goblin_Read_Object(const char* fileName,
    goblinController &thisContext) throw()
{
    try
    {
        goblinImport F(fileName,thisContext);
        F.DontComplain();

        char* type = F.Scan();

        if (strcmp(type,"mixed")==0)
            return new mixedGraph(fileName,thisContext);
        if (strcmp(type,"graph")==0)
            return new sparseGraph(fileName,thisContext);
        if (strcmp(type,"dense_graph")==0)
            return new denseGraph(fileName,thisContext);
        if (strcmp(type,"bigraph")==0)
            return new sparseBiGraph(fileName,thisContext);
        if (strcmp(type,"dense_bigraph")==0)
            return new denseBiGraph(fileName,thisContext);
        if (strcmp(type,"digraph")==0)
            return new sparseDiGraph(fileName,thisContext);
        if (strcmp(type,"dense_digraph")==0)
            return new denseDiGraph(fileName,thisContext);
        if (strcmp(type,"balanced_fnw")==0)
            return new balancedFNW(fileName,thisContext);
        if (strcmp(type,"mixed_integer")==0)
        {
            mipFactory* theMipFactory =
                reinterpret_cast<mipFactory*>(
                    const_cast<char*>(goblinController::pMipFactory));
            return theMipFactory->ReadInstance(fileName,thisContext);
        }

        return NULL;
    }
    catch (...) {}

    return NULL;
}
