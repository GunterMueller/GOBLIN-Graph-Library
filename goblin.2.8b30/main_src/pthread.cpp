
#include <tcl.h>
#include <pthread.h>


pthread_mutex_t evalLock;

char testCmd[] =
    "for {set i 1} {$i <= 10} {incr i} {\n"
    "  puts \"  Hello world, the $i th time\"\n"
    "}\n";

const unsigned NUM_THREADS = 10;


void My_Evaluate(Tcl_Interp* interp,unsigned myID)
{
    for (unsigned i=1;i<=30;i++)
    {
//        pthread_mutex_lock(&evalLock);

//        if (Tcl_EvalFile(interp, "test.tcl")==TCL_ERROR)
//        if (Tcl_Eval(interp, "source \"test.tcl\"")==TCL_ERROR)
        if (Tcl_Eval(interp, testCmd)==TCL_ERROR)
        {
            printf("...Script exit status: %s\n",interp->result);
            break;
        }
        else
        {
            printf("...Script executed the %uth time by %u\n", i, myID);
        }

//        pthread_mutex_unlock(&evalLock);
    }
}


void* My_Thread(void*)
{
    unsigned myID = unsigned(pthread_self());

    Tcl_Interp* interp2 = Tcl_CreateInterp();
    Tcl_Preserve(interp2);

    if (Tcl_Init(interp2) == TCL_ERROR)
    {
        printf("...Tcl interpreter could not be initialized\n");
    }
    else
    {
        printf("Running thread %u...\n",myID);

        My_Evaluate(interp2,myID);
    }

    Tcl_Release(interp2);
    Tcl_DeleteInterp(interp2);

    printf("...Thread %u executed\n", myID);

    pthread_exit(NULL);

    return NULL;
}


int main(int ParamCount,const char *ParamStr[])
{
#ifdef TCL_THREADS
    printf("TCL_THREADS enabled\n");
#else
    printf("TCL_THREADS disabled\n");
#endif

    Tcl_FindExecutable(ParamStr[0]);

    Tcl_Interp* interp1 = Tcl_CreateInterp();
    Tcl_Preserve(interp1);

    pthread_mutex_init(&evalLock,NULL);

    pthread_t threadID[NUM_THREADS];

    for (unsigned i=0;i<NUM_THREADS;i++)
    {
        pthread_create(&threadID[i],NULL,My_Thread,NULL);
        printf ("...Thread %u created\n", unsigned(threadID[i]));
    }

    unsigned myID = unsigned(pthread_self());

    printf("Master thread has ID %u...\n", myID);

    My_Evaluate(interp1,myID);

    for (unsigned i=0;i<NUM_THREADS;i++)
    {
        pthread_join(threadID[i],NULL);
    }

    Tcl_Release(interp1);
    Tcl_DeleteInterp(interp1);

    printf ("...All threads completed\n");

    return 0;
}
