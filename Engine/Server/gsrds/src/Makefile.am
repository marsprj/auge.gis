AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsrds.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsrds_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Raster/src \
		-L$(top_srcdir)/Engine/Processor/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsrds_la_LIBADD=-lgcore -lgscore -lgraster -lgprocessor

libgsrds_la_SOURCES=\
		    CreateFolderHandler.cpp \
		    CreateFolderRequest.cpp \
		    RemoveFolderHandler.cpp \
		    RemoveFolderRequest.cpp \
		    RemoveFileHandler.cpp \
		    RemoveFileRequest.cpp \
		    GetRasterHandler.cpp \
		    GetRasterRequest.cpp \
		    GetRasterResponse.cpp \
		    GetThumbnailHandler.cpp \
		    GetThumbnailRequest.cpp \
		    GetThumbnailResponse.cpp \
		    RemoveRasterHandler.cpp \
		    RemoveRasterRequest.cpp \
		    AddRasterHandler.cpp \
		    AddRasterRequest.cpp \
		    DescribeRasterHandler.cpp \
		    DescribeRasterRequest.cpp \
		    DescribeRasterResponse.cpp \
		    GetValueHandler.cpp \
		    GetValueRequest.cpp \
		    GetValueResponse.cpp \
		    MoveHandler.cpp \
		    MoveRequest.cpp \
		    ListRequest.cpp \
		    ListResponse.cpp \
		    ListHandler.cpp \
		    WRasterEngine.cpp
