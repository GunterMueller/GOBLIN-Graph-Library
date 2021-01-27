
# Unit test for the st-ordering and dag search functionality

set testTargetFilters \
    [list [file join planar deg3 "*.gob"] \
          [file join planar deg4 "*.gob"] \
          [file join planar deg_high "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Biconnectivity test"   TestBiconnectivity  \
    "Construct DAG"         TestDAGConstruction \
}

proc TestBiconnectivity {testObject testOutput} {
    $testObject bipolarOrientation

    return 1
}

proc TestDAGConstruction {testObject testOutput} {
    $testObject inducedOrientation DEP_OBJECT

    if {[DEP_OBJECT topSort] != "*"} {
        DEP_OBJECT delete
        return 0
    }

    DEP_OBJECT delete
    return 1
}
