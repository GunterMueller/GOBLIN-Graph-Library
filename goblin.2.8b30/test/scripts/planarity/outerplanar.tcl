
# Unit test for the recognition, embedding and drawing of outerplanar graphs

set testTargetFilters \
    [list [file join planar outerplanar "*.gob"] ]

set testTargetFormat "gob"

set listOfTestSteps {
    "Outerplanar drawing"       TestOuterplanarity      \
    "Export to GIF"             TestGifExport           \
    "Extract embedding"         ExtractEmbedding        \
    "Dual graph construction"   TestDuality             \
}

proc TestOuterplanarity {testObject testOutput} {
    $testObject reorder incidences -outerplanar
    $testObject layout circular -outerplanar

    return 0
}

proc TestGifExport {testObject testOutput} {
    global goblinXZoom goblinYZoom

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

    for {set a 0} {$a<[expr 2*[$testObject #edges]]} {incr a} {
        if {[$testObject arc $a info -righthandArc]!=$right($a)} {
            return 1
        }
    }

    return 0
}

proc TestDuality {testObject testOutput} {
    set state [catch {
        $testObject dualgraph DEP_OBJECT
        DEP_OBJECT delete
    }]

    return $state
}
