#include <boost/test/unit_test.hpp>
#include <satcomms_kvh/Dummy.hpp>

using namespace satcomms_kvh;

BOOST_AUTO_TEST_CASE(it_should_not_crash_when_welcome_is_called)
{
    satcomms_kvh::DummyClass dummy;
    dummy.welcome();
}
