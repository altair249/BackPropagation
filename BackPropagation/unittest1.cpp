#include "stdafx.h"
#include "CppUnitTest.h"
#include "main.cpp"
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			neural Net;
			Assert::AreEqual(0., Net.fa(0), 0.001);
			Assert::AreEqual(0.5, Net.dfa(0), 0.001);
		}

	};
}
