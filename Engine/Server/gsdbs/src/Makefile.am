AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsdbs.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsdbs_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Carto/src \
		-L$(top_srcdir)/Engine/Server/gscore/src \
		-L$(top_srcdir)/Engine/Server/gsvrs/src

libgsdbs_la_LIBADD=-lgcore -lgcarto -lgscore -lgsvrs

libgsdbs_la_SOURCES=\
		    GetDataSourceHandler.cpp \
		    GetDataSourceRequest.cpp \
		    GetDataSourceResponse.cpp \
		    RegisterDataSourceHandler.cpp \
		    RegisterDataSourceRequest.cpp \
		    UnRegisterDataSourceHandler.cpp \
		    UnRegisterDataSourceRequest.cpp \
		    GetDataSetHandler.cpp \
		    GetDataSetRequest.cpp \
		    GetDataSetResponse.cpp \
		    RefreshDataSetHandler.cpp \
		    RefreshDataSetRequest.cpp \
		    GetPreviewRequest.cpp \
		    GetPreviewResponse.cpp \
		    GetPreviewHandler.cpp \
		    CreateDataSetRequest.cpp \
		    CreateDataSetHandler.cpp \
		    RemoveDataSetRequest.cpp \
		    RemoveDataSetHandler.cpp \
		    CreateTileStoreHandler.cpp \
		    CreateTileStoreRequest.cpp \
		    RemoveTileStoreHandler.cpp \
		    RemoveTileStoreRequest.cpp \
		    DescribeTileStoreHandler.cpp \
		    DescribeTileStoreRequest.cpp \
		    DescribeTileStoreResponse.cpp \
		    GetTileHandler.cpp \
		    GetTileRequest.cpp \
		    GetTileResponse.cpp \
		    TestConnectionRequest.cpp \
		    TestConnectionHandler.cpp \
		    WDataEngine.cpp

