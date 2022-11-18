add_executable(
        archiver_tests
        ../src/BitsReader.h
        ../src/BitsReader.cpp
        ../src/BinaryHeap.h
        ../src/BitsWriter.h
        ../src/BitsWriter.cpp
        ../src/HaffmanAlgorithm.h
        ../src/HaffmanAlgorithm.cpp
        ../src/Archiver.h
        ../src/Archiver.cpp
        main.cpp
        test_binary_heap.cpp
        test_bit_input.cpp
        test_bit_output.cpp
        test_haffman_algorithm.cpp
)

target_link_libraries(archiver_tests GTest::GTest GTest::Main)

add_test(ArchiverTests archiver_tests)