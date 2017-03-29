
//  This file forms part of the GOBLIN C++ Class Library.
//
//  Initially written by Christian Fremuth-Paeger, June 2002
//
//  Copying, compiling, distribution and modification
//  of this source code is permitted only in accordance
//  with the GOBLIN general licence information.

/// \file   messenger.cpp
/// \brief  #goblinMessenger class implementation


#include "messenger.h"


// ------------------------------------------------------


goblinMessenger::goblinMessenger(TLog thisSize,goblinController &thisContext)
    throw() : CT(thisContext)
{
    qSize = thisSize;

    qMsg = new msgType[qSize];
    qModule = new TModule[qSize];
    qLevel = new int[qSize];
    qHandle = new THandle[qSize];
    qText = new char*[qSize];
    qNext = new TLog[qSize];
    qHidden = new bool[qSize];

    pthread_mutex_init(&msgLock,NULL);
    pthread_mutex_init(&traceLock,NULL);
    pthread_mutex_init(&traceEventLock,NULL);
    pthread_mutex_init(&solverStateLock,NULL);

    for (TLog i=0;i<qSize;i++) qNext[i] = i+1;

    firstEntry = qSize;
    lastEntry = qSize;
    currentEntry = qSize;
    firstFree = 0;

    traceEvent = false;
    cTraceFile = NULL;
    signalHalt = THREAD_STATE_IDLE;

    cachedLineNumber = 0;
    cachedFilePos = 0;

    sprintf(CT.logBuffer,"This is the GOBLIN Graph Library, Release %d.%d%s",
             CT.MajorVersion(),CT.MinorVersion(),CT.PatchLevel());
    LogEventHandler(LOG_SHELL,ModRoot,NoHandle,CT.logBuffer);
    LogEventHandler(LOG_SHELL,ModRoot,NoHandle,"");
    LogEventHandler(LOG_SHELL,ModRoot,NoHandle,
        "(C) 1998-2009  Dr.Christian Fremuth-Paeger and others");
    LogEventHandler(LOG_SHELL,ModRoot,NoHandle,"See reference manual for a complete list of contributions");
    LogEventHandler(LOG_SHELL,ModRoot,NoHandle,"");
}


goblinMessenger::~goblinMessenger() throw()
{
    pthread_mutex_lock(&msgLock);
    pthread_mutex_lock(&traceLock);
    pthread_mutex_lock(&traceEventLock);
    pthread_mutex_lock(&solverStateLock);

    TLog i = firstEntry;

    while (i!=qSize)
    {
        delete[] qText[i];
        i = qNext[i];
    }

    delete[] qMsg;
    delete[] qModule;
    delete[] qLevel;
    delete[] qHandle;
    delete[] qText;
    delete[] qNext;
    delete[] qHidden;

    pthread_mutex_destroy(&msgLock);
    pthread_mutex_destroy(&traceLock);
    pthread_mutex_destroy(&traceEventLock);
    pthread_mutex_destroy(&solverStateLock);
}


unsigned long goblinMessenger::Size() throw()
{
    unsigned long ret =
          sizeof(goblinMessenger)
        + qSize*sizeof(msgType) // qMsg[]
        + qSize*sizeof(TModule) // qModule[]
        + qSize*sizeof(THandle) // qHandle[]
        + qSize*sizeof(int)     // qLevel[]
        + qSize*sizeof(char*)   // qText[]
        + qSize*sizeof(bool)    // qHidden[]
        + qSize*sizeof(TLog);   // qNext[]

    TLog i = firstEntry;

    while (i!=qSize)
    {
        ret += sizeof(char)*(strlen(qText[i])+1);
        i = qNext[i];
    }

    // mutexes ?

    return ret;
}


void goblinMessenger::SweepBuffer() throw()
{
    pthread_mutex_lock(&msgLock);

    for (TLog i=0;i<qSize;i++) qNext[i] = i+1;

    firstEntry = qSize;
    lastEntry = qSize;
    currentEntry = qSize;
    firstFree = 0;

    pthread_mutex_unlock(&msgLock);
}


void goblinMessenger::Restart() throw()
{
    SweepBuffer();

    traceEvent = false;
    CT.breakLevel = INT_MAX;
    CT.logLevel = 0;

    if (cTraceFile)
    {
        delete[] cTraceFile;
        cTraceFile = NULL;
    }

    cachedLineNumber = 0;
    cachedFilePos = 0;
}


void goblinMessenger::LogEventHandler(msgType msg,TModule mod,THandle OH,char* logText)
    throw()
{
    CT.DefaultLogEventHandler(msg,mod,OH,logText);
    MsgAppend(msg,mod,OH,CT.logLevel,logText);
}


void goblinMessenger::MsgAppend(msgType msg,TModule mod,THandle OH,int logLevel,char* logText)
    throw()
{
    pthread_mutex_lock(&msgLock);

    if (msg==MSG_APPEND && lastEntry<qSize)
    {
        qText[lastEntry] = (char*)GoblinRealloc(qText[lastEntry],
            strlen(qText[lastEntry])+strlen(logText)+1);
        strcat(qText[lastEntry],logText);
    }
    else
    {
        TLog nextEntry = firstFree;
        if (nextEntry==qSize)
        {
            nextEntry = firstEntry;
            firstEntry = qNext[firstEntry];
            delete[] qText[nextEntry];
        }
        else firstFree = qNext[firstFree];

        if (lastEntry==qSize)
        {
            firstEntry = nextEntry;
            currentEntry = nextEntry;
        }
        else qNext[lastEntry] = nextEntry;

        lastEntry = nextEntry;
        qText[lastEntry] = new char[strlen(logText)+1];
        sprintf(qText[lastEntry],"%s",logText);
        qHandle[lastEntry] = OH;
        qLevel[lastEntry] = logLevel;
        qMsg[lastEntry] = msg;
        qModule[lastEntry] = mod;
        qHidden[lastEntry] = false;
    }

    pthread_mutex_unlock(&msgLock);
}


void goblinMessenger::MsgReset() throw()
{
    pthread_mutex_lock(&msgLock);
    currentEntry = firstEntry;
    pthread_mutex_unlock(&msgLock);
}


bool goblinMessenger::MsgEndOfBuffer() throw()
{
    pthread_mutex_lock(&msgLock);
    bool ret = (currentEntry==qSize || currentEntry==lastEntry);
    pthread_mutex_unlock(&msgLock);

    return ret;
}


bool goblinMessenger::MsgVoid() throw()
{
    pthread_mutex_lock(&msgLock);
    bool ret = (firstEntry==qSize);
    pthread_mutex_unlock(&msgLock);

    return ret;
}


void goblinMessenger::MsgSkip() throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize || qNext[currentEntry]==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgSkip","No more queued messages");
    }

    currentEntry = qNext[currentEntry];
    pthread_mutex_unlock(&msgLock);
}

size_t goblinMessenger::MsgText(char* exportBuffer,size_t bufferSize) throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgText","No more queued messages");
    }

    strncpy(exportBuffer,qText[currentEntry],bufferSize);
    size_t numChars = strlen(qText[currentEntry]);
    pthread_mutex_unlock(&msgLock);

    if (numChars<bufferSize) return numChars;

    exportBuffer[bufferSize-1] = 0;
    return bufferSize;
}


msgType goblinMessenger::MsgClass() throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgClass","No more queued messages");
    }

    pthread_mutex_unlock(&msgLock);

    return qMsg[currentEntry];
}


TModule goblinMessenger::MsgModule() throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgModule","No more queued messages");
    }

    pthread_mutex_unlock(&msgLock);

    return qModule[currentEntry];
}


int goblinMessenger::MsgLevel() throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgLevel","No more queued messages");
    }

    pthread_mutex_unlock(&msgLock);

    return qLevel[currentEntry];
}


THandle goblinMessenger::MsgHandle() throw(ERRejected)
{
    pthread_mutex_lock(&msgLock);

    if (currentEntry==qSize)
    {
        pthread_mutex_unlock(&msgLock);
        CT.Error(ERR_REJECTED,NoHandle,"MsgHandle","No more queued messages");
    }

    pthread_mutex_unlock(&msgLock);

    return qHandle[currentEntry];
}


void goblinMessenger::SignalTraceEvent(char* traceFileName) throw()
{
    pthread_mutex_lock(&traceLock);

    pthread_mutex_lock(&traceEventLock);
    traceEvent = true;
    pthread_mutex_unlock(&traceEventLock);

    delete[] cTraceFile;

    if (!traceFileName) cTraceFile = NULL;
    else
    {
        cTraceFile = new char[strlen(traceFileName)+1];
        sprintf(cTraceFile,"%s",traceFileName);
    }

    pthread_mutex_unlock(&traceLock);
}


void goblinMessenger::TraceSemTake() throw()
{
    bool pending = true;

    while (pending)
    {
        pthread_mutex_lock(&traceEventLock);
        pending = traceEvent;
        pthread_mutex_unlock(&traceEventLock);
    }
}


size_t goblinMessenger::TraceFilename(char* exportBuffer,size_t bufferSize) throw()
{
    pthread_mutex_lock(&traceLock);

    if (cTraceFile==NULL)
    {
        pthread_mutex_unlock(&traceLock);
        exportBuffer[0] = 0;
        return 0;
    }

    strncpy(exportBuffer,cTraceFile,bufferSize);
    size_t numChars = strlen(cTraceFile);
    pthread_mutex_unlock(&traceLock);

    if (numChars<bufferSize) return numChars;

    exportBuffer[bufferSize-1] = 0;
    return bufferSize;
}


bool goblinMessenger::TraceEvent() throw()
{
    pthread_mutex_lock(&traceEventLock);
    bool ret = traceEvent;
    pthread_mutex_unlock(&traceEventLock);

    return ret;
}


void goblinMessenger::TraceUnblock() throw()
{
    pthread_mutex_lock(&traceEventLock);
    traceEvent = false;
    pthread_mutex_unlock(&traceEventLock);
}


bool goblinMessenger::SolverIdle() throw()
{
    pthread_mutex_lock(&solverStateLock);
    bool ret = (signalHalt==THREAD_STATE_IDLE);
    pthread_mutex_unlock(&solverStateLock);

    return ret;
}


bool goblinMessenger::SolverRunning() throw()
{
    pthread_mutex_lock(&solverStateLock);
    bool ret = (signalHalt==THREAD_STATE_RUNNING);
    pthread_mutex_unlock(&solverStateLock);

    return ret;
}


void goblinMessenger::SolverSignalPending() throw()
{
    pthread_mutex_lock(&solverStateLock);
    signalHalt = THREAD_STATE_PENDING;
    pthread_mutex_unlock(&solverStateLock);
}


void goblinMessenger::SolverSignalStarted() throw()
{
    pthread_mutex_lock(&solverStateLock);
    signalHalt = THREAD_STATE_RUNNING;
    pthread_mutex_unlock(&solverStateLock);
}


void goblinMessenger::SolverSignalStop() throw()
{
    pthread_mutex_lock(&solverStateLock);
    if (signalHalt!=THREAD_STATE_IDLE) signalHalt = THREAD_STATE_STOPPED;
    pthread_mutex_unlock(&solverStateLock);
}


void goblinMessenger::SolverSignalIdle() throw()
{
    pthread_mutex_lock(&solverStateLock);
    signalHalt = THREAD_STATE_IDLE;
    pthread_mutex_unlock(&solverStateLock);
}


TLog goblinMessenger::GetBufferSize() const throw()
{
    return qSize;
}


unsigned long goblinMessenger::GetNumLines(const char* fileName) const throw(ERFile)
{
    FILE* fptr = fopen(fileName,"r");
    unsigned long numLines = 1;

    if (!fptr)
    {
        throw ERFile();
    }

    while (!feof(fptr))
    {
        if (fgetc(fptr)=='\n') ++numLines;
    }

    if (fclose(fptr)!=0)
    {
        throw ERFile();
    }

    return numLines;
}


static void getNormalizedLine(FILE* fptr,char* lineBuffer,size_t bufferSize)
{
    char truncStr[] = " <..>";

    for (size_t i=0;;++i)
    {
        if (i+strlen(truncStr)+1>bufferSize)
        {
            strcpy(lineBuffer+i,truncStr);

            // Consume the remainder of this line
            while (!feof(fptr) && fgetc(fptr)!='\n') {}

            return;
        }

        if (feof(fptr))
        {
            lineBuffer[i] = 0;
            return;
        }

        lineBuffer[i] = fgetc(fptr);

        if (lineBuffer[i]=='\n' || lineBuffer[i]==EOF)
        {
            lineBuffer[i] = 0;
            return;
        }
    }
}


long goblinMessenger::GetLineByNumber(
    const char* fileName,char* lineBuffer,size_t bufferSize,size_t lineNumber) const throw(ERFile)
{
    FILE* fptr = fopen(fileName,"r");

    if (!fptr)
    {
        throw ERFile();
    }

    size_t numLines = 0;
    strcpy(lineBuffer,"\n");

    while (!feof(fptr) && (numLines<lineNumber || lineNumber<0))
    {
        if (fgetc(fptr)=='\n') ++numLines;
    }

    getNormalizedLine(fptr,lineBuffer,bufferSize);

    long filePos = ftell(fptr);

    if (filePos<0)
    {
        throw ERFile();
    }

    if (fclose(fptr)!=0)
    {
        throw ERFile();
    }

    return filePos;
}


long goblinMessenger::GetLineByPos(
    const char* fileName,char* lineBuffer,size_t bufferSize,long filePos) const throw(ERFile)
{
    FILE* fptr = fopen(fileName,"r");

    if (!fptr)
    {
        throw ERFile();
    }

    if (fseek(fptr,filePos,SEEK_SET)!=0)
    {
        throw ERFile();
    }

    getNormalizedLine(fptr,lineBuffer,bufferSize);

    long nextFilePos = ftell(fptr);

    if (nextFilePos<0)
    {
        throw ERFile();
    }

    if (fclose(fptr)!=0)
    {
        throw ERFile();
    }

    return nextFilePos;
}


long goblinMessenger::SeekLinesBack(
    const char* fileName,long filePos,unsigned long numLines) const throw(ERFile)
{
    if (filePos<=0) return 0;

    FILE* fptr = fopen(fileName,"r");

    if (!fptr)
    {
        throw ERFile();
    }

    long prevFilePos = filePos;
    unsigned long countLines = 0;

    while (true)
    {
        if (fseek(fptr,prevFilePos-1,SEEK_SET)!=0)
        {
            prevFilePos = 0;
            break;
        }

        if (fgetc(fptr)=='\n') ++countLines;

        if (countLines>numLines) break;

        --prevFilePos;
    }

    if (fclose(fptr)!=0)
    {
        throw ERFile();
    }

    return prevFilePos;
}


long goblinMessenger::SeekLinesAhead(
    const char* fileName,long filePos,unsigned long numLines) const throw(ERFile)
{
    if (numLines==0) return SeekLinesBack(fileName,filePos,0);

    FILE* fptr = fopen(fileName,"r");

    if (!fptr)
    {
        throw ERFile();
    }

    long nextFilePos = filePos;
    unsigned long countLines = 0;

    while (!feof(fptr))
    {
        if (fseek(fptr,nextFilePos,SEEK_SET)!=0)
        {
            break;
        }

        ++nextFilePos;

        if (fgetc(fptr)=='\n')
        {
            ++countLines;

            if (countLines>=numLines) break;
        }
    }

    if (fclose(fptr)!=0)
    {
        throw ERFile();
    }

    return nextFilePos;
}


void goblinMessenger::LoadBuffer(const char* fileName,size_t lineNumber) throw(ERFile)
{
    SweepBuffer();

    char lineBuffer[500];
    const size_t textOffset = 21;

    // Position the file
    long filePos = 0;

    if (lineNumber>=qSize)
    {
        if (lineNumber<cachedLineNumber-2*qSize)
        {
            // Does not make sense to search from the cached position
            filePos = GetLineByNumber(fileName,lineBuffer,sizeof(lineBuffer),lineNumber-qSize);
        }
        else if (lineNumber-qSize<cachedLineNumber)
        {
            filePos = SeekLinesBack(fileName,cachedFilePos,cachedLineNumber+qSize-lineNumber);
        }
        else if (lineNumber-qSize>cachedLineNumber)
        {
            filePos = SeekLinesAhead(fileName,cachedFilePos,lineNumber-cachedLineNumber-qSize);
        }
        else // if (lineNumber-qSize==cachedLineNumber)
        {
            filePos = cachedFilePos;
        }
    }

    for (TLog i=0;i<qSize;++i)
    {
        long nextFilePos = GetLineByPos(fileName,lineBuffer,sizeof(lineBuffer),filePos);

        if (strlen(lineBuffer)>0)
        {
            // This conditional guarantees that cachedFilePos always follows on
            // a line feed and is not the file end.
            cachedLineNumber = lineNumber-qSize+i;
            cachedFilePos = filePos;
        }

        filePos = nextFilePos;

        int msg = int(LOG_SHELL);
        int mod = int(ModRoot);
        THandle OH = NoHandle;
        int logLevel = 0;
        int numItems = 0;

        if (strlen(lineBuffer)>=textOffset)
        {
           numItems = sscanf(lineBuffer,"%i %i %lu %i",&msg,&mod,&OH,&logLevel);
        }

        if (OH<0) OH = NoHandle;

        if (numItems==4 && strlen(lineBuffer)>=textOffset+2*logLevel)
        {
            char* thisText = lineBuffer+textOffset+2*logLevel;
            MsgAppend(msgType(msg),TModule(mod),OH,logLevel,thisText);
        }
        else
        {
            // MsgAppend(msgType(msg),TModule(mod),OH,logLevel,"");
        }
    }
}
