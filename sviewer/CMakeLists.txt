add_executable(sviewer expr.cpp utils.cpp opts.cpp sviewer.cpp)
target_link_libraries(sviewer PUBLIC ExprTk viewer-core)
