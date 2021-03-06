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
	CPPUNIT_TEST( testLeadVertex );
	CPPUNIT_TEST( testCheckPole );
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

	void testIsUpsideDepth0()
	{
		CPPUNIT_ASSERT( JWTriangle::isUpside(0b000, 0) );
	}

	void testIsUpsideDepth9()
	{
		//100 is the start, with 4 appearances of 11 triangles
		CPPUNIT_ASSERT( !JWTriangle::isUpside(0b00111001111110101100100, 9) );
	}

	void testIsUpsideDepth9Upside()
	{
		//010 is the start, with 4 appearances of 11 triangles
		CPPUNIT_ASSERT( JWTriangle::isUpside(0b00111001111110101100010, 9) );
	}
	void testLeadVertex()
	{
		//010 is the start, with 4 appearances of 11 triangles
		CPPUNIT_ASSERT_EQUAL(5, JWTriangle::getLeadVertex(0b1111001, 2) );
	}

	void testCheckPole() {
		//north pole
		CPPUNIT_ASSERT_EQUAL(1,  JWTriangle::checkPole(0b0101010101011, 5));
		//south pole
		CPPUNIT_ASSERT_EQUAL(-1, JWTriangle::checkPole(0b000000000000111, 6));
		CPPUNIT_ASSERT_EQUAL(-1, JWTriangle::checkPole(0b1010101010110, 5));

		//around the equator
		CPPUNIT_ASSERT_EQUAL(0,  JWTriangle::checkPole(0b000000000000110, 5));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION( TestJWTriangle );

int main()
{
	//TestJWTriangle test;
	//test.testLeadVertex();

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
