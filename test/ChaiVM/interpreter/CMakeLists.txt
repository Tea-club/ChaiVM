chai_test(decoder-test.cpp)
chai_test(code-manager-test.cpp)
chai_test(./executor_test.cpp)
chai_test(./simple-programs-executor-test.cpp)

target_sources(simple-programs-executor-test PRIVATE executor-test-fixture.cpp)
target_sources(executor_test PRIVATE executor-test-fixture.cpp)
