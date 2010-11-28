//priject includes
#include "AutoCleanHashMap.h"

//cppunit includes
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

using namespace jw;

class TestAutoCleanHashMap: public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( TestAutoCleanHashMap );
		/*CPPUNIT_TEST( testFunctionName );*/
		CPPUNIT_TEST( testCollision );
		CPPUNIT_TEST( testForwardIteration );
		CPPUNIT_TEST( testBackwardIteration );
		CPPUNIT_TEST( testFreeSpace );
		CPPUNIT_TEST( testAutoFreeSpace );
		CPPUNIT_TEST( testAutoFree15000 );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
		/*Called befor each test function*/
	}

	void tearDown()
	{
		/*Called after each test function*/
	}

public:
	/*Test functions go here*/
	/*void testFunctionName() {...}*/
	void testCollision()
	{
		AutoCleanHashMap<int> hm(10);
		hm.init();
		hm.put(6, -6);
		hm.put(7, -7);
		hm.put(8, -8);
		hm.put(9, -9);
		hm.put(10, -10);
		hm.put(19, -19); //collision
		CPPUNIT_ASSERT_EQUAL(6, hm.size());
		CPPUNIT_ASSERT_EQUAL(-6, *(hm.get(6)));
		CPPUNIT_ASSERT_EQUAL(-9, *(hm.get(9)));
		CPPUNIT_ASSERT_EQUAL(-10, *(hm.get(10)));
		CPPUNIT_ASSERT_EQUAL(-19, *(hm.get(19)));
	}

	void testForwardIteration()
	{
		AutoCleanHashMap<int> hm(10);
		hm.init();
		hm.put(6, -6);
		hm.put(7, -7);
		hm.put(8, -8);
		hm.put(9, -9);
		hm.put(10, -10);
		hm.put(19, -19); //collision
		CPPUNIT_ASSERT_EQUAL(6, hm.size());
		AutoCleanHashMap<int>::Iterator iter = hm.begin();
		CPPUNIT_ASSERT_EQUAL(-19, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-10, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-9, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-8, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-7, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-6, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-19, *iter);
		iter += 7;
		CPPUNIT_ASSERT_EQUAL(-10, *(iter));
	}

	void testBackwardIteration()
	{
		AutoCleanHashMap<int> hm(10);
		hm.init();
		hm.put(6, -6);
		hm.put(7, -7);
		hm.put(8, -8);
		hm.put(9, -9);
		hm.put(10, -10);
		hm.put(19, -19); //collision
		hm.put(10, -11); //existing
		CPPUNIT_ASSERT_EQUAL(6, hm.size());
		AutoCleanHashMap<int>::Iterator iter = hm.begin();
		CPPUNIT_ASSERT_EQUAL(-11, *(iter--));
		CPPUNIT_ASSERT_EQUAL(-6, *(iter--));
		CPPUNIT_ASSERT_EQUAL(-7, *(iter));
		iter -= 3;
		CPPUNIT_ASSERT_EQUAL(-19, *(iter));
	}

	void testFreeSpace()
	{
		AutoCleanHashMap<int> hm(10);
		hm.init();
		hm.put(6, -6);
		hm.put(7, -7);
		hm.put(8, -8);
		hm.put(9, -9);
		hm.put(10, -10);
		hm.put(19, -19); //collision
		hm.put(10, -11); //existing
		CPPUNIT_ASSERT_EQUAL(6, hm.size());
		hm.deleteLast(3);
		CPPUNIT_ASSERT_EQUAL(3, hm.size());
		AutoCleanHashMap<int>::Iterator iter = hm.begin();
		CPPUNIT_ASSERT_EQUAL(-11, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-19, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-9, *(iter++));
		CPPUNIT_ASSERT_EQUAL(-11, *(iter++));
	}

	void testAutoFreeSpace()
	{
		AutoCleanHashMap<int> hm(10, 0.79, 0.3);
		hm.init();
		hm.put(6, -6);
		hm.put(7, -7);
		hm.put(8, -8);
		hm.put(9, -9);
		hm.put(10, -10);
		hm.put(19, -19); //collision
		hm.put(11, -11);
		hm.put(12, -12);
		CPPUNIT_ASSERT_EQUAL(8, hm.size());
		CPPUNIT_ASSERT_EQUAL(-7, *(hm.get(7))); //use 7
		hm.put(13, -13); //will cause deletion of 3 elements - 6,8,9
		CPPUNIT_ASSERT_EQUAL(6, hm.size());
		CPPUNIT_ASSERT_EQUAL((int*)NULL, hm.get(6));
		CPPUNIT_ASSERT_EQUAL((int*)NULL, hm.get(8));
		CPPUNIT_ASSERT_EQUAL((int*)NULL, hm.get(9));

	}

	void testAutoFree15000()
	{
		AutoCleanHashMap<int> hm(150013);
		hm.init();
#define BORDER 120011
		for (int i = 0; i <= BORDER; i++)
		{
			hm.put(i, -i);
		}
		CPPUNIT_ASSERT(hm.get(BORDER));
		CPPUNIT_ASSERT(hm.get(BORDER - 1));
		CPPUNIT_ASSERT(hm.get(BORDER - 2));
		CPPUNIT_ASSERT(hm.get(BORDER - 3));
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION( TestAutoCleanHashMap );

int main()
{
	TestAutoCleanHashMap test;
	test.testAutoFree15000();
	CPPUNIT_NS::TestResult testResult;
	CPPUNIT_NS::TestResultCollector testsCollector;
	testResult.addListener(&testsCollector);
	CPPUNIT_NS::BriefTestProgressListener progress;
	testResult.addListener(&progress);
	CPPUNIT_NS::TestRunner runner;
	runner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	runner.run(testResult);
	CPPUNIT_NS::CompilerOutputter outCompiler(&testsCollector, std::cerr);
	outCompiler.write();

	return (testsCollector.wasSuccessful() ? 0 : 1);
}
