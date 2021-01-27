
# Unit test for the wieghted matching solver when applied to geometric instances

set testTargetFilters \
    [list [file join tsplib smallGeo "*.tsp"] ]

set testTargetFormat "tsp"

set listOfTestSteps {
    "#Nodes"                GetNNodes       \
    "Geometric 1-factor"    Test1Factor     \
    "Geometric 2-factor"    Test2Factor     \
    "Export to GIF"         TestGifExport   \
    "Extract 2-factor"      Extract2Factor  \
}

proc GetNNodes {testObject testOutput} {
    return [$testObject #nodes]
}

proc Test1Factor {testObject testOutput} {
    global goblinMethCandidates goblinMethMCF goblinMethMCFST goblinMethFailSave

    set goblinMethFailSave 1
    set goblinMethCandidates -1
    set goblinMethMCF 5
    set goblinMethMCFST 2

    if {[expr [$testObject #nodes]%2]==1} {
        return 0
    }

    $testObject configure -nodeDemand 1
    $testObject minimumCostMatching
    $testObject extract matching

    return [$testObject info -subgraphWeight]
}

proc Test2Factor {testObject testOutput} {
    $testObject configure -nodeDemand 2
    $testObject minimumCostMatching

    return [$testObject info -subgraphWeight]
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    $testObject layout configure \
        -arcColourMode  4  -arcVisibilityMode  3  -arcStippleMode   6  \
        -nodeSize     100  -nodeShapeMode      0  -arcLabelFormat  ""

    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}

proc Extract2Factor {testObject testOutput} {
    $testObject extract cycles

    return 0
}
