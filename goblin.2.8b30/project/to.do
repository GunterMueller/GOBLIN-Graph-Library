

-----------------------------------------------------------------------------
                          Scheduled for Release 2.8
-----------------------------------------------------------------------------

Bug              : Arc insertion without bends but with label anchor point => deferred upload
Scope            : GUI / Editor

Speziallogik ShortestPath() und Indexmenge für Rückwärtssuche (t definiert, s==NoNode)
series parallel
ear decomposition
bipolar orientation

Milestone        : OriginalArc() and OriginalNode() implementations
Scope            : Library



Milestone        : (File) support for problem comments
Scope            : Documentation
Estimated Effort : 1 week
Dependencies     : None

Milestone        : Support for 3-dimensional layouts
Scope            : Library / Data structures
Estimated Effort : 3 weeks
Dependencies     : None

Milestone        : Cleanup of the solver interfaces
Scope            : Library / Interfaces
Estimated Effort : 1 weeks
Dependencies     : None
Comments         : EdgeConnectivity() and all NP-hard problem solvers


-----------------------------------------------------------------------------
                            Unscheduled Projects
-----------------------------------------------------------------------------


Milestone        : Make file import filter extensible and prepare abstraction
                 : layer for binary format
Scope            : Interfaces
Estimated Effort : 2 weeks
Dependencies     : None

Milestone        : Move import / export filters from tcl to C++ code
Scope            : Interfaces
Estimated Effort : 2 weeks
Dependencies     : None

Milestone        : Radial drawings
Scope            : Layout
Estimated Effort : 4 weeks
Dependencies     : None

Bugfix    : Mismatches in calls of overwritten new() and delete()
          : on some platforms
Scope     : Library / GUI

Bugfix    : Wrong selection order in Fibonacci Heap
Scope     : Library

Bugfix    : InfFloat must be symbolic in external formats
          : Value must be increased to avoid overruns
Scope     : Library /Shell

Milestone        : Some more index sets
Scope            : Data structures
Estimated Effort : 2 weeks
Comments         : (1) Complementary indices
                 : (2) Incidence lists as index sets
                 : (3) Implicit generation of lists of positive indices
                 : (4) Pairs
                 : (5) Containers constructed by index sets

Milestone        : GLPK plugin
Scope            : Interfaces
Estimated Effort : 2 weeks
Dependencies     : None
Comments         : (1) Display of tableau information
                 : (2) Make sparse column definitions abstract. Apply to
                 :     MPS loader and to min-cost flow.
                 : (3) Extend methLP to interior point method (GUI also)

Milestone        : Revision of the DMP planarity code
Scope            : Library
Estimated Effort : 1 weeks
Dependencies     : None
Comments         : Segments should require O(m) storage

Milestone        : Browsing facility for duality gaps
Scope            : GUI
Estimated Effort : 2 weeks
Dependencies     : None

Milestone        : Wallpapers
Scope            : GUI
Estimated Effort : 2 weeks
Dependencies     : None
Comments         : (1) Dialog for file selection
                 : (2) Relative path mechanism
                 : (3) Export to xFig format
                 : (4) Editor sometimes draws object behind wallpaper

Milestone        : Composition of linear programs
Scope            : Handling
Estimated Effort : 3-4 weeks
Dependencies     : None
Comments         : (1) Polar / Recession cones
                 : (2) Compose line graph from LP
                 : (3) Deletion of rows/variables
                 : (4) Map basis with all LP transformations 

Milestone        : Tracing interaction
Scope            : GUI, Redesign
Estimated Effort : 2 weeks
Dependencies     : None
Comments         : 

Milestone        : Gomory Cutting Planes
Scope            : Algorithms
Estimated Effort : ???
Dependencies     : None
Comments         : General separation scheme

Milestone        : Export of logfile
Scope            : GUI
Estimated Effort : 1 week
Dependencies     : None
Comments         : Export logfile from GOBLET messenger without header info

Milestone        : Documentation of file formats
Scope            : Documentation
Estimated Effort : 3 weeks
Dependencies     : None
Comments         : Describe all supported file formats

Milestone        : MKM method for maximum flows
Scope            : Algorithms
Estimated Effort : 2 weeks
Dependencies     : None
Comments         : Reuse Dinic code

Milestone        : Pruefer Codes
Scope            : Algorithms
Estimated Effort : 1 week
Dependencies     : None

Milestone        : Revision of graph data structures
Scope            : Redesign
Estimated Effort : 1 month
Dependencies     : Index data structure, change of file format -> Release 3
Comments         : Explicit arc colours and free labellings

Milestone        : Editing of colour tables
Scope            : GUI
Estimated Effort : 1-2 weeks
Dependencies     : None
Comments         : 

Milestone        : Improved starting heuristics for NP-hard problems
Scope            : Algorithms
Estimated Effort : 6 weeks
Dependencies     : Requires literature
Comments         : Especially for max-cut and independent set

Milestone        : Branch and cut scheme
Scope            : Algorithms
Estimated Effort : 6-8 weeks
Dependencies     : Dakin, Gomory
Comments         : Sophisticated branch & cut including restriction pools

Milestone        : Problems with linear side constraints
Scope            : Redesign
Estimated Effort : 3-4 weeks
Dependencies     : None
Comments         : General subgradient optimization scheme

Milestone        : Minimum cut networks
Scope            : Algorithms
Estimated Effort : 3 weeks
Dependencies     : None
Comments         : Compose minimum cut network from dense undirected graph

Milestone        : Post-Optimization for matching code
Scope            : Algorithms, redesign
Estimated Effort : ???
Dependencies     : None
Comments         : Optimization on sparse graphs

Milestone        : Separation of subtours
Scope            : Algorithms
Estimated Effort : 1-2 weeks
Dependencies     : Branch and cut scheme
Comments         : Reuse min-cut code

Milestone        : General local search scheme
Scope            : Algorithms, redesign
Estimated Effort : ???
Dependencies     : None
Comments         : Simulated annealing?

Milestone        : Steiner tree branch and bound
Scope            : Algorithms
Estimated Effort : 3 weeks
Dependencies     : None

Milestone        : Revised graph composition
Scope            : Library, graph handling
Estimated Effort : 1-2 weeks
Dependencies     : None
Comments         : (1) Backtransformation of nodes and arcs
                   (2) Simple graphs choose the arc with minimum cost

Milestone        : Redesign of trace point filter
Scope            : Tracing
Estimated Effort : 1 week
Dependencies     : None
Comments         : Restrict trace points by a maximum module nesting level

Milestone        : Let First(v) point to an exterior arc rather than its
                 : left predecessor
Scope            : Library / Planarity
Estimated Effort : 1 week
Dependencies     : None


-----------------------------------------------------------------------------
                            General redesign goals
-----------------------------------------------------------------------------

(1) Mark more methods as private
(2) Avoid global symbols
(3) Replace virtual inheritance by parent class initializations
    Then, replace dynamic_cast<>() by static_cast<>()
(4) Doxygen comments
(5) const qualifiers
(6) Mappings to original graph for derived graph objects
(7) Avoid static variables
(8) Solver main procedures can be called with a preselected method option
(9) Replace BackCast() by static_cast<>()
(10) Make more algorithms interruptable
(11) Represent native ILPs with attribute pools
(12) When writing graphs, apply the super-pool listOfGraphPars
(13) Make file import more robust
(14) No inline virtual methods



-----------------------------------------------------------------------------
                            
-----------------------------------------------------------------------------

Milestone        : 
Scope            : 
Estimated Effort : 
Dependencies     : 
Comments         : 
