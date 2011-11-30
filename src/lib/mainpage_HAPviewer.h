/**
 *	\file mainpage_HAPviewer.h
 *	\brief Documentation main page for HAPviewer.
 *	Documentaion can be generated by use of application doxygen.
 *
 * 	Copyright (c) 2009, Eduard Glatz 
 * 
 * 	Author: Eduard Glatz  (eglatz@tik.ee.ethz.ch)
 */

/*!\mainpage Host Application Profile (HAP) Viewer

 A handy host application profile graphlet viewer written in C++.

 It can import network traffic data from files saved in different formats (pcap/nfdump/ipfix/cflow/hpg)
 and supports also the visualization of graph descriptions in DOT language.

 This software is hosted at SourceForge and has a project web page to be found under: http://hapviewer.sourceforge.net


 \section version Stand-alone and Library Version

 HAPviewer is available in two versions. 
 The stand-alone version is a GUI program while the library version is a shared library intended for integration in other tools and frameworks.

 \section build Building and Installing

 HAPviewer needs a couple of packages to be installed in their developer version on the system before a successful build is possible.
 For details see file README or the project documentation web page http://hapviewer.sourceforge.net


 A CMake based buildsystem is provided for building and installing both versions, the headers and the documentation.

 Building and installing:

 mkdir bulid

 ccmake ../$HAPVIEWERSOURCE/ # Configure project as wished

 make

 make install

 Building and installing the doxygen documentation:

 make doc

 make install

 Note: Many build and install options can be changed with ccmake or cmake-gui

 To test the shared library and also to illustrate its use by other programs the test programm "haplibtest.cpp" is provided.

 \section technologies Technologies

 HAPviewer makes use of the follwing technologies and libaries:

 GTK+/gtkmm	GTK GUI middleware for GUI building (accessed through C++ wrapper)

 pcap++		PCAP handling functions (accessed through C++ wrapper)

 ipfix		IPFIX handling function

 nfdump		NFDUMP handling function

 Graphviz	Graph visualization library

 Boost		Boost C++ Libraries


 \section organization Organization of Source Files

 mainpage_HAPviewer.h : 	Provides the main page of doxygen-generated documentation

 HAPviewer.cpp/.h :		Application main file

 gview.cpp/.h :				Most of the GUI stuff (menus, main dialogs)

 glistview_hpg.cpp/.h :		Host browser list view containing graphlet (hpg) meta data (imported from *.hpg)

 glistview_cflow.cpp/.h :		Host browser list view containing per host meta data (imported from traffic data files)

 ghpgdata.cpp/.h :			Host browser metadata storage, HPG-to-DOT transoformation, and host profile graphlet data storage

 gmodel.cpp/.h :			Graphics data handling (visualization of DOT files). DOT-to-GIF transfromation.

 gimport.cpp/.h :			Network traffic data import. CFLOW-to-HPG transformation.

 gutil.cpp/.h :			Collection of utility functions

 HashMapE.cpp/.h :

 lookup3.cpp/.h :

 HashMap.cpp/.h :			Hash table/map with application-specific key definitions

 heapsort.cpp/.h :		Sorting functions

 grole.cpp/.h :			Classes for host role summarization

 cflow.cpp/.h :			Compact format for NetFlow records

 ggraph.cpp/.h	:			Class CGraphlet for graphlet inference

 gfilter.cpp/.h :			Abstract class to read and write network traffic data

 gfilter_cflow.cpp/.h :	Classes to read and write cflow files

 gfilter_argus.cpp/.h :	Class to read argus files

 gfilter_ipfix_vx5ipfix_bi.cpp/.h :
 gfilter_ipfix.cpp/.h :	Example IPFIX file reader. Code based on a template for IPv4 uniflows and biflows.

 gfilter_nfdump_gnfdump.cpp/.h :
 gfilter_nfdump.cpp/.h : 	Contains needed code from nfdump tool set to import flows from nfdump files

 gfilter_pcap.cpp/.h :	Class to read pcap files

 ginterface.cpp/.h :		Interface for the library

 haplibtest.cpp :			Simple example of library usage

 global.h :					Preferences for the building of graphlets

 gsummarynodeinfo.cpp/.h :	Handling of summarized nodes in a way which allows to unsummarize them as requested

 HAPGraphlet.cpp/.h :	Reads a DOT-Graph into memory and allows to access its elements

 hpg.h :						Definition of the HPG format

 IPv6_addr.cpp/.h :		Class IPv6_addr for handling IPv6 and v4 addresses

 mk_cflows.cpp :			Reads textual described flows from a file and generates the according cflow file

 mk_test_cflows.cpp :	Creates a cflow file with some interesting flows

 show_cflows.cpp :		Prints the content of a cflow file to a terminal


 \section formats Supported Traffic Data File Formats

 A couple of input file formats are supported by HAPviewer:

 *.dot:	Graph description format as used by the dot-language of Graphviz

 *.pcap:	Libpcap file format for storage of captured network packet data. We support version 2.4

 nfcapd.*: NfDump/NfSen flow format

 *.dat:	IPFIX flow format (we support uniflows and biflows)
 
 *.gz :	While *.gz in general is used for any data compressed by the GZIP compressor we employ
 it specifically to store flow data in a compact proprietary binary format.
 This format is defined in file cflow.h and stores the most important fields of
 network flow data in a pre-processed way. This format was developed for flow data
 analysis not only in the context of HAPviewer. Given this context it stores more
 flow details than used by HAPviewer.

 *.hpg :	Preprocessed graphlet data in three formats. The v2/3 specification is of primary interest
 as it represents the HAPviewer graphlet shape of a 5-partite graph.
 The v1 specification supports an older shape definition.
 For details on both definitions see file ghpgdata.cpp.



 */