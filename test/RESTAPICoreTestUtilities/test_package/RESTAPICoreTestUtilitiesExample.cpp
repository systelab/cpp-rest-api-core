#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1

// STL
#include <iostream>
#include <memory>

// GTEST
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "RESTAPICoreTestUtilities/Mocks/Endpoint/MockEndpoint.h"


int main(int argc, char *argv[])
{
	systelab::rest_api_core::test_utility::MockEndpoint endpoint;
    std::cout << "REST API Core test utilities work as expected" << std::endl;

    return 0;
}
