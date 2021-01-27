
# Unit test for the TSP solver

set testTargetFilters \
    [list [file join tsplib tinyGeo "*.tsp"] ]

set testTargetFormat "tsp"

set listOfTestSteps {
    "#Nodes"            GetNNodes       \
    "Heuristic TSP"     HeuristicTsp    \
    "Candidates TSP"    CandidatesTsp   \
    "Exhaustive TSP"    ExhaustiveTsp   \
    "Export to GIF"     TestGifExport   \
}

proc GetNNodes {testObject testOutput} {
    return [$testObject #nodes]
}

proc HeuristicTsp {testObject testOutput} {
    global goblinMethRelaxTSP1 goblinMethSolve goblinMethLocal

    set goblinMethRelaxTSP1 1
    set goblinMethSolve 1
    set goblinMethLocal 1
    return [$testObject hamiltonianCycle]
}

proc CandidatesTsp {testObject testOutput} {
    global goblinMethCandidates goblinMethRelaxTSP1 goblinMethRelaxTSP2 \
        goblinMethSolve goblinMethLocal goblinMaxBBIterations goblinMaxBBNodes

    set goblinMethRelaxTSP1 0
    set goblinMethRelaxTSP2 1
    set goblinMethSolve 2
    set goblinMethCandidates 0
    set goblinMaxBBIterations 5
    set goblinMaxBBNodes -1
    return [$testObject hamiltonianCycle]
}

proc ExhaustiveTsp {testObject testOutput} {
    global goblinMethCandidates goblinMaxBBIterations goblinMaxBBNodes goblinMethLocal \
        goblinLogMethods goblinLogGaps

    set goblinMethCandidates -1
    set goblinMethLocal 0
    set goblinMaxBBIterations -1
    set goblinMaxBBNodes 3
    set goblinLogMethods 2
    set goblinLogGaps 0

    return [$testObject hamiltonianCycle]
}

proc TestGifExport {testObject testOutput} {
    # This exports the optimal tour given by the predecessor attribute,
    # and the one-cycle tree provided by the subgraph labels

    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    $testObject layout configure \
        -arcColourMode    4  -arcVisibilityMode    3  -arcStippleMode  6  \
        -arrowDisplayMode 4  -nodeSize           100  -nodeColourMode  2  \
        -nodeShapeMode    0  -bendSpacing          5  -arcLabelFormat ""

    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}
