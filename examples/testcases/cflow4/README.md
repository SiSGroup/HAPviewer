All graphlets stored in the folder "known-good-graphlets" got generated with most recent HAPviewer as of the 8th March 2011 and are used as references.

Since the version of the 8th has/had some bugs...

* the file demo.gz in examples/cflow4/ got used as input
* affected (wrong) graphlets got corrected manually

Note: For future testing please use the splitted files in this folder

To compare the graphlets generated by the old HAPviewer version with the boost graph library, the following patch has been applied:

	--- HAPviewer_17FEB2011/ghpgdata.cpp.original	2011-12-07 23:38:12.395141700 +0100
	+++ HAPviewer_17FEB2011/ghpgdata.cpp	2011-12-07 09:50:08.889369319 +0100
	@@ -1555,14 +1555,15 @@ bool ChpgData::hpg2dot3(int index, char
	 	char st[] = { "k1_" };
	 	// We expect first edge to have rank=localIP_prot
	 	// (must be an edge incident to localIP)
	+	static int counter_for_unique_subgraphs = 0;
	 	if (rank==localIP_prot) {
	 		uint32_t localIP = value[1];
	 		// Rank list
	 		if (prefix) {
	-			outfs << "{rank=same;\"localIP\";" << st << localIP << ";}\n";
	+			outfs << "subgraph " << counter_for_unique_subgraphs++ << " {rank=same;\"localIP\";" << st << localIP << ";}\n";
	 			if (dbg5) cout << "{rank=same;\"localIP\";" << st << localIP << ";}\n";
	 		} else {
	-			outfs << "{rank=same;\"localIP\";" << localIP << ";}\n";
	+			outfs << "subgraph " << counter_for_unique_subgraphs++ << " {rank=same;\"localIP\";" << localIP << ";}\n";
	 		}
	 		// Node annotation
	 		char adr[16];
	@@ -1615,7 +1616,8 @@ bool ChpgData::hpg2dot3(int index, char
	 			if (last_rank != totalBytes) { // Suppress for pseudo node that is not visualized
	 
	 				// a) Output header text for "same rank" list
	-				outfs << "{rank=same;"; if (dbg5) cout << "{rank=same;";
	+				outfs << "subgraph " << counter_for_unique_subgraphs++ << " {rank=same;";
	+				if (dbg5) cout << "{rank=same;";
	 
	 				// b) Output "pseudo node" as descriptive text for old partition
	 				switch (last_rank) {

To allow the haplibtest programm to compile, the following patches have been applied:

	--- HAPviewer_17FEB2011/ginterface.cpp.original	2011-12-07 23:40:43.255362960 +0100
	+++ HAPviewer_17FEB2011/ginterface.cpp	2011-12-06 18:06:20.886437913 +0100
	@@ -76,7 +76,9 @@ bool CInterface::handle_binary_import(st
	 			return false;
	 		}
	 
	-	} else if (itype == CImport::isPcap) {
	+	} 
	+	#ifdef PCAP
	+	else if (itype == CImport::isPcap) {
	 		if (netmask == 0) {
	 			// Network address and/or prefix are invalid: do not continue
	 			return false;
	@@ -90,7 +92,10 @@ bool CInterface::handle_binary_import(st
	 			return false;
	 		}
	 
	-	} else if (itype == CImport::isNfdump) {
	+	} 
	+	#endif
	+	#ifdef NFDUMP
	+	else if (itype == CImport::isNfdump) {
	 
	 		// Is nfdump binary data
	 		try {
	@@ -101,7 +106,10 @@ bool CInterface::handle_binary_import(st
	 			return false;
	 		}
	 
	-	} else if (itype == CImport::isIpfix) {
	+	}
	+	#endif
	+	#ifdef IPFIX
	+	else if (itype == CImport::isIpfix) {
	 
	 		// Is nfdump binary data
	 		try {
	@@ -112,7 +120,9 @@ bool CInterface::handle_binary_import(st
	 			return false;
	 		}
	 
	-	} else {
	+	}
	+	#endif
	+	else {
	 
	 		string errtext = "unknown file type (hint: supported are *.gz, *.pcap, nfcapd* and *.dat)\n";
	 		cerr << errtext << endl;

	--- HAPviewer_17FEB2011/gimport.h.original	2011-12-07 23:40:10.165314424 +0100
	+++ HAPviewer_17FEB2011/gimport.h	2011-12-06 18:04:30.306275711 +0100
	@@ -101,9 +101,15 @@ class CImport
	 
	 		// Flow imports
	 		void read_cflow_file();
	+		#ifdef PCAP
	 		void read_pcap_file(uint32_t local_net, uint32_t netmask);
	+		#endif // PCAP
	+		#ifdef NFDUMP
	 		void read_nfdump_file(uint32_t local_net, uint32_t netmask);			
	+		#endif
	+		#ifdef IPFIX
	 		void read_ipfix_file(uint32_t local_net, uint32_t netmask);	
	+		#endif // IPFIX
	 		void cflow2hpg();
	 
	 		// Flow helper functions

Add some functionality to haplitest:

	--- haplibtest.cpp.original	2012-01-02 10:49:48.282393770 +0100
	+++ haplibtest.cpp	2012-01-02 10:50:06.222296580 +0100
	@@ -3,31 +3,171 @@
	   *	\brief Test program for HAPviewer library.
	   *
	   *
	-  *	To compile: g++ -g haplibtest.cpp -lhapviz -o haplibtest -Wno-deprecated -L/home/eglatz/bin/lib/
	+  *	To compile: g++ -g haplibtest.cpp -lhapviz -lboost_program_options -o haplibtest -Wno-deprecated -L/home/eglatz/bin/lib/
	   */
	 
	 #include <iostream>
	 #include "ginterface.h"
	+#include <boost/program_options.hpp>
	 
	 using namespace std;
	 
	+int main(int argc, char * argv[]) {
	+	// 1. Process command line
	+	// ***********************
	+	//
	+	// Check for needed command line arguments and process supported command line options
	+	boost::program_options::variables_map variablesMap;
	+	boost::program_options::options_description desc("Allowed options");
	+
	+	string outfilename; ///< Output filename (Default: test.dot)
	+	CInterface::filter_flags_t filters = static_cast<CInterface::filter_flags_t>(0); ///< Filtersettings
	+
	+	CInterface libif; ///< Provides access to the HAPviewer functionality
	+	CInterface::summarize_flags_t sum_flags = CInterface::summarize_all; ///< Summary filter flags
	+
	+	string IP_str;
	+
	+	try {
	+		desc.add_options()
	+				("inputfile,i", boost::program_options::value<string>(), "File to read")
	+				("outputfile,o", boost::program_options::value<string>(&outfilename)->default_value("test.dot"),"Name of output file")
	+				("ip", boost::program_options::value<string>(), "Host IP address")
	+
	+				("notcp", "Filter TCP traffic, overrides the tcponly flag")
	+				("noicmp", "Filter ICMP traffic, overrides the icmponly flag")
	+				("noudp", "Filter UDP traffic, overrides the icmponly flag")
	+				("noother", "Filter UDP traffic, overrides the otheronly flag")
	+
	+				("tcponly", "Show only TCP traffic")
	+				("udponly", "Show only UDP traffic")
	+				("icmponly", "Show only ICMP traffic")
	+				("otheronly", "Show only non TCP/ICMP/UDP traffic")
	+
	+				("nosummarize", "Do not summarize per default (default: summarize all roles)")
	+
	+				("nosumserverroles", "Desummarize server roles, overrides the sumserverroles flag")
	+				("nosumclientroles", "Desummarize client roles, overrides the sumclientroles flag")
	+				("nosump2proles", "Desummarize peer 2 peer roles, overrides the sump2proles flag")
	+				("nosummulticlientroles", "Desummarize multiclient roles, overrides the summulticlientroles flag")
	+
	+				("sumserverroles", "Summarize server roles")
	+				("sumclientroles", "Summarize client roles")
	+				("sump2proles", "Summarize peer 2 peer roles")
	+				("summulticlientroles", "Summarize multiclient roles (default: summarize all roles)")
	+
	+				("help,h", "show this help message")
	+			;
	+
	+		boost::program_options::positional_options_description posOpt;
	+		posOpt.add("inputfile", 1);
	+		posOpt.add("ip", 1);
	+
	+		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variablesMap);
	+		boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(posOpt).run(), variablesMap);
	+		boost::program_options::notify(variablesMap);
	+	} catch (std::exception & e) {
	+		cerr << "Error: " << e.what() << endl;
	+		exit(1);
	+	}
	 
	-int main(int argc, char * argv[])
	-{
	-	if (argc < 3) {
	-		cerr << "ERROR: too few arguments.\n";
	-		cerr << "Usage: " << argv[0] << " datafile IP_addr\n\n";
	-		return 1;
	+	// Set stuff up as requested by the user
	+	if (variablesMap.count("help")) {
	+		cerr << desc;
	+		exit(0);
	+	}
	+
	+	if (!variablesMap.count("inputfile")) {
	+		cerr << desc;
	+		exit(1);
	+	}
	+
	+	if (!variablesMap.count("ip")) {
	+		cerr << desc;
	+		exit(1);
	+	}
	+
	+	IP_str = variablesMap["ip"].as<string>();
	+	string in_filename = variablesMap["inputfile"].as<string>();
	+
	+	if((variablesMap.count("tcponly") + variablesMap.count("udponly") + variablesMap.count("icmponly") + variablesMap.count("otheronly")) > 1) {
	+		cerr << "Please use only one \"*only\" flag per call" << endl;
	+		exit(1);
	+	}
	+
	+	if(variablesMap.count("tcponly") && !variablesMap.count("notcp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(CInterface::filter_icmp + CInterface::filter_other + CInterface::filter_udp);
	+	} else if(variablesMap.count("udponly") && !variablesMap.count("noudp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(CInterface::filter_icmp + CInterface::filter_other + CInterface::filter_tcp);
	+	} else if(variablesMap.count("icmponly") && !variablesMap.count("noicmp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(CInterface::filter_udp + CInterface::filter_other + CInterface::filter_tcp);
	+	} else if(variablesMap.count("otheronly") && !variablesMap.count("noother")) {
	+		filters = static_cast<CInterface::filter_flags_t>(CInterface::filter_udp + CInterface::filter_icmp + CInterface::filter_tcp);
	+	}
	+
	+	if(variablesMap.count("notcp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(filters ^ CInterface::filter_tcp);
	+	}
	+	if(variablesMap.count("noudp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(filters ^ CInterface::filter_udp);
	+	}
	+	if(variablesMap.count("noicmp")) {
	+		filters = static_cast<CInterface::filter_flags_t>(filters ^ CInterface::filter_icmp);
	+	}
	+	if(variablesMap.count("noother")) {
	+		filters = static_cast<CInterface::filter_flags_t>(filters ^ CInterface::filter_other);
	 	}
	-	string in_filename = argv[1];
	-	string outfilename = "test99.dot";
	-	string IP_str = argv[2];
	-
	-	CInterface libif;
	-	CInterface::summarize_flags_t sum_flags = CInterface::summarize_all;
	-	bool ok = libif.get_graphlet(in_filename, outfilename, IP_str, sum_flags, (CInterface::filter_flags_t)0);
	+
	+	/**
	+	 * 	("nosummarize", "Do not summarize per default (default: summarize all roles)")
	+	 */
	+
	+	if(variablesMap.count("nosummarize")) {
	+		sum_flags = static_cast<CInterface::summarize_flags_t>(0);
	+	}
	+
	+	/**
	+	 * ("sumserverroles", "Summarize server roles (default: summarize all roles)")
	+	 * ("sumclientroles", "Summarize client roles (default: summarize all roles)")
	+	 * ("sump2proles", "Summarize peer 2 peer roles (default: summarize all roles)")
	+	 * ("summulticlientroles", "Summarize multiclient roles (default: summarize all roles)")
	+	 */
	+	if(variablesMap.count("sumserverroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags | CInterface::summarize_server_roles);
	+	}
	+	if(variablesMap.count("sumclientroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags | CInterface::summarize_client_roles);
	+	}
	+	if(variablesMap.count("sump2proles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags | CInterface::summarize_p2p_roles);
	+	}
	+	if(variablesMap.count("summulticlientroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags | CInterface::summarize_multi_client_roles);
	+	}
	+
	+	/*
	+		("nosumserverroles", "Summarize server roles")
	+		("nosumclientroles", "Summarize client roles (default: summarize all roles)")
	+		("nosump2proles", "Summarize peer 2 peer roles (default: summarize all roles)")
	+		("nosummulticlientroles", "Summarize multiclient roles (default: summarize all roles)")
	+	 */
	+	if(variablesMap.count("nosumserverroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags ^ CInterface::summarize_server_roles);
	+	}
	+	if(variablesMap.count("nosumclientroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags ^ CInterface::summarize_client_roles);
	+	}
	+	if(variablesMap.count("nosump2proles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags ^ CInterface::summarize_p2p_roles);
	+	}
	+	if(variablesMap.count("nosummulticlientroles")) {
	+			sum_flags = static_cast<CInterface::summarize_flags_t>(sum_flags ^ CInterface::summarize_multi_client_roles);
	+	}
	+
	+	bool ok = libif.get_graphlet(in_filename, outfilename, IP_str, sum_flags, filters);
	+
	 	if (!ok) {
	-		cerr << "ERROR: could not create a dot file from input data.\n\n";
	+		cerr << "ERROR: could not create a dot file from input data." << endl;
	 		return 1;
	 	} else {
	 		cout << "Successfully created file " << outfilename << endl;
	@@ -35,4 +175,3 @@ int main(int argc, char * argv[])
	 
	 	return 0;
	 }
	-
