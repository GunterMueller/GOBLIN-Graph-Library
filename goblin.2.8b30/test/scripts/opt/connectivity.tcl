
# Unit test for both all connectivity and max-flow methods
# Visual output shows the minimum order non-trivial edge connected components

set testTargetFilters \
    [list [file join directed connectivity "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Edge Connectivity"                 TestEdgeConnectivity              \
    "Vertex Connectivity"               TestVertexConnectivity            \
    "Generalized Connectivity"          TestGeneralizedConnectivity       \
    "Edge Connected Components"         TestComponents                    \
    "Export to GIF"                     TestGifExport                     \
    "Bipolar Connectivity"              TestBipolarConnectivity           \
    "Strong Edge Connectivity"          TestStrongEdgeConnectivity        \
    "Strong Vertex Connectivity"        TestStrongVertexConnectivity      \
    "Strong Generalized Connectivity"   TestStrongGeneralizedConnectivity \
    "Strong Components"                 TestStrongComponents              \
    "Export to GIF"                     TestGifExport2                    \
    "Check Bounds"                      CheckBounds                       \
}

proc TestEdgeConnectivity {testObject testOutput} {
    global goblinMethMCC edgeConnectivityNumber

    $testObject configure -lowerBound 0 -upperBound 1

    set goblinMethMCC 3
    set edgeConnectivityNumber [$testObject connectivity -edge]

    return $edgeConnectivityNumber
}

proc TestVertexConnectivity {testObject testOutput} {
    global goblinMethMXF vertexConnectivityNumber

    set goblinMethMXF 1
    $testObject configure -nodeDemand 9999
    set vertexConnectivityNumber [$testObject connectivity -vertex]

    return $vertexConnectivityNumber
}

proc TestGeneralizedConnectivity {testObject testOutput} {
    global goblinMethMXF generalizedConnectivityNumber

    set goblinMethMXF 2
    set generalizedConnectivityNumber [$testObject connectivity]

    return $generalizedConnectivityNumber
}

proc TestComponents {testObject testOutput} {
    global goblinMethMXF edgeConnectivityNumber leftHandNode rightHandNode

    set goblinMethMXF 4
    set leftHandNode *
    set rightHandNode *
    set isConnected \
        [$testObject components -kappa [expr $edgeConnectivityNumber+1]]

    for {set v 0} {$v<[$testObject #nodes]} {incr v} {
        if {[$testObject node $v info -nodeColour]==0} {
            set leftHandNode $v
        }

        if {[$testObject node $v info -nodeColour]==1} {
            set rightHandNode $v
        }
    }

    if {$leftHandNode=="*"} {
        error "Missing left hand node"
    }

    if {$rightHandNode=="*"} {
        error "Missing right hand node"
    }

    if {$isConnected==1} {
        error "Graph is connected"
    }

    return 0
}

proc TestBipolarConnectivity {testObject testOutput} {
    global goblinMethMXF bipolarConnectivityNumber leftHandNode rightHandNode

    $testObject configure -lowerBound 0 -upperBound 1

    set goblinMethMXF 0
    set bipolarConnectivityNumber \
        [$testObject connectivity -edge -source $leftHandNode -target $rightHandNode]

    return $bipolarConnectivityNumber
}

proc TestStrongEdgeConnectivity {testObject testOutput} {
    global goblinMethMCC strongEdgeConnectivityNumber

    set goblinMethMCC 3
    set strongEdgeConnectivityNumber [$testObject connectivity -edge -strong]

    return $strongEdgeConnectivityNumber
}

proc TestStrongVertexConnectivity {testObject testOutput} {
    global goblinMethMXF strongVertexConnectivityNumber

    set goblinMethMXF 5
    set strongVertexConnectivityNumber [$testObject connectivity -vertex -strong]

    return $strongVertexConnectivityNumber
}

proc TestStrongGeneralizedConnectivity {testObject testOutput} {
    global goblinMethMXF strongGeneralizedConnectivityNumber

    set goblinMethMXF 3
    set strongGeneralizedConnectivityNumber [$testObject connectivity -strong]

    return $strongGeneralizedConnectivityNumber
}

proc CheckBounds {testObject testOutput} {
    global edgeConnectivityNumber vertexConnectivityNumber \
        generalizedConnectivityNumber bipolarConnectivityNumber \
        strongEdgeConnectivityNumber strongVertexConnectivityNumber \
        strongGeneralizedConnectivityNumber

    if {$edgeConnectivityNumber<$vertexConnectivityNumber} {
        error "Edge connectivity and vertex connectivity number mismatch"
    }

    if {$edgeConnectivityNumber!=$generalizedConnectivityNumber} {
        error "Edge connectivity and generalized connectivity number mismatch"
    }

    if {$edgeConnectivityNumber!=$bipolarConnectivityNumber} {
        error "Edge connectivity and bipolar connectivity number mismatch"
    }

    if {$strongEdgeConnectivityNumber<$strongVertexConnectivityNumber} {
        error "Strong edge connectivity and vertex connectivity number mismatch"
    }

    if {$strongEdgeConnectivityNumber!=$strongGeneralizedConnectivityNumber} {
        error "Strong edge connectivity and generalized connectivity number mismatch"
    }

    if {[expr 2*$strongEdgeConnectivityNumber]>$edgeConnectivityNumber} {
        error "Edge connectivity and strong edge connectivity number mismatch"
    }

    return 0
}

proc TestStrongComponents {testObject testOutput} {
    global goblinMethMXF strongEdgeConnectivityNumber leftHandNode rightHandNode

    set goblinMethMXF 1
    set leftHandNode *
    set rightHandNode *
    set isConnected \
        [$testObject components -strong -kappa [expr $strongEdgeConnectivityNumber+1]]

    for {set v 0} {$v<[$testObject #nodes]} {incr v} {
        if {[$testObject node $v info -nodeColour]==0} {
            set leftHandNode $v
        }

        if {[$testObject node $v info -nodeColour]==1} {
            set rightHandNode $v
        }
    }

    if {$leftHandNode=="*"} {
        error "Missing left hand node"
    }

    if {$rightHandNode=="*"} {
        error "Missing right hand node"
    }

    if {$isConnected==1} {
        error "Graph is connected"
    }

    return 0
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    $testObject layout configure \
        -arcColourMode    0  -arcLabelFormat ""  -arcVisibilityMode 4  \
        -arcStippleMode   0  -nodeSize      100  -nodeLabelFormat  ""  \
        -nodeColourMode   2  -nodeShapeMode   1  -bendSpacing       5  \
        -arrowDisplayMode 1

    ExportAndZip $testObject "$testOutput.edge.gif" gif bitmap

    return 0
}

proc TestGifExport2 {testObject testOutput} {
    global goblinXZoom goblinYZoom ZipEnabled

    set goblinXZoom 100
    set goblinYZoom 100
    set ZipEnabled 0

    $testObject layout configure \
        -arcColourMode    0  -arcLabelFormat ""  -arcVisibilityMode 4  \
        -arcStippleMode   0  -nodeSize      100  -nodeLabelFormat  ""  \
        -nodeColourMode   2  -nodeShapeMode   1  -bendSpacing       5  \
        -arrowDisplayMode 0

    ExportAndZip $testObject "$testOutput.strong.gif" gif bitmap

    return 0
}
