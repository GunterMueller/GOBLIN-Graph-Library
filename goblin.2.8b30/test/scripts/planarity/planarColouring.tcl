
# Unit test for the node colouring solver

set testTargetFilters \
    [list [file join planar deg3 "*.gob"] \
          [file join planar deg4 "*.gob"] \
          [file join planar deg_high "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Clique construction"   TestCliques \
    "Colour nodes"          TestNodeColouring \
}

proc TestCliques {testObject testOutput} {
    return [$testObject maximumClique]
}

proc TestNodeColouring {testObject testOutput} {
    return [$testObject nodeColouring]
}
