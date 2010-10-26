//priject includes
#include "JWTriangle.h"

//cppunit includes
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

class TestJWTriangle : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestJWTriangle );
	/*CPPUNIT_TEST( testFunctionName );*/
	CPPUNIT_TEST( testIsUpsideDepth0 );
	CPPUNIT_TEST( testIsUpsideDepth9 );
	CPPUNIT_TEST( testIsUpsideDepth9Upside );
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

protected:
	/*Test functions go here*/
	/*void testFunctionName() {...}*/

	void testIsUpsideDepth0()
	{
		JWTriangle triangle(0b000, 0b001, 0b100, 0b010);

		CPPUNIT_ASSERT( triangle.isUpside() );
	}

	void testIsUpsideDepth9()
	{
		//100 is the start, with 4 appearances of 11 triangles
		JWTriangle triangle(0b00111001111110101100100, 0b001, 0b100, 0b010);
		triangle.setIteration(9);
		CPPUNIT_ASSERT( !triangle.isUpside() );
	}

	void testIsUpsideDepth9Upside()
	{
		//010 is the start, with 4 appearances of 11 triangles
		JWTriangle triangle(0b00111001111110101100010, 0b001, 0b100, 0b010);
		triangle.setIteration(9);
		CPPUNIT_ASSERT( triangle.isUpside() );
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestJWTriangle );

int main()
{
	CPPUNIT_NS::TestResult testResult;
	CPPUNIT_NS::TestResultCollector testsCollector;
	testResult.addListener( &testsCollector );
	CPPUNIT_NS::BriefTestProgressListener progress;
	testResult.addListener( &progress );

	CPPUNIT_NS::TestRunner runner;
	runner.addTest( CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest() );
	runner.run( testResult );

	CPPUNIT_NS::CompilerOutputter outCompiler( &testsCollector, std::cerr );
	outCompiler.write();

	return ( testsCollector.wasSuccessful() ? 0 : 1 );
}
