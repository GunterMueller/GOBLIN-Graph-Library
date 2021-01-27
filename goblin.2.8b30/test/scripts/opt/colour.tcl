
# Unit test for the node colouring and clique construction functionality

set testTargetFilters \
    [list [file join dimacs colour "miles*.col"] \
          [file join dimacs colour "anna.col"] \
          [file join dimacs colour "jean.col"] ]

set testTargetFormat "edge"

set listOfTestSteps {
    "#Nodes"                GetNNodes \
    "#Arcs"                 GetNArcs \
    "Clique construction"   TestCliques \
    "Colour nodes"          TestNodeColouring \
}

proc GetNNodes {testObject testOutput} {
    return [$testObject #nodes]
}

proc GetNArcs {testObject testOutput} {
    return [$testObject #edges]
}

proc TestCliques {testObject testOutput} {
    return [$testObject maximumClique]
}

proc TestNodeColouring {testObject testOutput} {
    return [$testObject nodeColouring]
}
