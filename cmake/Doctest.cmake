if(ENABLE_DOCTESTS)
    add_definitions(-DENABLE_DOCTEST_IN_LIBRARY)
    include(FetchContent)
    FetchContent_Declare(
            DocTest
            GIT_REPOSITORY "https://github.com/onqtam/doctest"
            GIT_TAG "d345fcf9ce4e9ed84cfbca2f3997fe832d6ddab0"
    )

    FetchContent_MakeAvailable(DocTest)
    include_directories(${DOCTEST_INCLUDE_DIR})
endif()