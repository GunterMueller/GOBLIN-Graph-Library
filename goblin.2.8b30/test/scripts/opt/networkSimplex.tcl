
# Unit test for the minimum circulation and b-flow solver

set testTargetFilters \
    [list [file join dimacs netgen "stndrd1*.net"] ]

set testTargetFormat "min"

set listOfTestSteps {
    "#Nodes"            GetNNodes \
    "#Arcs"             GetNArcs \
    "Network simplex"   TestNetworkSimplex \
}

proc GetNNodes {testObject testOutput} {
    return [$testObject #nodes]
}

proc GetNArcs {testObject testOutput} {
    return [$testObject #edges]
}

proc TestNetworkSimplex {testObject testOutput} {
    global goblinMethMCF goblinMethFailSave

    set goblinMethMCF 5
    set goblinMethFailSave 1
    $testObject networkFlow

    return [$testObject info -subgraphWeight]
}
