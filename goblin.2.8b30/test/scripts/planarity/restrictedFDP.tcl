
# Unit test for the planar combinatorial embedding codes

set testTargetFilters \
    [list [file join planar regular "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Disturb embedding" DisturbEmbedding \
    "Test planarity"    TestPlanarity \
    "Convex drawing"    TestConvexDrawing \
    "FDP drawing"       TestRestrictedFDP \
    "Export to GIF"     TestGifExport       \
    "Extract embedding" ExtractEmbedding \
}

proc DisturbEmbedding {testObject testOutput} {
    $testObject reorder incidences -shuffle

    return 0
}

proc TestPlanarity {testObject testOutput} {
    global goblinMethPlanarity
    set goblinMethPlanarity 1

    return [$testObject reorder incidences -planar]
}

proc TestConvexDrawing {testObject testOutput} {
    $testObject layout plane -convex -spacing 10

    return 0
}

proc TestRestrictedFDP {testObject testOutput} {
    $testObject layout fdp -preserve

    return 0
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom

    $testObject layout configure \
        -nodeShapeMode 1 -nodeColourMode 2 -arcColourMode 0
    set goblinXZoom 100
    set goblinYZoom 100

    ExportAndZip $testObject "$testOutput.gif" gif bitmap

    return 0
}

proc ExtractEmbedding {testObject testOutput} {
    for {set a 0} {$a<[expr 2*[$testObject #edges]]} {incr a} {
        set right($a) [$testObject arc $a info -righthandArc]
    }

    $testObject reorder incidences -geometric

    # Verify that the drawing code has mirrored the embedding
    set ret 0

    for {set a 0} {$a<[expr 2*[$testObject #edges]]} {incr a} {
        if {[$testObject arc $a info -righthandArc]!=$right($a)} {
            set ret 1
        }
    }

    return $ret
}
