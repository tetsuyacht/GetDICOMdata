#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iterator>

using namespace std;


/* --- Read a DICOM File --- */
string readDICOM(char *fn)
{
	ifstream ifs(fn);

	string sfn = string(fn); // file name as string
	int slen = sfn.length(); // file name length

	if(sfn.substr(slen - 4, 4) != ".dcm"){
		cerr << "Specify a DICOM file (.dcm)" << endl;
		return "";
	}

	if(ifs.fail()){
		cerr << "Cannot read the DICOM file: " << slen << endl;
		return "";
	}
	
	istreambuf_iterator<char> it(ifs);
	istreambuf_iterator<char> last;
	string data(it, last);
	
	ifs.close();
	
	return data;
}


/* --- Read a DICOM Field --- */
double readDICOMfield(string dcmdata, string grp, string elm, string *msg)
{
	if((grp.size() != 4) || (elm.size() != 4)){
		cerr << "If you want to read the DICOM field (0019,1028), the function should be used as follows:" << endl
			<< "\ttreadDICOMfield(DICOMdata as string, \"0019\", \"1028\");" << endl;
		return 0.0;
	}
	
	string fldasc[4]; // little-endian-ordered field name as string
	char fldhex[4]; // little-endian-ordered field name expressed by hexadecimal
	
	for(int i = 0; i < 2; i ++){
		*(fldasc + 1 - i) = grp.substr(2 * i, 2);
		*(fldasc + 3 - i) = elm.substr(2 * i, 2);
	}
	
	for(int i = 0; i < 4; i ++)
		*(fldhex + i) = stoi(*(fldasc + i), nullptr, 16);
	
	string tgtfld = string(fldhex, 4); // target field name on "dcmdata"
	int head = dcmdata.find(tgtfld); // head position of the target field name in bytes (nth byte)
	string typ = dcmdata.substr(head + 4, 2); // data type of the target field
	unsigned short slen; // data length of the target field in bytes as unsigned short
	
	if(typ == "FD")
		slen = 8;
	else if(typ == "LO" || typ == "DS"){
		string str = dcmdata.substr(head + 6, 2);
		unsigned char uc[2];
		
		for(int i = 0; i < 2; i ++)
			*(uc + i) = (unsigned char)str[i];
		
		memcpy(&slen, uc, 2);
	}
	else if(typ == "US")
		slen = 2;
	
	*msg += "<DICOM Field (" + grp + "," + elm + ")>\n";
	
	string dcmtag = grp + elm;
	
	if(dcmtag == "00191028")
		*msg += "BandwidthPerPixelPhaseEncode: ";
	else if(dcmtag == "0051100b")
		*msg += "AcquisitonMatrixText first value (# of phase encoding samples): ";
	else if(dcmtag == "00180095")
		*msg += "ACQPixelBandwidth: ";
	else if(dcmtag == "00280010")
		*msg += "IMGRows: ";
	
	const int len = (int)slen; // data length of the target field in bytes as integer
	string ascflddata = dcmdata.substr(head + 8, len); // field data as string
	unsigned char hexflddata[len]; // field data expressed by hexadecimal
	
	for(int i = 0; i < len; i ++)
		*(hexflddata + i) = (unsigned char)ascflddata[i];
		//cout << setw(2) << setfill('0') << hex << uppercase << (short)*(hexflddata + i) << ' ';
	
	double ans;
	
	if(dcmtag == "00191028"){
		memcpy(&ans, hexflddata, len);
		
		ostringstream sout;
		sout << fixed << setprecision(12) << ans;
		string dstr = sout.str();
		*msg += dstr + " Hz\n";
	}
	else if(dcmtag == "0051100b" || dcmtag == "00180095"){
		double num[2];
		string s;
		istringstream is(ascflddata);
		
		is >> *num >> s >> *(num + 1);
		ans = *num;
		
		*msg += to_string((int)ans) + "\n";
	}
	else if(dcmtag == "00280010"){
		unsigned short us;
		memcpy(&us, hexflddata, len);
		
		*msg += to_string(us) + "\n";
		ans = (double)us;
	}
	
	return ans;
}


int main(int argc, char **argv)
{
	/* --- Variable Commonly Used in the Main Process --- */
	string log;
	
	/* --- Process for Arguments --- */
	char *ifn, *ofn; // input and output filenames
	
	if(argc == 2)
		ifn = *(argv + 1);
	else if(argc == 3){
		ifn = *(argv + 1);
		ofn = *(argv + 2);
	}
	else{
		cerr << endl
			<< "getEchoSpacing version 1.0.0" << endl
			<< endl
			<< "This command gets the echo spacing from a specified DICOM file." << endl
			<< endl
			<< "Usage: getEchoSpacing <DICOM file> <optional: Log file>" << endl
			<< "\tExample: getEchoSpacing IM-0001-0001-0001.dcm" << endl
			<< endl
			<< "If you also want to check values necessary for calculating the echo spacing, execute like this:" << endl
			<< "\tgetEchoSpacing IM-0001-0001-0001.dcm OutputFileNameYouWant.txt" << endl
			<< endl
			<< "Apr 22nd 2019  T.Yamamoto" << endl
			<< endl;
		
		return - 1;
	}
	
	/* --- Read a DICOM File --- */
	string dcmdata = readDICOM(ifn);
	
	if(dcmdata == "")
		return - 1;
	
	int szdcm = dcmdata.size();
	log = "DICOM File size: " + to_string(szdcm) + " bytes\n\n";
	
	/* --- Read DICOM Fields --- */
	double d1 = readDICOMfield(dcmdata, "0019", "1028", &log); // (0019, 1028)
	double d2 = readDICOMfield(dcmdata, "0051", "100b", &log); // (0051, 100b)
	double dans = 1.0 / (d1 * d2);
	
	ostringstream sout;
	sout << fixed << setprecision(12) << dans;
	string dstr = sout.str();
	log += "\nEchoSpacing: " + dstr + " sec\n\tEchoSpacing = 1 / (BandwidthPerPixelPhaseEncode * # of phase encoding samples)";
	
	/* --- Output Values Necessary for Calculating the Echo Spacing --- */
	if(argc == 3){
		ofstream ofs(ofn);
		ofs << log;
	}
	
	/* --- OutPut the Echo Spacing --- */
	cout << fixed << setprecision(12) << dans << endl;
	
	return 0;
}
