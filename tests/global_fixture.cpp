#include <boost/test/unit_test.hpp>
#include <boost/test/debug.hpp>

struct global_fixture {
    global_fixture()   {  
        boost::debug::detect_memory_leaks(false);
    }
    ~global_fixture()  {  }
};

BOOST_TEST_GLOBAL_FIXTURE(global_fixture);
