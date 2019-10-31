#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING  1

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>

#include "JSONSettingsTestUtilities/Mocks/MockSettingsService.h"


int main(int argc, char *argv[])
{
	systelab::setting::test_utility::MockSettingsService settingsService;
    std::cout << "JSONSettings test utilities work as expected" << std::endl;

    return 0;
}
