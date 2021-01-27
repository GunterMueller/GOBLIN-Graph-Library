
set inputBaseDir "input"
set scriptBaseDir "scripts"

set listOfTestUnitsSpecial [list \
    [file join draw fourOrthogonal.tcl] \
]

set listOfTestUnitsFail [list \
    [file join planarity notPlanar.tcl] \
]

# Optimization tests
set listOfTestUnitsOpt [list \
    [file join opt connectivity.tcl] \
    [file join opt tspGeoExhaustive.tcl] \
    [file join opt cardinalityMatching.tcl] \
    [file join opt edgeCover.tcl] \
    [file join opt matchingGeo.tcl] \
    [file join opt colour.tcl] \
    [file join opt dag.tcl] \
    [file join opt networkSimplex.tcl] \
]

# Layout tests
set listOfTestUnitsDraw [list \
    [file join planarity outerplanar.tcl] \
    [file join planarity seriesParallel.tcl] \
    [file join planarity convexDrawing.tcl] \
    [file join planarity restrictedFDP.tcl] \
    [file join planarity staircaseDrawing.tcl] \
    [file join draw interactiveKandinsky.tcl] \
    [file join planarity notOuterplanar.tcl] \
    [file join planarity planarColouring.tcl] \
    [file join planarity visibilityRepresentation.tcl] \
    [file join planarity visibilityToGiotto.tcl] \
    [file join planarity notSeriesParallel.tcl] \
    [file join planarity fourPlanar.tcl] \
    [file join planarity visibilityDeg3.tcl] \
    [file join draw fourOrthogonal.tcl] \
    [file join planarity notPlanar.tcl] \
]


set listOfTestUnitsComplete [concat $listOfTestUnitsOpt $listOfTestUnitsDraw]

set listOfTestUnits $listOfTestUnitsFail
