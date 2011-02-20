//priject includes
#include "GoogleTilesLoader.h"

//cppunit includes
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/BriefTestProgressListener.h>

//using namespace jw;

class GenericTest: public CPPUNIT_NS::TestFixture
{
CPPUNIT_TEST_SUITE( GenericTest );
		/*CPPUNIT_TEST( testFunctionName );*/
		//CPPUNIT_TEST( loadTile );
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
	void loadTile()
	{
		GoogleTilesLoader tileLoader;
		CImg<pixelFormat>* result = new CImg<pixelFormat>(256 * 4, 256 * 4, 1, 3);

		for (int x = 0; x < 4; x++) {
			for (int y = 0; y < 4; y++) {
				CImg<pixelFormat>* img = tileLoader.loadTile(x, y, 2);
				result->draw_image(x * 256, y * 256, *img);
				delete img;
			}
		}

/*
		CImgDisplay main_disp(*img, "View");
		while (!main_disp.is_closed())
		{
			main_disp.wait();
		}
*/
		result->save_jpeg("out.jpg", 100);
		delete result;

	}
};
CPPUNIT_TEST_SUITE_REGISTRATION( GenericTest );

int main()
{
	GenericTest test;
	test.loadTile();
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
