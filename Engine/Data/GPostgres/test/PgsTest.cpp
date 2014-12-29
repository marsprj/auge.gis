#include "PgsTest.h"
#include "AugeFeature.h"

CPPUNIT_TEST_SUITE_REGISTRATION(PgsTest);

void PgsTest::setUp() 
{
	//const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=GISDB;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";
	const char* path = "SERVER=127.0.0.1;INSTANCE=5432;DATABASE=gisdb;USER=postgres;PASSWORD=qwer1234;ENCODING=GBK";

	RESULTCODE rc = AG_FAILURE;
	auge::DataEngine* pDataEngine = NULL;
	pDataEngine = auge::augeGetDataEngineInstance();
	
	m_pWorkspace = (auge::FeatureWorksapce*)pDataEngine->CreateWorkspace();
	m_pWorkspace->SetConnectionString(path);

	rc = m_pWorkspace->Open();
	CPPUNIT_ASSERT(rc==AG_SUCCESS);

	printf("setUp\n");
}

void PgsTest::tearDown()
{
	m_pWorkspace->Close();	
	AUGE_SAFE_RELEASE(m_pWorkspace);
	printf("tearDown\n");
}

void PgsTest::ReadTest()
{
	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query();
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uchar* wkb = NULL;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		pGeometry = pFeature->GetGeometry();
		//wkb = pGeometry->AsBinary();

		printf("[Name]:%s\n", pFeature->GetString("name"));
		//auge::WKBPoint* pWKBPoint = (auge::WKBPoint*)wkb;
		//printf("[%d]:%f,%f\n", pFeature->GetFID(),pWKBPoint->point.x, pWKBPoint->point.y);

		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);

	
}

void PgsTest::QueryExent()
{
	auge::GEnvelope extent(0,0,180,90);

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(extent);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}

void PgsTest::QueryBinaryComparision()
{
	

	auge::BinaryComparisonFilter* pFilter = NULL;
	auge::FilterFactory* pFactory = auge::augeGetFilterFactoryInstance();

//	auge::PropertyName* pProp = pFactory->CreatePropertyName("country");
//	auge::GValue* pValue = new auge::GValue("China");
//	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	auge::PropertyName* pProp = pFactory->CreatePropertyName("gid");
	auge::GValue* pValue = new auge::GValue(10);
	auge::Literal* pLiteral = pFactory->CreateLiteral(pValue);

	pFilter = pFactory->CreateBinaryComparisonFilter();
	pFilter->SetExpression1(pProp);
	pFilter->SetExpression2(pLiteral);
	pFilter->SetOperator(auge::augeComOprLessThan);	

	auge::FeatureClass* pFeatureClass = NULL;
	pFeatureClass = m_pWorkspace->OpenFeatureClass("cities");
	CPPUNIT_ASSERT(pFeatureClass!=NULL);

	auge::FeatureCursor* pCursor = NULL;
	pCursor = pFeatureClass->Query(pFilter);
	CPPUNIT_ASSERT(pCursor!=NULL);

	g_uint i=0;
	auge::Geometry	*pGeometry = NULL;
	auge::Feature	*pFeature = NULL;
	while((pFeature=pCursor->NextFeature())!=NULL)
	{	
		printf("\r%d",++i);
		pFeature->Release();
	}
	printf("\n");

	AUGE_SAFE_RELEASE(pFilter);
	AUGE_SAFE_RELEASE(pCursor);
	AUGE_SAFE_RELEASE(pFeatureClass);
}