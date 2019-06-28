#include "stdafx.h"


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	int res = RUN_ALL_TESTS();

	return res;
}

