AUTOMAKE_OPTION=foreign
lib_LTLIBRARIES=libgsufs.la
AM_CXXFLAGS=-D_LINUX

AM_CPPFLAGS=-I../include \
		-I$(top_srcdir)/include

libgsufs_la_LDFLAGS=\
		-L$(top_srcdir)/Engine/Core/src \
		-L$(top_srcdir)/Engine/Server/gscore/src

libgsufs_la_LIBADD=-lgcore -lgscore

libgsufs_la_SOURCES=\
		    CreateFolderHandler.cpp \
		    CreateFolderRequest.cpp \
		    RemoveFolderHandler.cpp \
		    RemoveFolderRequest.cpp \
		    RemoveFileHandler.cpp \
		    RemoveFileRequest.cpp \
		    MoveHandler.cpp \
		    MoveRequest.cpp \
		    ListRequest.cpp \
		    ListResponse.cpp \
		    ListHandler.cpp \
		    DescribeCsvHandler.cpp \
		    DescribeCsvRequest.cpp \
		    DescribeCsvResponse.cpp \
		    WFileSystemEngine.cpp
