//priject includes
#include "JWSphere.h"

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

class TestJWSphere: public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( TestJWSphere );
		/*CPPUNIT_TEST( testFunctionName );*/
		CPPUNIT_TEST( testOctahedronGetTriangleVertex );
		CPPUNIT_TEST( testLvl1GetTriangleVertex );
		CPPUNIT_TEST( testGetTriangle );
		CPPUNIT_TEST( testGetTriangle1 );
		CPPUNIT_TEST( testGetNeighborTriangle );
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

	/*Test functions go here*/
	/*void testFunctionName() {...}*/
	void testOctahedronGetTriangleVertex()
	{
		JWSphere shpere;
		//vertexes of triangle 0b000
		CPPUNIT_ASSERT_EQUAL((u32)0b000, shpere.getTriangleVertex(0b000, 0, 0));
		CPPUNIT_ASSERT_EQUAL((u32)0b001, shpere.getTriangleVertex(0b000, 0, 1));
		CPPUNIT_ASSERT_EQUAL((u32)0b010, shpere.getTriangleVertex(0b000, 0, 2));

		//vertexes of triangle 0b001
		CPPUNIT_ASSERT_EQUAL((u32)0b001, shpere.getTriangleVertex(0b001, 0, 0));
		CPPUNIT_ASSERT_EQUAL((u32)0b000, shpere.getTriangleVertex(0b001, 0, 1));
		CPPUNIT_ASSERT_EQUAL((u32)0b011, shpere.getTriangleVertex(0b001, 0, 2));

		//vertexes of triangle 0b010
		CPPUNIT_ASSERT_EQUAL((u32)0b010, shpere.getTriangleVertex(0b010, 0, 0));
		CPPUNIT_ASSERT_EQUAL((u32)0b110, shpere.getTriangleVertex(0b010, 0, 1));
		CPPUNIT_ASSERT_EQUAL((u32)0b000, shpere.getTriangleVertex(0b010, 0, 2));

		//vertexes of triangle 0b011
		CPPUNIT_ASSERT_EQUAL((u32)0b011, shpere.getTriangleVertex(0b011, 0, 0));
		CPPUNIT_ASSERT_EQUAL((u32)0b000, shpere.getTriangleVertex(0b011, 0, 1));
		CPPUNIT_ASSERT_EQUAL((u32)0b110, shpere.getTriangleVertex(0b011, 0, 2));

		//TODO the 3 triangles from the souther hemisphere

		//vertexes of triangle 0b111
		CPPUNIT_ASSERT_EQUAL((u32)0b111, shpere.getTriangleVertex(0b111, 0, 0));
		CPPUNIT_ASSERT_EQUAL((u32)0b011, shpere.getTriangleVertex(0b111, 0, 1));
		CPPUNIT_ASSERT_EQUAL((u32)0b110, shpere.getTriangleVertex(0b111, 0, 2));
	}

	void testLvl1GetTriangleVertex() {
		JWSphere shpere;
		CPPUNIT_ASSERT_EQUAL((u32)0b00000, shpere.getTriangleVertex(0b00000, 1, 0, true));
		CPPUNIT_ASSERT_EQUAL((u32)0b01000, shpere.getTriangleVertex(0b00000, 1, 1, true));
		CPPUNIT_ASSERT_EQUAL((u32)0b10000, shpere.getTriangleVertex(0b00000, 1, 2, true));

		CPPUNIT_ASSERT_EQUAL((u32)0b01010, shpere.getTriangleVertex(0b01110, 1, 0, true));
		CPPUNIT_ASSERT_EQUAL((u32)0b10100, shpere.getTriangleVertex(0b01110, 1, 2, true));
	}

	void testGetTriangle() {
		JWSphere shpere;
		JWTriangle * tr = shpere.getTriangle(0b00010, 1);
		CPPUNIT_ASSERT_EQUAL((u32)0b01110, tr->getNeighbour(0));
		CPPUNIT_ASSERT_EQUAL((u32)0b11010, tr->getNeighbour(1));
		CPPUNIT_ASSERT_EQUAL((u32)0b10000, tr->getNeighbour(2));
	}
	void testGetTriangle1() {
		JWSphere shpere;
		JWTriangle * tr = shpere.getTriangle(0b01000, 1);
		CPPUNIT_ASSERT_EQUAL((u32)0b00001, tr->getNeighbour(0));
		CPPUNIT_ASSERT_EQUAL((u32)0b01100, tr->getNeighbour(1));
		CPPUNIT_ASSERT_EQUAL((u32)0b11000, tr->getNeighbour(2));
	}

	void testGetNeighborTriangle() {
		JWSphere shpere;

		CPPUNIT_ASSERT_EQUAL((u32)0b1101001, shpere.getNeighborTriangle(0b1111001, 2, DIR_UP));

		CPPUNIT_ASSERT_EQUAL((u32)0b0111101, shpere.getNeighborTriangle(0b0111001, 2, DIR_DOWN));

		CPPUNIT_ASSERT_EQUAL((u32)0b00001001, shpere.getNeighborTriangle(0b00001000, 2, DIR_UP));

		CPPUNIT_ASSERT_EQUAL((u32)0b101010101, shpere.getNeighborTriangle(0b001, 3, DIR_DOWN));

	}
};
CPPUNIT_TEST_SUITE_REGISTRATION( TestJWSphere );

int main()
{
	TestJWSphere test;
	test.testGetNeighborTriangle();

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
