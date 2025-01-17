#include "RasterTest.h"
#include "AugeCore.h"
#include "AugeData.h"
#include "AugeCarto.h"
#include "AugeGeometry.h"
#include "AugeProcessor.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION(RasterTest);

void RasterTest::setUp() 
{
	auge::GLogger	*pLogger = auge::augeGetLoggerInstance();
	pLogger->Initialize();

	auge::DataEngine	*pEngine = NULL;
	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Load();

	pEngine = pEngineManager->GetEngine("Postgres");
	m_pConnection = pEngine->CreateConnection();
	m_pConnection->SetConnectionString("SERVER=127.0.0.1;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	//m_pConnection->SetConnectionString("SERVER=192.168.111.160;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	//m_pConnection->SetConnectionString("SERVER=182.92.114.80;INSTANCE=5432;DATABASE=auge;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK");
	m_pConnection->Open();

	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Initialize(m_pConnection);

	printf("setUp\n");
}

void RasterTest::tearDown()
{
	auge::ConnectionManager* pConnManager = NULL;
	pConnManager = auge::augeGetConnectionManagerInstance();
	pConnManager->Unload();

	auge::DataEngineManager* pEngineManager = NULL;
	pEngineManager = auge::augeGetDataEngineManagerInstance();
	pEngineManager->Unload();

	//m_pConnection->Close();
	//m_pConnection->Release();
	m_pConnection = NULL;
	printf("tearDown\n");
}

void RasterTest::ExtractByRectangle()
{	
	DWORD ts = GetTickCount();

	auge::RasterExtractByRectangleProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterExtractByRectangleProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.png");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_clipped.png");
	pProcessor->SetOutputPath("/s2");

	auge::GEnvelope extent(100,100,500,500);
	pProcessor->SetInputRectangle(extent);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();
	
	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Stretch()
{
	DWORD ts = GetTickCount();

	auge::RasterStretchProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterStretchProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputPath("/dem");
	pProcessor->SetInputRaster("ASTGTM2_N29E082_dem.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("ASTGTM2_N29E082_dem_stretch.jpg");
	//pProcessor->SetOutputPath("/ds1");

	//auge::GColor start_color(0,255,0,255);
	//pProcessor->SetStartColor(start_color);
	//auge::GColor end_color(255,0,0,255);
	//pProcessor->SetEndColor(end_color);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::EdgeDetorSobol()
{
	DWORD ts = GetTickCount();

	auge::RasterEdgeDetectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterEdgeDetectProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_edge.tif");

	//pProcessor->SetInputDataSource("rsdb2");
	//pProcessor->SetInputRaster("Koala.jpeg");

	//pProcessor->SetOutputDataSource("rsdb2");
	//pProcessor->SetOutputRaster("Koala_edge.jpeg");

	pProcessor->SetEdgeDetector(auge::augeEdgeSobel);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Graylize()
{
	DWORD ts = GetTickCount();

	auge::RasterGraylizeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterGraylizeProcessor();

	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("gray.jpg");
	pProcessor->SetOutputPath("/ds1");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::ToJPG()
{
	DWORD ts = GetTickCount();

	auge::RasterFormatConvertToJPEGProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterFormatConvertToJPEGProcessor();

	//pProcessor->SetInputDataSource("rsdb2");
	//pProcessor->SetInputRaster("srtm_58_05.tif");

	//pProcessor->SetOutputDataSource("rsdb2");
	//pProcessor->SetOutputRaster("srtm_58_05.jpeg");

	auge::GEnvelope extent;
	extent.Set(100,100,800,800);

	pProcessor->SetInputRectangle(extent);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.png");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_xxx.jpg");
	//pProcessor->SetInputPath("/s1");

	pProcessor->SetRed(1);
	pProcessor->SetGreen(0);
	pProcessor->SetBlue(0);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Reverse()
{
	DWORD ts = GetTickCount();

	auge::RasterReverseProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterReverseProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_reverse.jpg");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Smooth()
{
	DWORD ts = GetTickCount();

	auge::RasterSmoothProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSmoothProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/s1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_mean.jpg");
	pProcessor->SetOutputPath("/s2/s_1_2");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	//pProcessor->SetSmoother(auge::augeSmoothMean);
	//pProcessor->SetSmoother(auge::augeSmoothGauss);
	pProcessor->SetSmoother(auge::augeSmoothMedian);

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Subtract()
{
	DWORD ts = GetTickCount();

	auge::RasterSubtractProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSubtractProcessor();

	pProcessor->SetUser(100032);
	pProcessor->SetInputDataSource_1("rsdb");
	pProcessor->SetInputRaster_1("ASTGTM2_N34E110_dem.jpg");
	pProcessor->SetInputPath_1("/");

	pProcessor->SetInputDataSource_2("rsdb");
	pProcessor->SetInputRaster_2("62.jpg");
	pProcessor->SetInputPath_2("/r");

	pProcessor->SetOutputDataSource("rsdb");
	pProcessor->SetOutputRaster("73_126.jpg");
	pProcessor->SetOutputPath("/r");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::PixelBlend()
{
	DWORD ts = GetTickCount();

	auge::RasterPixelBlendProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterPixelBlendProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource_1("rsdb2");
	pProcessor->SetInputRaster_1("Desert.jpg");
	pProcessor->SetInputPath_1("/ds1");

	pProcessor->SetInputDataSource_2("rsdb2");
	pProcessor->SetInputRaster_2("Koala.jpg");
	pProcessor->SetInputPath_2("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_Koala_5.jpg");
	pProcessor->SetOutputPath("/ds1");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::HistgramEqualization()
{
	DWORD ts = GetTickCount();

	auge::RasterHistogramEqualizationProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterHistogramEqualizationProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("gray.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("gray_histeql.jpg");
	pProcessor->SetOutputPath("/ds1");
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Threshold()
{
	DWORD ts = GetTickCount();

	auge::RasterThresholdProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterThresholdProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Desert.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Desert_threshold_average.jpg");
	pProcessor->SetOutputPath("/ds1");
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::SepiaTone()
{
	DWORD ts = GetTickCount();

	auge::RasterSepiaToneEffectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterSepiaToneEffectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_Sepia_2.jpg");
	pProcessor->SetOutputPath("/ds1");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::PenEffect()
{
	DWORD ts = GetTickCount();

	auge::RasterPenEffectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterPenEffectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("Koala.jpg");
	pProcessor->SetInputPath("/ds1");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("Koala_pen.jpg");
	pProcessor->SetOutputPath("/ds1");
	//pProcessor->SetOutputRaster("Desert_gauss.jpg");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMSlope()
{
	DWORD ts = GetTickCount();

	auge::DemSlopeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemSlopeProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_slope_3.tif");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMSlope_2()
{
	DWORD ts = GetTickCount();

	auge::DemSlopeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemSlopeProcessor();

	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster= NULL;

	pinRaster = pRasterIO->Read("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/ASTGTM2_N29E082_dem.tif");

	poutRaster = pProcessor->Slope(pinRaster);
	poutRaster->Save("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/xz_slope.tif");

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMAspect()
{
	DWORD ts = GetTickCount();

	auge::DemAspectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemAspectProcessor();

	pProcessor->SetUser(2);
	pProcessor->SetInputDataSource("rsdb2");
	pProcessor->SetInputRaster("srtm_58_05.tif");

	pProcessor->SetOutputDataSource("rsdb2");
	pProcessor->SetOutputRaster("srtm_58_05_aspect.tif");
	pProcessor->SetOutputPath("/s2/s_1_2");

	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}


void RasterTest::DEMAspect_2()
{
	DWORD ts = GetTickCount();

	auge::DemAspectProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemAspectProcessor();

	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster= NULL;

	pinRaster = pRasterIO->Read("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/ASTGTM2_N29E082_dem.tif");

	poutRaster = pProcessor->Aspect(pinRaster);
	poutRaster->Save("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/xz_aspect.tif");

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

//void RasterTest::DEMHillShade()
//{
//	DWORD ts = GetTickCount();
//
//	auge::DemHillshadeProcessor* pProcessor = NULL;
//	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
//	pProcessor = pFactory->CreateDemHillshadeProcessor();
//
//	pProcessor->SetUser(2);
//	pProcessor->SetInputDataSource("rsdb2");
//	pProcessor->SetInputRaster("ASTGTM2_N29E082_dem.tif");
//	pProcessor->SetInputPath("/dem");
//
//	pProcessor->SetOutputDataSource("rsdb2");
//	pProcessor->SetOutputRaster("n39_hillshade_7.tif");
//	pProcessor->SetOutputPath("/dem_result");
//
//	pProcessor->SetAzimuth(135);
//	pProcessor->SetZenith(45);
//	pProcessor->SetZFactor(5.0);
//
//	RESULTCODE rc = pProcessor->Execute();
//
//	pProcessor->Release();
//
//	DWORD te = GetTickCount();
//	printf("[时间]:%d毫秒\n", te-ts);
//}

void RasterTest::DEMHillShade()
{
	//DWORD ts = GetTickCount();

	auge::DemHillshadeProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemHillshadeProcessor();

	//pProcessor->SetUser(2);
	//pProcessor->SetInputDataSource("rsdb2");
	//pProcessor->SetInputRaster("ASTGTM2_N29E082_dem.tif");
	//pProcessor->SetInputPath("/dem");

	//pProcessor->SetOutputDataSource("rsdb2");
	//pProcessor->SetOutputRaster("n39_hillshade_7.tif");
	//pProcessor->SetOutputPath("/dem_result");

	pProcessor->SetAzimuth(135);
	pProcessor->SetZenith(45);
	pProcessor->SetZFactor(5.0);
	
	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster = NULL;
	auge::RasterIO* io = auge::augeGetRasterIOInstance();
	pinRaster = io->Read("H:\\Data\\DEM\\aster_gdem\\xz\\ASTGTM2_N29E082_dem.tif");
	DWORD ts = GetTickCount();
	poutRaster = pProcessor->Hillshade(pinRaster);
	DWORD te = GetTickCount();
	printf("[时间]:%d	毫秒",(te-ts));
	poutRaster->Save("H:\\temp\\aa\\hillshade.tif");
	//RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	//DWORD te = GetTickCount();
	//printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DEMStretch()
{
	DWORD ts = GetTickCount();

	auge::RasterStretchProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterStretchProcessor();

	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster= NULL;

	pinRaster = pRasterIO->Read("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/xz_aspect.tif");

	poutRaster = pProcessor->Stretch(pinRaster);
	poutRaster->Save("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/xz_stretched.tif");

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::DemInundation()
{
	DWORD ts = GetTickCount();
	 
	auge::DemInundationProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateDemInundationProcessor();

	auge::RasterIO* pRasterIO = auge::augeGetRasterIOInstance();
	auge::Raster* pinRaster = NULL;
	auge::Raster* poutRaster= NULL;

	//pinRaster = pRasterIO->Read("E:/Research/Auge.GIS/Dist/32_x86_win_vc10/binD/user/user1/rds/dem/xz_aspect.tif");
	pinRaster = pRasterIO->Read("E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds\\dem\\ASTGTM2_N29E082_dem.tif");

	float water_height = 4000.0f;
	poutRaster = pProcessor->Inundate(pinRaster, 4000);
	poutRaster->Save("E:\\Research\\Auge.GIS\\Dist\\32_x86_win_vc10\\binD\\user\\user1\\rds\\dem\\inundation.tif");

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::Thumbnail()
{
	DWORD ts = GetTickCount();

	auge::RasterThumbnailProcessor* pProcessor = NULL;
	auge::GProcessorFactory* pFactory = auge::augeGetGeoProcessorFactoryInstance();
	pProcessor = pFactory->CreateRasterThumbnailProcessor();

	pProcessor->SetInputRaster("G:\\Data\\aster_gdem\\ASTGTM2_N39E118_dem.tif");
	//pProcessor->SetInputRaster("G:\\Data\\raster\\tif\\world.tif");
	//pProcessor->SetInputRaster("G:\\temp\\aaa\\ASTGTM2_N29E082_dem_stretch.tif");
	//pProcessor->SetInputRaster("G:\\temp\\aaa\\Jellyfish.jpg");
	//pProcessor->SetInputRaster("G:\\temp\\aaa\\Koala.jpg");
	pProcessor->SetOutputRaster("G:\\temp\\aaa\\Jellyfish_thumbnail.jpg");
	
	RESULTCODE rc = pProcessor->Execute();

	pProcessor->Release();

	DWORD te = GetTickCount();
	printf("[时间]:%d毫秒\n", te-ts);
}

void RasterTest::RasterMosiac()
{
	const char* inpaths[] = {
							//"H:\\temp\\dem\\ASTGTM2_N41E115_dem.tif",
							"H:\\temp\\dem\\ASTGTM2_N41E116_dem.tif",
							"H:\\temp\\dem\\ASTGTM2_N40E116_dem.tif"
	};
	const char* outpath = "H:\\temp\\result\\result.tif";

	auge::Raster* poutRaster = NULL;
	auge::RasterMosiacProcessor* processor = NULL;
	auge::GProcessorFactory* factory = auge::augeGetGeoProcessorFactoryInstance();
	processor = factory->CreateRasterMosiacProcessor();
	poutRaster = processor->Mosiac((char**)inpaths, sizeof(inpaths)/sizeof(char*), outpath);

	//poutRaster->Save(outpath);
	poutRaster->Release();

	processor->Release();

}

void RasterTest::RasterProj()
{
	auge::RasterProjectProcessor* processor = NULL;
	auge::GProcessorFactory* factory = auge::augeGetGeoProcessorFactoryInstance();
	processor = factory->CreateRasterProjectProcessor();

	const char* i_path = "H:\\Data\\DEM\\aster_gdem\\xz\\ASTGTM2_N29E082_dem.tif";
	const char* o_path = "H:\\temp\\result\\dem_900913_3.jpg";

	processor->Project(i_path, o_path, 900913);

	processor->Release();

}

void RasterTest::RasterResample()
{
	auge::RasterResampleProcessor* processor = NULL;
	auge::GProcessorFactory* factory = auge::augeGetGeoProcessorFactoryInstance();
	processor = factory->CreateRasterResampleProcessor();

	//const char* i_path = "H:\\Data\\DEM\\aster_gdem\\xz\\ASTGTM2_N29E082_dem.tif";
	//const char* o_path = "H:\\temp\\result\\dem_900913_3.jpg";

	const char* i_path = "H:\\Data\\raster\\tif\\world.tif";
	const char* o_path = "H:\\temp\\result\\resample.tif";
	
	processor->Resample(i_path, o_path, 0.25);

	processor->Release();
}