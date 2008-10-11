#include <config.h>

#include "configuration.h"
#include "confdialog.h"
#include "xmlreader.h"
#include "soundconf.h"
#include "waterfall.h"

#if USE_HAMLIB
	#include "hamlib.h"
	#include "rigclass.h"
#endif

#include "rigMEM.h"
//#include "rigio.h"
#include "debug.h"

#include <iostream>
#include <fstream>

#ifdef __linux__
#  include <dirent.h>
#  include <limits.h>
#  include <errno.h>
#endif
#ifdef __APPLE__
#  include <glob.h>
#endif
#ifndef __CYGWIN__
#  include <sys/stat.h>
#endif

configuration progdefaults = {
	false,			// bool		rsid;
	false,			// bool		rsidWideSearch;
	false,			// bool		TransmitRSid;
	true,			// bool		slowcpu;
	false,			// bool		experimental;
	
	false,			// bool		changed;
	-20.0,			// double	wfRefLevel;
	70.0,			// double	wfAmpSpan;
	300,			// int		LowFreqCutoff;
	1000,			// int		CWsweetspot;
	1000,			// int		RTTYsweetspot;
	1000,			// int		PSKsweetspot;
	false,			// bool		StartAtSweetSpot;
	false,			// bool		WaterfallHistoryDefault;
	false,			// bool		WaterfallQSY;
	"",			// string	WaterfallClickText;
	waterfall::WF_CARRIER,	// int		WaterfallWheelAction;

//  for PSK mail interface	
	false,			// bool		PSKmailSweetSpot;
	200,			// int		SearchRange;
	40,				// int		ServerOffset;
	6.0,			// double	ACQsn;
// RTTY
	3,				// int			rtty_shift; = 170
	0,				// int			rtty_baud; = 45
	0,				// int 			rtty_bits; = 5
	RTTY_PARITY_NONE,	// RTTY_PARITY	rtty_parity;
	1,				// int			rtty_stop;
	false,			// bool 		rtty_reverse;
	false,			// bool		rtty_msbfirst;
	false,  		// bool		rtty_crcrlf;
	true,			// bool		rtty_autocrlf;
	72,				// int		rtty_autocount;
	1,				// int		rtty_afcspeed;
	false,			// bool		useFSKkeyline;
	false,			// bool		useFSKkeylineDTR;	
	true,			// bool		FSKisLSB;
//	true,			// bool		RTTY_USB;
	false,			// bool		useUART;
	false,			// bool		PreferXhairScope;
	false,			// bool		PseudoFSK;
	true,			// bool		UOSrx; // unshift on space - receive
	true,			// bool		UOStx; // unshift on space - transmit
	false,			// bool		Xagc; // use agc for X-scope
// CW
	false,			// bool		useCWkeylineRTS;
	false,			// bool		useCWkeylineDTR;
	50,				// int		CWweight;
	18,				// int		CWspeed;
	24,				// int		defCWspeed;
	150,			// int		CWbandwidth;
	true,			// int		CWtrack;
	10,				// int		CWrange;
	5,				// int		CWlowerlimit;
	50,				// int		CWupperlimit;
	4.0,			// double	CWrisetime;
	3.0,			// double	CWdash2dot;
	false,			// bool		QSK;
	4.0,			// double	CWpre;
	4.0,			// double	CWpost;
	false,			// bool		CWid;
	18,				// int		CWIDwpm;

// FELD-HELL
	150.0,			// double	HELL_BW;
	false,			// bool		HellRcvWidth;
	false,			// bool		HellBlackboard;
	1,				// bool		HellXmtWidth;
	true,			// bool		HellXmtIdle;
	false,			// bool		HellPulseFast;
// OLIVIA
	2,				// int		oliviatones;
	2,				// int		oliviabw;
	8,				// int		oliviasmargin
	4,				// int		oliviasinteg
	false,			// bool		olivia8bit
// THOR
	2.0,			// double	THOR_BW;
	true,			// bool		THOR_FILTER;
	"",				// string	THORsecText;
	5,				// int		THOR_PATHS;
	false,			// bool		THOR_SOFT;
	0.0,			// double	ThorCWI;
// DOMINOEX
	2.0,			// double	DOMINOEX_BW;
	true,			// bool		DOMINOEX_FILTER
	false,			// bool		DOMINOEX_FEC
	5,				// int		DOMINOEX_PATHS
	0.0,			// double	DomCWI;
// MT63
	false,			// bool 	mt63_8bit;
	32,				// int		mt63_interleave;
//
	0,				// uchar 	red
	255,			// uchar 	green
	255,			// uchar 	blue
	0,				// bool 	MultiColorWF;
	1,				// int  	wfPreFilter == Blackman
	false,			// bool		WFaveraging
	4,				// int		latency;
	true,			// bool 	UseCursorLines;
	true,			// bool 	UseCursorCenterLine;
	true,			// bool 	UseBWTracks;
	{255,255,0,255},		// RGBI	cursorLineRGBI;
	{255, 255, 255, 255},	// RGBI	cursorCenterRGBI;
	{255,0,0,255},			// RGBI	bwTrackRGBI;
	4,				// int		feldfontnbr;
	false,			// bool		viewXmtSignal;
	false,			// bool		sendid;
	false,			// bool		macroid;
	false,			// bool		sendtextid;
	"CQ",			// string	strTextid;
	false,			// bool		macroCWid;
	1,				// int		videowidth;
	true,			// bool		ID_SMALL;
	false,			// bool		macrotextid;
	0,				// int		QRZ;
	"",             // string   QRZpathname;
	"",				// string	QRZusername;
	"",				// string	QRZuserpassword;
	false,			// bool     QRZchanged;
//
	true,			// bool		btnusb;
	0, 				// int 		btnPTTis
	false,			// bool		RTSptt;
	false,			// bool		DTRptt;
	false,			// bool		RTSplus;
	false,			// bool		DTRplus;
	0,				// int 		choiceHAMLIBis
	0,				// int 		chkUSEMEMMAPis
	0,				// int 		chkUSEHAMLIBis
	0,				// int		chkUSERIGCATis
	0,				// int		chkUSEXMLRPCis
#if defined(__linux__)
	"/dev/ttyS0",		// string	PTTdev
	"/dev/ttyS1",		// string	CWFSKport
	"/dev/ttyS0",		// string	HamRigDevice
#elif defined(__CYGWIN__)
	"COM1",			// string	PTTdev
	"COM2",			// string	CWFSKport
	"COM1",			// string	HamRigDevice
#else // not sure
	"/dev/ptt",		// string	PTTdev
	"/dev/fsk",		// string	CWFSKport
	"/dev/rig",		// string	HamRigDevice
#endif
	"",			// string	HamRigName
	1,			// int		HamRigBaudrate
//
	"",				// myCall
	"",				// myName
	"",				// myQth
	"",				// myLoc
#if defined(__linux__)

#elif defined(__CYGWIN__)

#else

#endif
	"",				// secondary text
// Sound card
	SND_IDX_PORT,		// int		btnAudioIOis
	"",		// string	OSSdevice;
	"",		// string	PAdevice;
	"",		// string	PortIndevice;
	-1,		// int		PortInIndex;
	"",		// string	PortOutDevice;
	-1,		// int		PortOutIndex;
	0,		// int		PortFramesPerBuffer
	"",		// string	PulseServer
	SAMPLE_RATE_UNSET,		// int		sample_rate;
	SAMPLE_RATE_UNSET,		// int		in_sample_rate;
	SAMPLE_RATE_UNSET,		// int		out_sample_rate;
	SRC_SINC_FASTEST,		// int		sample_converter;
	0,				// int		RX_corr;
	0,				// int		TX_corr;
	0,				// int		TxOffset;
// Contest controls
	true,			// bool	UseLeadingZeros;
	0,				// int		ContestStart;
	4,				// int		ContestDigits;
// Macro timer constants and controls
	false,			// bool	useTimer;
	0,				// int		macronumber;
	0,				// int		timeout;
	0,				// bool		UseLastMacro;
	0,				// bool		DisplayMacroFilename;
	
	"",			// string	MXdevice
	false,			// bool		MicIn;
	true,			// bool		LineIn;
	false,			// bool		EnableMixer;
	true,			// bool 	MuteInput;
	80.0,			// double	PCMvolume
	{{  0,  0,  0},{  0,  0,  62},{  0,  0,126}, // default palette
	 {  0,  0,214},{145,142,  96},{181,184, 48},
	 {223,226,105},{254,254,   4},{255, 58,  0} },
	 	
// Button key color palette
	true,			  // bool useGroupColors;
	{  80, 144, 144}, // RGBint btnGroup1;
	{ 144,  80,  80}, // RGBint btnGroup2;
	{  80,  80, 144}, // RGBint btnGroup3;
	{ 255, 255, 255}, // RGBint btnFkeyTextColor;

// Rx / Tx / Waterfall Text Widgets

	FL_SCREEN,		// Fl_Font 		RxFontnbr
	16,				// int 		RxFontsize
	FL_BLACK,				// Fl_Color		RxFontcolor
	FL_SCREEN,		// Fl_Font 		TxFontnbr
	16,				// int 		TxFontsize
	FL_BLACK,				// Fl_Color		TxFontcolor
	{ 255, 242, 190}, // RGBint RxColor;
	{ 200, 235, 255}, // RGBint TxColor;
	
	FL_RED,			// Fl_Color		XMITcolor;
	FL_DARK_GREEN,	// Fl_Color		CTRLcolor;
	FL_BLUE,		// Fl_Color		SKIPcolor;
	FL_DARK_MAGENTA,// Fl_Color		ALTRcolor;
	
	FL_SCREEN,		// Fl_Font		WaterfallFontnbr
	12,				// int		WaterfallFontsize

	"gtk+",				// string	ui_scheme

        9876,		// int		rx_msgid
        6789,		// int		tx_msgid
	"127.0.0.1",	// string	arq_address
	"3122",		// string	arq_port
// PSK viewer parameters
	true,			// bool	VIEWERmarquee
	true,			// bool	VIEWERshowfreq
	500,			// int		VIEWERstart
	20,				// int		VIEWERchannels
	10.0,			// double	VIEWERsquelch
	15,				// int  VIEWERtimeout
	"127.0.0.1",		// string xmlrpc_address
	"7362",			// string xmlrpc_port
	"",			// string xmlrpc_allow
	""			// string xmlrpc_deny
};

const char *szBaudRates[] = {
	"", 
	"300","600","1200","2400",
	"4800","9600","19200","38400",
	"57600","115200","230400","460800"};
	
const char *szBands[] = {
	"",
	"1830", "3580", "7030", "7070", "10138",
	"14070", "18100", "21070", "21080", "24920", "28070", "28120", 0};


// XML config file support
enum TAG { \
	IGNORE,
	MYCALL, MYNAME, MYQTH, MYLOC, 
	SQUELCH, WFREFLEVEL, WFAMPSPAN, LOWFREQCUTOFF, 
	WATERFALLHISTORYDEFAULT, WATERFALLQSY, WATERFALLCLICKTEXT, WATERFALLWHEELACTION,
	STARTATSWEETSPOT, PSKMAILSWEETSPOT, 
	PSKSEARCHRANGE, PSKSERVEROFFSET,
	ACQSN,
	CWSWEETSPOT, PSKSWEETSPOT, RTTYSWEETSPOT,
	RTTYSHIFT, RTTYBAUD,
	RTTYBITS, RTTYPARITY, RTTYSTOP, RTTYREVERSE,
	RTTYMSBFIRST, RTTYCRCLF, RTTYAUTOCRLF,
	RTTYAUTOCOUNT, RTTYAFCSPEED,
	RTTYUSB,
	PREFERXHAIRSCOPE, 
	PSEUDOFSK,
	UOSRX, UOSTX,
	XAGC,
	CWWEIGHT, CWSPEED, CWDEFSPEED,
	CWBANDWIDTH, CWRANGE, CWLOWERLIMIT, CWUPPERLIMIT,
	CWTRACK, CWRISETIME, CWDASH2DOT,
	XQSK, CWPRE, CWPOST, CWID, CWIDWPM,
	OLIVIATONES, OLIVIABW, OLIVIASMARGIN, OLIVIASINTEG, OLIVIA8BIT,
	THORBW, THORFILTER, THORSECTEXT, THORPATHS, THORSOFT, THORCWI,
	DOMINOEXBW, DOMINOEXFILTER, DOMINOEXFEC, DOMINOEXPATHS, DOMCWI,
	FELDFONTNBR,
	HELLRCVWIDTH, HELLXMTWIDTH, HELLBLACKBOARD, HELLPULSEFAST, HELLXMTIDLE,
	WFPREFILTER, LATENCY,
	USECURSORLINES, USECURSORCENTERLINE, USEBWTRACKS,
	CLCOLORS,
	CCCOLORS,
	BWTCOLORS,
	VIEWXMTSIGNAL, SENDID, MACROID, SENDTEXTID, STRTEXTID, VIDEOWIDTH, IDSMALL,
	QRZTYPE, QRZPATHNAME, QRZUSER, QRZPASSWORD,
	BTNUSB, BTNPTTIS,
	RTSPTT, DTRPTT, RTSPLUS, DTRPLUS,
	CHOICEHAMLIBIS, CHKUSEMEMMAPIS,
	CHKUSEHAMLIBIS, CHKUSERIGCATIS, CHKUSEXMLRPCIS,
	HAMRIGNAME, HAMRIGDEVICE, HAMRIGBAUDRATE,
	PTTDEV,
	SECONDARYTEXT, 
	AUDIOIO, OSSDEVICE, PADEVICE, PORTINDEVICE, PORTININDEX, PORTOUTDEVICE, PORTOUTINDEX, PULSESERVER,
	SAMPLERATE, INSAMPLERATE, OUTSAMPLERATE, SAMPLECONVERTER, RXCORR, TXCORR, TXOFFSET,
	USELEADINGZEROS, CONTESTSTART, CONTESTDIGITS,
	USETIMER, MACRONUMBER, TIMEOUT, USELASTMACRO, DISPLAYMACROFILENAME,
	MXDEVICE, 
	PCMVOLUME,
	MICIN, LINEIN, ENABLEMIXER, MUTEINPUT,
	PALETTE0, PALETTE1, PALETTE2, PALETTE3, PALETTE4, 
	PALETTE5, PALETTE6, PALETTE7, PALETTE8,
	VIEWERMARQUEE, VIEWERSHOWFREQ, VIEWERSTART, 
	VIEWERCHANNELS, VIEWERSQUELCH, VIEWERTIMEOUT, WFAVERAGING,
	USEGROUPCOLORS, FKEYGROUP1, FKEYGROUP2, FKEYGROUP3,
	FKEYTEXTCOLOR,
	RXFONTNBR, RXFONTSIZE, RXFNTCOLOR, TXFONTNBR, TXFONTSIZE, TXFNTCOLOR,
	RXFONTCOLOR, TXFONTCOLOR, XMITCOLOR, CTRLCOLOR, SKIPCOLOR, ALTRCOLOR,
	WATERFALLFONTNBR, WATERFALLFONTSIZE, UISCHEME,
	RSIDWIDESEARCH, TRANSMITRSID, SLOWCPU,
	MT638BIT, MT63INTERLEAVE
};
	
void writeXMLint(ofstream &f, const char * tag,  int val)
{
	f << "<" << tag << ">" << val << "</" << tag << ">\n";
}

void writeXMLdbl(ofstream &f, const char * tag, double val)
{
	f << "<" << tag << ">" << val << "</" << tag << ">\n";
}

void writeXMLstr(ofstream &f, const char * tag, string val)
{
	size_t indx = val.find("&");
	while (indx != string::npos) {
		val.replace(indx, 1, "&amp;");
		indx = val.find("&", indx + 1);
	}
	while ((indx = val.find("<")) != string::npos)
		val.replace(indx, 1, "&lt;");
	while ((indx = val.find(">")) != string::npos)
		val.replace(indx, 1, "&gt;");
	while ((indx = val.find('"')) != string::npos)
		val.replace(indx, 1, "&quot;");
	while ((indx = val.find("'")) != string::npos)
		val.replace(indx, 1, "&apos;");
	f << "<" << tag << ">" << val.c_str() << "</" << tag << ">\n";
}

void writeXMLbool(ofstream &f, const char * tag, bool val)
{
	f << "<" << tag << ">" << val << "</" << tag << ">\n";
}

void writeXMLPalette(ofstream &f, int n, int r, int g, int b)
{
	f << "<PALETTE" << n << ">";
	f << r << " " << g << " " << b;
	f << "</PALETTE" << n << ">\n";
}

void writeXMLrgb(ofstream &f, const char *tag, int r, int g, int b)
{
	f << "<" << tag << ">" << r << " " << g << " " << b ;
	f << "</" << tag << ">\n";
}

string getstring(IrrXMLReader* xml)
{
	return (xml->getNodeData());
}


void configuration::writeDefaultsXML()
{
	string deffname(HomeDir);
	deffname.append("fldigi_def.xml");

	string deffname_backup(deffname);
	deffname_backup.append("-old");
	rename(deffname.c_str(), deffname_backup.c_str());

	ofstream f(deffname.c_str(), ios::out);
	if (!f) {
		LOG_ERROR("Could not write %s", deffname.c_str());
		return;
	}

	f << "<FLDIGI_DEFS>\n";

	writeXMLstr(f, "MYCALL", myCall);
	writeXMLstr(f, "MYNAME", myName);
	writeXMLstr(f, "MYQTH", myQth);
	writeXMLstr(f, "MYLOC", myLocator);

	writeXMLdbl(f, "WFREFLEVEL", wfRefLevel);
	writeXMLdbl(f, "WFAMPSPAN", wfAmpSpan);
	writeXMLint(f, "LOWFREQCUTOFF", LowFreqCutoff);

	writeXMLbool(f, "WATERFALLHISTORYDEFAULT", WaterfallHistoryDefault);
	writeXMLbool(f, "WATERFALLQSY", WaterfallQSY);
	writeXMLstr(f, "WATERFALLCLICKTEXT", WaterfallClickText);
	writeXMLint(f, "WATERFALLWHEELACTION", WaterfallWheelAction);
	writeXMLbool(f, "STARTATSWEETSPOT", StartAtSweetSpot);
	writeXMLbool(f, "PSKMAILSWEETSPOT", PSKmailSweetSpot);
	writeXMLint(f, "PSKSEARCHRANGE", SearchRange);
	writeXMLint(f, "PSKSERVEROFFSET", ServerOffset);
	writeXMLdbl(f, "CWSWEETSPOT", CWsweetspot);
	writeXMLdbl(f, "PSKSWEETSPOT", PSKsweetspot);
	writeXMLdbl(f, "ACQSN", ACQsn);
	writeXMLdbl(f, "RTTYSWEETSPOT", RTTYsweetspot);
	writeXMLint(f, "RTTYSHIFT", rtty_shift);
	writeXMLint(f, "RTTYBAUD", rtty_baud);
	writeXMLint(f, "RTTYBITS", rtty_bits);
	writeXMLint(f, "RTTYPARITY", rtty_parity);
	writeXMLint(f, "RTTYSTOP", rtty_stop);
	writeXMLbool(f, "RTTYREVERSE", rtty_reverse);
	writeXMLbool(f, "RTTYMSBFIRST", rtty_msbfirst);
	writeXMLbool(f, "RTTYCRCLF", rtty_crcrlf);
	writeXMLbool(f, "RTTYAUTOCRLF", rtty_autocrlf);
	writeXMLint(f, "RTTYAUTOCOUNT", rtty_autocount);
	writeXMLint(f, "RTTYAFCSPEED", rtty_afcspeed);
//	writeXMLbool(f, "RTTYUSB", RTTY_USB);
	writeXMLbool(f, "PREFERXHAIRSCOPE", PreferXhairScope);
	writeXMLbool(f, "PSEUDOFSK", PseudoFSK);
	writeXMLbool(f, "UOSRX", UOSrx);
	writeXMLbool(f, "UOSTX", UOStx);
	writeXMLbool(f, "XAGC", Xagc);

	writeXMLint(f, "CWWEIGHT", CWweight);	
	writeXMLint(f, "CWSPEED", CWspeed);
	writeXMLint(f, "CWDEFSPEED", defCWspeed);
	writeXMLint(f, "CWBANDWIDTH", CWbandwidth);
	writeXMLint(f, "CWRANGE", CWrange);
	writeXMLint(f, "CWLOWERLIMIT", CWlowerlimit);
	writeXMLint(f, "CWUPPERLIMIT", CWupperlimit);
	writeXMLbool(f, "CWTRACK", CWtrack);
	writeXMLdbl(f, "CWRISETIME", CWrisetime);
	writeXMLdbl(f, "CWDASH2DOT", CWdash2dot);
	writeXMLbool(f, "QSK", QSK);
	writeXMLdbl(f, "CWPRE", CWpre);
	writeXMLdbl(f, "CWPOST", CWpost);
	writeXMLbool(f, "CWID", CWid);
	writeXMLint(f, "IDWPM", CWIDwpm);
	
	writeXMLint(f, "OLIVIATONES", oliviatones);
	writeXMLint(f, "OLIVIABW", oliviabw);
	writeXMLint(f, "OLIVIASMARGIN", oliviasmargin);
	writeXMLint(f, "OLIVIASINTEG", oliviasinteg);
	writeXMLbool(f, "OLIVIA8BIT", olivia8bit);
	
	writeXMLdbl(f,  "THORBW", THOR_BW);
	writeXMLbool(f, "THORFILTER", THOR_FILTER);
	writeXMLstr(f,  "THORSECTEXT", THORsecText);		
	writeXMLint(f, "THORPATHS", THOR_PATHS);
	writeXMLbool(f, "THORSOFT", THOR_SOFT);
	writeXMLdbl(f, "THORCWI", ThorCWI);
	
	writeXMLdbl(f, "DOMINOEXBW", DOMINOEX_BW);
	writeXMLbool(f, "DOMINOEXFILTER", DOMINOEX_FILTER);
	writeXMLbool(f, "DOMINOEXFEC", DOMINOEX_FEC);
	writeXMLint(f, "DOMINOEXPATHS", DOMINOEX_PATHS);
	writeXMLdbl(f, "DOMCWI", DomCWI);
	
	writeXMLint(f, "FELDFONTNBR", feldfontnbr);
	writeXMLbool(f, "HELLRCVWIDTH", HellRcvWidth);
	writeXMLint(f, "HELLXMTWIDTH", HellXmtWidth);
	writeXMLbool(f, "HELLBLACKBOARD", HellBlackboard);
	writeXMLbool(f, "HELLPULSEFAST", HellPulseFast);
	writeXMLbool(f, "HELLXMTIDLE", HellXmtIdle);

	writeXMLint(f, "WFPREFILTER", wfPreFilter);
	writeXMLint(f, "LATENCY", latency);
	writeXMLbool(f, "USECURSORLINES", UseCursorLines);
	writeXMLbool(f, "USECURSORCENTERLINE", UseCursorCenterLine);
	writeXMLbool(f, "USEBWTRACKS", UseBWTracks);
	writeXMLrgb(f, "CLCOLORS", 
		cursorLineRGBI.R,
		cursorLineRGBI.G,
		cursorLineRGBI.B);	
	writeXMLrgb(f, "CCCOLORS", 
		cursorCenterRGBI.R,
		cursorCenterRGBI.G,
		cursorCenterRGBI.B);
	writeXMLrgb(f, "BWTCOLORS",
		bwTrackRGBI.R,
		bwTrackRGBI.G,
		bwTrackRGBI.B);	
	writeXMLbool(f, "VIEWXMTSIGNAL", viewXmtSignal);
	writeXMLbool(f, "SENDID", sendid);
	writeXMLbool(f, "MACROID", macroid);
	writeXMLbool(f, "SENDTEXTID", sendtextid);
	writeXMLstr(f, "STRTEXTID", strTextid);
	writeXMLint(f, "VIDEOWIDTH", videowidth);
	writeXMLbool(f, "IDSMALL", ID_SMALL);
	writeXMLint(f, "QRZTYPE", QRZ);
	writeXMLstr(f, "QRZPATHNAME", QRZpathname);
	writeXMLstr(f, "QRZUSER", QRZusername);
	writeXMLstr(f, "QRZPASSWORD", QRZuserpassword);
	writeXMLbool(f, "BTNUSB", btnusb);
	writeXMLint(f, "BTNPTTIS", btnPTTis);
	writeXMLbool(f, "RTSPTT", RTSptt);
	writeXMLbool(f, "DTRPTT", DTRptt);
	writeXMLbool(f, "RTSPLUS", RTSplus);
	writeXMLbool(f, "DTRPLUS", DTRplus);
	writeXMLint(f, "CHOICEHAMLIBIS", choiceHAMLIBis);	
	writeXMLint(f, "CHKUSEMEMMAPIS", chkUSEMEMMAPis);
	writeXMLint(f, "CHKUSEHAMLIBIS", chkUSEHAMLIBis);
	writeXMLint(f, "CHKUSERIGCATIS", chkUSERIGCATis);
	writeXMLint(f, "CHKUSEXMLRPCIS", chkUSEXMLRPCis);	
	writeXMLstr(f, "HAMRIGNAME", HamRigName);
	writeXMLstr(f, "HAMRIGDEVICE", HamRigDevice);
	writeXMLint(f, "HAMRIGBAUDRATE", HamRigBaudrate);

	writeXMLstr(f, "PTTDEV", PTTdev);
	writeXMLstr(f, "SECONDARYTEXT", secText);		
	writeXMLint(f, "AUDIOIO", btnAudioIOis);
	writeXMLstr(f, "OSSDEVICE", OSSdevice);
	writeXMLstr(f, "PADEVICE", PAdevice);
	writeXMLstr(f, "PORTINDEVICE", PortInDevice);
	writeXMLint(f, "PORTININDEX", PortInIndex);
	writeXMLstr(f, "PORTOUTDEVICE", PortOutDevice);
	writeXMLint(f, "PORTOUTINDEX", PortOutIndex);
	writeXMLstr(f, "PULSESERVER", PulseServer);
	writeXMLint(f, "SAMPLERATE", sample_rate);
	writeXMLint(f, "INSAMPLERATE", in_sample_rate);
	writeXMLint(f, "OUTSAMPLERATE", out_sample_rate);
	writeXMLint(f, "SAMPLECONVERTER", sample_converter);
	writeXMLint(f, "RXCORR", RX_corr);		
	writeXMLint(f, "TXCORR", TX_corr);
	writeXMLint(f, "TXOFFSET", TxOffset);
	writeXMLbool(f, "USELEADINGZEROS", UseLeadingZeros);
	writeXMLint(f, "CONTESTSTART", ContestStart);
	writeXMLint(f, "CONTESTDIGITS", ContestDigits);
	writeXMLbool(f, "USETIMER", useTimer);
	writeXMLint(f, "MACRONUMBER", macronumber);
	writeXMLint(f, "TIMEOUT", timeout);	
	writeXMLbool(f, "USELASTMACRO", UseLastMacro);
	writeXMLbool(f, "DISPLAYMACROFILENAME", DisplayMacroFilename);
	writeXMLstr(f, "MXDEVICE", MXdevice);
	writeXMLdbl(f, "PCMVOLUME", PCMvolume);
	writeXMLbool(f, "MICIN", MicIn);
	writeXMLbool(f, "LINEIN", LineIn);
	writeXMLbool(f, "ENABLEMIXER", EnableMixer);
	writeXMLbool(f, "MUTEINPUT", MuteInput);
	for (int i = 0; i < 9; i++)
		writeXMLPalette(f, i, cfgpal[i].R, cfgpal[i].G, cfgpal[i].B);

	writeXMLbool(f, "VIEWERMARQUEE", VIEWERmarquee);
	writeXMLbool(f, "VIEWERSHOWFREQ", VIEWERshowfreq);
	writeXMLint(f, "VIEWERSTART", VIEWERstart);
	writeXMLint(f, "VIEWERCHANNELS", VIEWERchannels);
	writeXMLdbl(f, "VIEWERSQUELCH", VIEWERsquelch);
	writeXMLint(f, "VIEWERTIMEOUT", VIEWERtimeout);
	writeXMLbool(f,"WFAVERAGING", WFaveraging);

	writeXMLbool(f,"USEGROUPCOLORS", useGroupColors);
	writeXMLrgb(f, "FKEYGROUP1", btnGroup1.R, btnGroup1.G, btnGroup1.B);
	writeXMLrgb(f, "FKEYGROUP2", btnGroup2.R, btnGroup2.G, btnGroup2.B);
	writeXMLrgb(f, "FKEYGROUP3", btnGroup3.R, btnGroup3.G, btnGroup3.B);
	writeXMLrgb(f, "FKEYTEXTCOLOR", 
		btnFkeyTextColor.R, btnFkeyTextColor.G, btnFkeyTextColor.B);
	
	writeXMLint(f, "RXFONTNBR", (int)RxFontnbr);
	writeXMLint(f, "RXFONTSIZE", RxFontsize);
	writeXMLint(f, "RXFNTCOLOR", (int)RxFontcolor);
	writeXMLint(f, "TXFONTNBR", (int)TxFontnbr);
	writeXMLint(f, "TXFONTSIZE", TxFontsize);
	writeXMLint(f, "TXFNTCOLOR", (int)TxFontcolor);
	writeXMLint(f, "XMITCOLOR", (int)XMITcolor);
	writeXMLint(f, "CTRLCOLOR", (int)CTRLcolor);
	writeXMLint(f, "SKIPCOLOR", (int)SKIPcolor);
	writeXMLint(f, "ALTRCOLOR", (int)ALTRcolor);

	writeXMLrgb(f, "RXFONTCOLOR", RxColor.R, RxColor.G, RxColor.B);
	writeXMLrgb(f, "TXFONTCOLOR", TxColor.R, TxColor.G, TxColor.B);
	writeXMLint(f, "WATERFALLFONTNBR", (int)WaterfallFontnbr);
	writeXMLint(f, "WATERFALLFONTSIZE", WaterfallFontsize);

	writeXMLstr(f, "UISCHEME", ui_scheme);

	writeXMLbool(f, "RSIDWIDESEARCH", rsidWideSearch);
	writeXMLbool(f, "TRANSMITRSID", TransmitRSid);
	writeXMLbool(f, "SLOWCPU", slowcpu);
	
	writeXMLbool(f, "MT638BIT", mt63_8bit);
	writeXMLint(f, "MT63INTERLEAVE", mt63_interleave);
	
	f << "</FLDIGI_DEFS>\n";
	f.close();
}
	
bool configuration::readDefaultsXML()
{
	string deffname = HomeDir;
	deffname.append("fldigi_def.xml");
	ifstream f_in(deffname.c_str(), ios::in);

	if (!f_in) return false;
	string xmlpage;
	char str[255];
	while (f_in) {
		f_in.getline(str, 255);
		xmlpage += str;
		xmlpage += '\n';
	}
	f_in.close();
		
	IrrXMLReader* xml = createIrrXMLReader(new IIrrXMLStringReader(xmlpage));

// strings for storing the data we want to get out of the file
	TAG tag = IGNORE;
	
// parse the file until end reached
	while(xml && xml->read()) {
		switch(xml->getNodeType()) {
			case EXN_TEXT:
			case EXN_CDATA:
				switch (tag) {
					default:
					case IGNORE:
						break;
					case MYCALL :
						myCall = getstring(xml);
						break;
					case MYNAME:
						myName = getstring(xml);
						break;
					case MYQTH:
						myQth = getstring(xml);
						break;
					case MYLOC:
						myLocator = getstring(xml);
						break;
					case WFREFLEVEL:
						wfRefLevel = atof(getstring(xml).c_str());
						break;
					case WFAMPSPAN :
						wfAmpSpan = atof(getstring(xml).c_str());
						break;
					case LOWFREQCUTOFF :
						LowFreqCutoff = atoi(getstring(xml).c_str());
						break;
					case WATERFALLHISTORYDEFAULT :
						WaterfallHistoryDefault = atoi(getstring(xml).c_str());
						break;
					case WATERFALLQSY :
						WaterfallQSY = atoi(getstring(xml).c_str());
						break;
					case WATERFALLCLICKTEXT :
						WaterfallClickText = getstring(xml);
						break;
					case WATERFALLWHEELACTION :
						WaterfallWheelAction = atoi(getstring(xml).c_str());
						break;
					case STARTATSWEETSPOT :
						StartAtSweetSpot = atoi(getstring(xml).c_str());
						break;
					case PSKMAILSWEETSPOT :
						PSKmailSweetSpot = atoi(getstring(xml).c_str());
						break;
					case PSKSEARCHRANGE :
						SearchRange = atoi(getstring(xml).c_str());
						break;
					case PSKSERVEROFFSET :
						ServerOffset = atoi(getstring(xml).c_str());
						break;
					case ACQSN :
						ACQsn = atof(getstring(xml).c_str());
						break;
					case CWSWEETSPOT :
						CWsweetspot = atof(getstring(xml).c_str());
						break;
					case PSKSWEETSPOT :
						PSKsweetspot = atof(getstring(xml).c_str());
						break;
					case RTTYSWEETSPOT :
						RTTYsweetspot = atof(getstring(xml).c_str());
						break;
					case RTTYSHIFT :
						rtty_shift = atoi(getstring(xml).c_str());
						break;
					case RTTYBAUD :
						rtty_baud = atoi(getstring(xml).c_str());
						break;
					case RTTYBITS :
						rtty_bits = atoi(getstring(xml).c_str());
						break;
					case RTTYPARITY :
						rtty_parity = atoi(getstring(xml).c_str());
						break;
					case RTTYSTOP :
						rtty_stop = atoi(getstring(xml).c_str());
						break;
					case RTTYREVERSE :
						rtty_reverse = atoi(getstring(xml).c_str());
						break;
					case RTTYMSBFIRST :
						rtty_msbfirst = atoi(getstring(xml).c_str());
						break;
					case RTTYCRCLF :
						rtty_crcrlf = atoi(getstring(xml).c_str());
						break;
					case RTTYAUTOCRLF :
						rtty_autocrlf = atoi(getstring(xml).c_str());
						break;
					case RTTYAUTOCOUNT :
						rtty_autocount = atoi(getstring(xml).c_str());
						break;
					case RTTYAFCSPEED :
						rtty_afcspeed = atoi(getstring(xml).c_str());
						break;
//					case RTTYUSB :
//						RTTY_USB = atoi(getstring(xml).c_str());
//						break;
					case PREFERXHAIRSCOPE :
						PreferXhairScope = atoi(getstring(xml).c_str());
						break;
					case PSEUDOFSK :
						PseudoFSK = atoi(getstring(xml).c_str());
						break;
					case UOSRX :
						UOSrx = atoi(getstring(xml).c_str());
						break;
					case UOSTX :
						UOStx = atoi(getstring(xml).c_str());
						break;
					case XAGC :
						Xagc = atoi(getstring(xml).c_str());
						break;
					case CWWEIGHT :
						CWweight = atoi(getstring(xml).c_str());
						break;
					case CWSPEED :
						CWspeed = atoi(getstring(xml).c_str());
						break;
					case CWDEFSPEED :
						defCWspeed = atoi(getstring(xml).c_str());
						break;
					case CWBANDWIDTH :
						CWbandwidth = atoi(getstring(xml).c_str());
						break;
					case CWRANGE :
						CWrange = atoi(getstring(xml).c_str());
						break;
					case CWLOWERLIMIT :
						CWlowerlimit = atoi(getstring(xml).c_str());
						break;
					case CWUPPERLIMIT :
						CWupperlimit = atoi(getstring(xml).c_str());
						break;
					case CWTRACK :
						CWtrack = atoi(getstring(xml).c_str());
						break;
					case CWRISETIME :
						CWrisetime = atof(getstring(xml).c_str());
						break;
					case CWDASH2DOT :
						CWdash2dot = atof(getstring(xml).c_str());
						break;
					case XQSK :
						QSK = atoi(getstring(xml).c_str());
						break;
					case CWPRE :
						CWpre = atof(getstring(xml).c_str());
						break;
					case CWPOST :
						CWpost = atof(getstring(xml).c_str());
						break;
					case CWID :
						CWid = atoi(getstring(xml).c_str());
						break;
					case CWIDWPM :
						CWIDwpm = atoi(getstring(xml).c_str());
						break;
					case OLIVIATONES :
						oliviatones = atoi(getstring(xml).c_str());
						break;
					case OLIVIABW :
						oliviabw = atoi(getstring(xml).c_str());
						break;
					case OLIVIASMARGIN :
						oliviasmargin = atoi(getstring(xml).c_str());
						break;
					case OLIVIASINTEG :
						oliviasinteg = atoi(getstring(xml).c_str());
						break;
					case OLIVIA8BIT :
						olivia8bit = atoi(getstring(xml).c_str());
						break;
					case THORBW :
						THOR_BW = atof(getstring(xml).c_str());
						break;
					case THORFILTER :
						THOR_FILTER = atoi(getstring(xml).c_str());
						break;
					case THORSECTEXT :
						THORsecText = getstring(xml);
						break;
					case THORPATHS :
						THOR_PATHS = atoi(getstring(xml).c_str());
						break;
					case THORSOFT :
						THOR_SOFT = atoi(getstring(xml).c_str());
						break;
					case THORCWI :
						ThorCWI = atof(getstring(xml).c_str());
						break;
					case DOMINOEXBW :
						DOMINOEX_BW = atof(getstring(xml).c_str());
						break;
					case DOMINOEXFILTER :
						DOMINOEX_FILTER = atoi(getstring(xml).c_str());
						break;
					case DOMINOEXFEC :
						DOMINOEX_FEC = atoi(getstring(xml).c_str());
						break;
					case DOMINOEXPATHS :
						DOMINOEX_PATHS = atoi(getstring(xml).c_str());
						break;
					case DOMCWI :
						DomCWI = atof(getstring(xml).c_str());
						break;
					case FELDFONTNBR :
						feldfontnbr = atoi(getstring(xml).c_str());
						break;
					case HELLRCVWIDTH :
						HellRcvWidth = atoi(getstring(xml).c_str());
						break;
					case HELLXMTWIDTH :
						HellXmtWidth = atoi(getstring(xml).c_str());
						if (HellXmtWidth == 0) HellXmtWidth = 1;
						break;
					case HELLBLACKBOARD :
						HellBlackboard = atoi(getstring(xml).c_str());
						break;
					case HELLPULSEFAST :
						HellPulseFast = atoi(getstring(xml).c_str());
						break;
					case HELLXMTIDLE :
						HellXmtIdle = atoi(getstring(xml).c_str());
						break;
					case WFPREFILTER :
						wfPreFilter = atoi(getstring(xml).c_str());
						break;
					case LATENCY :
						latency = atoi(getstring(xml).c_str());
						break;
					case USECURSORLINES :
						UseCursorLines = atoi(getstring(xml).c_str());
						break;
					case USECURSORCENTERLINE :
						UseCursorCenterLine = atoi(getstring(xml).c_str());
						break;
					case USEBWTRACKS :
						UseBWTracks = atoi(getstring(xml).c_str());
						break;
					case CLCOLORS :
						sscanf( getstring(xml).c_str(), "%hhu %hhu %hhu",
							&cursorLineRGBI.R,
							&cursorLineRGBI.G,
							&cursorLineRGBI.B );	
						break;
					case CCCOLORS :
						sscanf( getstring(xml).c_str(), "%hhu %hhu %hhu",
							&cursorCenterRGBI.R,
							&cursorCenterRGBI.G,
							&cursorCenterRGBI.B );	
						break;
					case BWTCOLORS :
						sscanf( getstring(xml).c_str(), "%hhu %hhu %hhu",
							&bwTrackRGBI.R,
							&bwTrackRGBI.G,
							&bwTrackRGBI.B );	
						break;
					case VIEWXMTSIGNAL :
						viewXmtSignal = atoi(getstring(xml).c_str());
						break;
					case SENDID :
						sendid = atoi(getstring(xml).c_str());
						break;
					case MACROID :
						macroid = atoi(getstring(xml).c_str());
						break;
					case SENDTEXTID :
						sendtextid = atoi(getstring(xml).c_str());
						break;
					case STRTEXTID :
						strTextid = getstring(xml);
					case VIDEOWIDTH :
						videowidth = atoi(getstring(xml).c_str());
					case IDSMALL :
						ID_SMALL = atoi(getstring(xml).c_str());
					case QRZTYPE :
						QRZ = atoi(getstring(xml).c_str());
						break;
					case QRZPATHNAME :
						QRZpathname = getstring(xml);
						break;
					case QRZUSER :
						QRZusername = getstring(xml);
						break;
					case QRZPASSWORD :
						QRZuserpassword = getstring(xml);
						break;
					case BTNUSB :
						btnusb = atoi(getstring(xml).c_str());
						break;
					case BTNPTTIS :
						btnPTTis = atoi(getstring(xml).c_str());
						break;
					case RTSPTT :
						RTSptt = atoi(getstring(xml).c_str());
						break;
					case DTRPTT :
						DTRptt = atoi(getstring(xml).c_str());
						break;
					case RTSPLUS :
						RTSplus = atoi(getstring(xml).c_str());
						break;
					case DTRPLUS :
						DTRplus = atoi(getstring(xml).c_str());
						break;
					case CHOICEHAMLIBIS :
						choiceHAMLIBis = atoi(getstring(xml).c_str());
						break;
					case CHKUSEMEMMAPIS :
						chkUSEMEMMAPis = atoi(getstring(xml).c_str());
						break;
					case CHKUSEHAMLIBIS :
						chkUSEHAMLIBis = atoi(getstring(xml).c_str());
						break;
					case CHKUSERIGCATIS :
						chkUSERIGCATis = atoi(getstring(xml).c_str());
						break;
					case CHKUSEXMLRPCIS :
						chkUSEXMLRPCis = atoi(getstring(xml).c_str());
						break;
					case HAMRIGNAME :
						HamRigName = getstring(xml);
						break;
					case HAMRIGDEVICE :
						HamRigDevice = getstring(xml);
						break;
					case HAMRIGBAUDRATE :
						HamRigBaudrate = atoi(getstring(xml).c_str());
						break;
					case PTTDEV :
						PTTdev = getstring(xml);
						break;
					case SECONDARYTEXT :
						secText = getstring(xml);
						break;
					case AUDIOIO :
						btnAudioIOis = atoi(getstring(xml).c_str());
						break;
					case OSSDEVICE :
						OSSdevice = getstring(xml);
						break;
					case PADEVICE :
						PAdevice = getstring(xml);
						break;
					case PORTINDEVICE :
						PortInDevice = getstring(xml);
						break;
					case PORTININDEX :
						PortInIndex = atoi(getstring(xml).c_str());
						break;
					case PORTOUTDEVICE :
						PortOutDevice = getstring(xml);
						break;
					case PORTOUTINDEX :
						PortOutIndex = atoi(getstring(xml).c_str());
						break;
					case PULSESERVER :
						PulseServer = getstring(xml);
						break;
					case SAMPLERATE :
						sample_rate = atoi(getstring(xml).c_str());
						break;
					case INSAMPLERATE :
						in_sample_rate = atoi(getstring(xml).c_str());
						break;
					case OUTSAMPLERATE :
						out_sample_rate = atoi(getstring(xml).c_str());
						break;
					case SAMPLECONVERTER :
						sample_converter = atoi(getstring(xml).c_str());
						break;
					case RXCORR :
						RX_corr = atoi(getstring(xml).c_str());
						break;
					case TXCORR :
						TX_corr = atoi(getstring(xml).c_str());
						break;
					case TXOFFSET :
						TxOffset = atoi(getstring(xml).c_str());
						break;
					case USELEADINGZEROS :
						UseLeadingZeros = atoi(getstring(xml).c_str());
						break;
					case CONTESTSTART :
						ContestStart = atoi(getstring(xml).c_str());
						break;
					case CONTESTDIGITS :
						ContestDigits = atoi(getstring(xml).c_str());
						break;
					case USETIMER :
						useTimer = atoi(getstring(xml).c_str());
						break;
					case MACRONUMBER :
						macronumber = atoi(getstring(xml).c_str());
						break;
					case TIMEOUT :
						timeout = atoi(getstring(xml).c_str());
						break;
					case USELASTMACRO :
						UseLastMacro = atoi(getstring(xml).c_str());
						break;
					case DISPLAYMACROFILENAME :
						DisplayMacroFilename = atoi(getstring(xml).c_str());
						break;
					case MXDEVICE :
						MXdevice = getstring(xml);
						break;
					case PCMVOLUME :
						PCMvolume = atof(getstring(xml).c_str());
						break;
					case MICIN :
						MicIn = atoi(getstring(xml).c_str());
						break;
					case LINEIN :
						LineIn = atoi(getstring(xml).c_str());
						break;
					case ENABLEMIXER :
						EnableMixer = atoi(getstring(xml).c_str());
						break;
					case MUTEINPUT :
						MuteInput = atoi(getstring(xml).c_str());
						break;
					case PALETTE0 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[0].R, &cfgpal[0].G, &cfgpal[0].B );
						break;
					case PALETTE1 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[1].R, &cfgpal[1].G, &cfgpal[1].B );
						break;
					case PALETTE2 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[2].R, &cfgpal[2].G, &cfgpal[2].B );
						break;
					case PALETTE3 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[3].R, &cfgpal[3].G, &cfgpal[3].B );
						break;
					case PALETTE4 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[4].R, &cfgpal[4].G, &cfgpal[4].B );
						break;
					case PALETTE5 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[5].R, &cfgpal[5].G, &cfgpal[5].B );
						break;
					case PALETTE6 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[6].R, &cfgpal[6].G, &cfgpal[6].B );
						break;
					case PALETTE7 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[7].R, &cfgpal[7].G, &cfgpal[7].B );
						break;
					case PALETTE8 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
								&cfgpal[8].R, &cfgpal[8].G, &cfgpal[8].B );
						break;
					case VIEWERMARQUEE :
						VIEWERmarquee = atoi(getstring(xml).c_str());
						break;
					case VIEWERSHOWFREQ :
						VIEWERshowfreq = atoi(getstring(xml).c_str());
						break;
					case VIEWERSTART :
						VIEWERstart = atoi(getstring(xml).c_str());
						break;
					case VIEWERCHANNELS :
						VIEWERchannels = atoi(getstring(xml).c_str());
						break;
					case VIEWERSQUELCH :
						VIEWERsquelch = atof(getstring(xml).c_str());
						break;
					case VIEWERTIMEOUT :
						VIEWERtimeout = atoi(getstring(xml).c_str());
						break;
					case WFAVERAGING :
						WFaveraging = atoi(getstring(xml).c_str());
						break;
					case USEGROUPCOLORS :
						useGroupColors = atoi(getstring(xml).c_str());
					case FKEYGROUP1 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&btnGroup1.R, &btnGroup1.G, &btnGroup1.B);
						break;
					case FKEYGROUP2 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&btnGroup2.R, &btnGroup2.G, &btnGroup2.B);
						break;
					case FKEYGROUP3 :
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&btnGroup3.R, &btnGroup3.G, &btnGroup3.B);
						break;
					case FKEYTEXTCOLOR : 
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&btnFkeyTextColor.R, 
							&btnFkeyTextColor.G, 
							&btnFkeyTextColor.B);
						break;
					case RXFONTNBR :
						RxFontnbr = (Fl_Font)atoi(getstring(xml).c_str());
						break;
					case RXFONTSIZE :
						RxFontsize = atoi(getstring(xml).c_str());
						break;
					case RXFNTCOLOR :
						RxFontcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case TXFONTNBR :
						TxFontnbr = (Fl_Font)atoi(getstring(xml).c_str());
						break;
					case TXFONTSIZE :
						TxFontsize = atoi(getstring(xml).c_str());
						break;
					case TXFNTCOLOR :
						TxFontcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case RXFONTCOLOR :
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&RxColor.R, &RxColor.G, &RxColor.B);
						break;
					case XMITCOLOR :
						XMITcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case CTRLCOLOR :
						CTRLcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case SKIPCOLOR :
						SKIPcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case ALTRCOLOR :
						ALTRcolor = (Fl_Color)atoi(getstring(xml).c_str());
						break;
					case WATERFALLFONTNBR :
						WaterfallFontnbr = (Fl_Font)atoi(getstring(xml).c_str());
						break;
					case WATERFALLFONTSIZE :
						WaterfallFontsize = atoi(getstring(xml).c_str());
						break;
					case UISCHEME :
						ui_scheme = getstring(xml);
						break;
					case TXFONTCOLOR :
						sscanf( getstring(xml).c_str(), "%d %d %d",
							&TxColor.R, &TxColor.G, &TxColor.B);
						break;
					case RSIDWIDESEARCH :
						rsidWideSearch = atoi(getstring(xml).c_str());
						break;
					case TRANSMITRSID :
						TransmitRSid = atoi(getstring(xml).c_str());
						break;
					case SLOWCPU :
						slowcpu = atoi(getstring(xml).c_str());
						break;
					case MT638BIT :
						mt63_8bit = atoi(getstring(xml).c_str());
						break;
					case MT63INTERLEAVE :
						mt63_interleave = atoi(getstring(xml).c_str());
						break;
				}
				break;
				
			case EXN_ELEMENT_END:
				tag=IGNORE;
				break;

			case EXN_ELEMENT: 
				{
				const char *nodeName = xml->getNodeName();
				if (!strcmp("MYCALL", nodeName)) 		tag = MYCALL;
				else if (!strcmp("MYNAME", nodeName)) 	tag = MYNAME;
				else if (!strcmp("MYQTH", nodeName)) 	tag = MYQTH;
				else if (!strcmp("MYLOC", nodeName)) 	tag = MYLOC;
				else if (!strcmp("SQUELCH", nodeName)) 	tag = SQUELCH;
				else if (!strcmp("WFREFLEVEL", nodeName)) 	tag = WFREFLEVEL;
				else if (!strcmp("WFAMPSPAN", nodeName)) 	tag = WFAMPSPAN;
				else if (!strcmp("LOWFREQCUTOFF", nodeName)) 	tag = LOWFREQCUTOFF;
				else if (!strcmp("WATERFALLHISTORYDEFAULT", nodeName)) 	tag = WATERFALLHISTORYDEFAULT;
				else if (!strcmp("WATERFALLQSY", nodeName)) 	tag = WATERFALLQSY;
				else if (!strcmp("WATERFALLCLICKTEXT", nodeName)) 	tag = WATERFALLCLICKTEXT;
				else if (!strcmp("WATERFALLWHEELACTION", nodeName)) 	tag = WATERFALLWHEELACTION;
				else if (!strcmp("STARTATSWEETSPOT", nodeName)) 	tag = STARTATSWEETSPOT;
				else if (!strcmp("PSKMAILSWEETSPOT", nodeName)) 	tag = PSKMAILSWEETSPOT;
				else if (!strcmp("PSKSEARCHRANGE", nodeName)) 	tag = PSKSEARCHRANGE;
				else if (!strcmp("PSKSERVEROFFSET", nodeName)) 	tag = PSKSERVEROFFSET;
				else if (!strcmp("ACQSN", nodeName)) tag = ACQSN;
				else if (!strcmp("CWSWEETSPOT", nodeName)) 	tag = CWSWEETSPOT;
				else if (!strcmp("PSKSWEETSPOT", nodeName)) 	tag = PSKSWEETSPOT;
				else if (!strcmp("RTTYSWEETSPOT", nodeName)) 	tag = RTTYSWEETSPOT;
				else if (!strcmp("RTTYSHIFT", nodeName)) 	tag = RTTYSHIFT;
				else if (!strcmp("RTTYBAUD", nodeName)) 	tag = RTTYBAUD;
				else if (!strcmp("RTTYBITS", nodeName)) 	tag = RTTYBITS;
				else if (!strcmp("RTTYPARITY", nodeName)) 	tag = RTTYPARITY;
				else if (!strcmp("RTTYSTOP", nodeName)) 	tag = RTTYSTOP;
				else if (!strcmp("RTTYREVERSE", nodeName)) 	tag = RTTYREVERSE;
				else if (!strcmp("RTTYMSBFIRST", nodeName)) 	tag = RTTYMSBFIRST;
				else if (!strcmp("RTTYCRCLF", nodeName)) 	tag = RTTYCRCLF;
				else if (!strcmp("RTTYAUTOCRLF", nodeName)) 	tag = RTTYAUTOCRLF;
				else if (!strcmp("RTTYAUTOCOUNT", nodeName)) 	tag = RTTYAUTOCOUNT;
				else if (!strcmp("RTTYAFCSPEED", nodeName)) 	tag = RTTYAFCSPEED;
				else if (!strcmp("RTTYUSB", nodeName))		tag = RTTYUSB;
				else if (!strcmp("PREFERXHAIRSCOPE", nodeName)) 	tag = PREFERXHAIRSCOPE;
				else if (!strcmp("PSEUDOFSK", nodeName)) 	tag = PSEUDOFSK;
				else if (!strcmp("UOSRX", nodeName)) 	tag = UOSRX;
				else if (!strcmp("UOSTX", nodeName)) 	tag = UOSTX;
				else if (!strcmp("XAGC", nodeName)) 	tag = XAGC;
				else if (!strcmp("CWWEIGHT", nodeName)) 	tag = CWWEIGHT;
				else if (!strcmp("CWSPEED", nodeName)) 	tag = CWSPEED;
				else if (!strcmp("CWDEFSPEED", nodeName)) 	tag = CWDEFSPEED;
				else if (!strcmp("CWBANDWIDTH", nodeName)) 	tag = CWBANDWIDTH;
				else if (!strcmp("CWRANGE", nodeName)) 	tag = CWRANGE;
				else if (!strcmp("CWLOWERLIMIT", nodeName)) 	tag = CWLOWERLIMIT;
				else if (!strcmp("CWUPPERLIMIT", nodeName)) 	tag = CWUPPERLIMIT;
				else if (!strcmp("CWTRACK", nodeName)) 	tag = CWTRACK;
				else if (!strcmp("CWRISETIME", nodeName)) 	tag = CWRISETIME;
				else if (!strcmp("CWDASH2DOT", nodeName)) 	tag = CWDASH2DOT;
				else if (!strcmp("QSK", nodeName)) 	tag = XQSK;
				else if (!strcmp("CWPRE", nodeName)) 	tag = CWPRE;
				else if (!strcmp("CWPOST", nodeName)) 	tag = CWPOST;
				else if (!strcmp("CWID", nodeName))	tag = CWID;
				else if (!strcmp("IDWPM", nodeName)) tag = CWIDWPM;
				else if (!strcmp("OLIVIATONES", nodeName)) 	tag = OLIVIATONES;
				else if (!strcmp("OLIVIABW", nodeName)) 	tag = OLIVIABW;
				else if (!strcmp("OLIVIASMARGIN", nodeName)) 	tag = OLIVIASMARGIN;
				else if (!strcmp("OLIVIASINTEG", nodeName)) 	tag = OLIVIASINTEG;
				else if (!strcmp("OLIVIA8BIT", nodeName)) 	tag = OLIVIA8BIT;
				else if (!strcmp("THORBW", nodeName)) 	tag = THORBW;
				else if (!strcmp("THORFILTER", nodeName))	tag = THORFILTER;
				else if (!strcmp("THORSECTEXT", nodeName))	tag = THORSECTEXT;
				else if (!strcmp("THORPATHS", nodeName)) tag = THORPATHS;
				else if (!strcmp("THORSOFT", nodeName)) tag = THORSOFT;
				else if (!strcmp("THORCWI", nodeName)) tag = THORCWI;
				else if (!strcmp("DOMINOEXBW", nodeName)) 	tag = DOMINOEXBW;
				else if (!strcmp("DOMINOEXFILTER", nodeName))	tag = DOMINOEXFILTER;
				else if (!strcmp("DOMINOEXFEC", nodeName))	tag = DOMINOEXFEC;
				else if (!strcmp("DOMINOEXPATHS", nodeName)) tag = DOMINOEXPATHS;
				else if (!strcmp("DOMCWI", nodeName)) tag = DOMCWI;
				else if (!strcmp("FELDFONTNBR", nodeName)) 	tag = FELDFONTNBR;
				else if (!strcmp("HELLRCVWIDTH", nodeName)) 	tag = HELLRCVWIDTH;
				else if (!strcmp("HELLXMTWIDTH", nodeName)) 	tag = HELLXMTWIDTH;
				else if (!strcmp("HELLBLACKBOARD", nodeName)) 	tag = HELLBLACKBOARD;
				else if (!strcmp("HELLPULSEFAST", nodeName)) 	tag = HELLPULSEFAST;
				else if (!strcmp("HELLXMTIDLE", nodeName)) 	tag = HELLXMTIDLE;
				else if (!strcmp("WFPREFILTER", nodeName)) 	tag = WFPREFILTER;
				else if (!strcmp("LATENCY", nodeName)) 	tag = LATENCY;
				else if (!strcmp("USECURSORLINES", nodeName)) 	tag = USECURSORLINES;
				else if (!strcmp("USECURSORCENTERLINE", nodeName)) 	tag = USECURSORCENTERLINE;
				else if (!strcmp("USEBWTRACKS", nodeName)) 	tag = USEBWTRACKS;
				else if (!strcmp("CLCOLORS", nodeName)) 	tag = CLCOLORS;
				else if (!strcmp("CCCOLORS", nodeName)) 	tag = CCCOLORS;
				else if (!strcmp("BWTCOLORS", nodeName)) 	tag = BWTCOLORS;
				else if (!strcmp("VIEWXMTSIGNAL", nodeName)) 	tag = VIEWXMTSIGNAL;
				else if (!strcmp("SENDID", nodeName)) 	tag = SENDID;
				else if (!strcmp("MACROID", nodeName)) 	tag = MACROID;
				else if (!strcmp("SENDTEXTID", nodeName))	tag = SENDTEXTID;
				else if (!strcmp("STRTEXTID", nodeName))	tag = STRTEXTID;
				else if (!strcmp("VIDEOWIDTH", nodeName))	tag = VIDEOWIDTH;
				else if (!strcmp("IDSMALL", nodeName))	tag = IDSMALL;
				else if (!strcmp("QRZUSER", nodeName)) 	tag = QRZUSER;
				else if (!strcmp("QRZPATHNAME", nodeName)) tag = QRZPATHNAME;
				else if (!strcmp("QRZPASSWORD", nodeName)) 	tag = QRZPASSWORD;
				else if (!strcmp("QRZTYPE", nodeName)) 	tag = QRZTYPE;
				else if (!strcmp("BTNUSB", nodeName)) 	tag = BTNUSB;
				else if (!strcmp("BTNPTTIS", nodeName)) 	tag = BTNPTTIS;
				else if (!strcmp("RTSPTT", nodeName)) 	tag = RTSPTT;
				else if (!strcmp("DTRPTT", nodeName)) 	tag = DTRPTT;
				else if (!strcmp("RTSPLUS", nodeName)) 	tag = RTSPLUS;
				else if (!strcmp("DTRPLUS", nodeName)) 	tag = DTRPLUS;
				else if (!strcmp("CHOICEHAMLIBIS", nodeName)) 	tag = CHOICEHAMLIBIS;
				else if (!strcmp("CHKUSEMEMMAPIS", nodeName)) 	tag = CHKUSEMEMMAPIS;
				else if (!strcmp("CHKUSEHAMLIBIS", nodeName)) 	tag = CHKUSEHAMLIBIS;
				else if (!strcmp("CHKUSERIGCATIS", nodeName)) 	tag = CHKUSERIGCATIS;
				else if (!strcmp("CHKUSEXMLRPCIS", nodeName)) 	tag = CHKUSEXMLRPCIS;
				else if (!strcmp("HAMRIGNAME", nodeName)) 	tag = HAMRIGNAME;
				else if (!strcmp("HAMRIGDEVICE", nodeName)) 	tag = HAMRIGDEVICE;
				else if (!strcmp("HAMRIGBAUDRATE", nodeName)) 	tag = HAMRIGBAUDRATE;
				else if (!strcmp("PTTDEV", nodeName)) 	tag = PTTDEV;
				else if (!strcmp("SECONDARYTEXT", nodeName)) 	tag = SECONDARYTEXT;
				else if (!strcmp("AUDIOIO", nodeName)) 	tag = AUDIOIO;
				else if (!strcmp("OSSDEVICE", nodeName)) 	tag = OSSDEVICE;
				else if (!strcmp("PADEVICE", nodeName)) 	tag = PADEVICE;
				else if (!strcmp("PORTINDEVICE", nodeName)) 	tag = PORTINDEVICE;
				else if (!strcmp("PORTININDEX", nodeName)) 	tag = PORTININDEX;
				else if (!strcmp("PORTOUTDEVICE", nodeName)) 	tag = PORTOUTDEVICE;
				else if (!strcmp("PORTOUTINDEX", nodeName)) 	tag = PORTOUTINDEX;
				else if (!strcmp("SAMPLERATE", nodeName)) 	tag = SAMPLERATE;
				else if (!strcmp("INSAMPLERATE", nodeName)) 	tag = INSAMPLERATE;
				else if (!strcmp("OUTSAMPLERATE", nodeName)) 	tag = OUTSAMPLERATE;
				else if (!strcmp("SAMPLECONVERTER", nodeName)) 	tag = SAMPLECONVERTER;
				else if (!strcmp("RXCORR", nodeName)) 	tag = RXCORR;
				else if (!strcmp("TXCORR", nodeName)) 	tag = TXCORR;
				else if (!strcmp("TXOFFSET", nodeName)) 	tag = TXOFFSET;
				else if (!strcmp("USELEADINGZEROS", nodeName)) 	tag = USELEADINGZEROS;
				else if (!strcmp("CONTESTSTART", nodeName)) 	tag = CONTESTSTART;
				else if (!strcmp("CONTESTDIGITS", nodeName)) 	tag = CONTESTDIGITS;
				else if (!strcmp("USETIMER", nodeName)) 	tag = USETIMER;
				else if (!strcmp("MACRONUMBER", nodeName)) 	tag = MACRONUMBER;
				else if (!strcmp("TIMEOUT", nodeName)) 	tag = TIMEOUT;
				else if (!strcmp("USELASTMACRO", nodeName)) 	tag = USELASTMACRO;
				else if (!strcmp("DISPLAYMACROFILENAME", nodeName)) 	tag = DISPLAYMACROFILENAME;
				else if (!strcmp("MXDEVICE", nodeName)) 	tag = MXDEVICE;
				else if (!strcmp("PCMVOLUME", nodeName)) 	tag = PCMVOLUME;
				else if (!strcmp("MICIN", nodeName)) 	tag = MICIN;
				else if (!strcmp("LINEIN", nodeName)) 	tag = LINEIN;
				else if (!strcmp("ENABLEMIXER", nodeName)) 	tag = ENABLEMIXER;
				else if (!strcmp("MUTEINPUT", nodeName)) 	tag = MUTEINPUT;
				else if (!strcmp("PALETTE0", nodeName)) 	tag = PALETTE0;
				else if (!strcmp("PALETTE1", nodeName)) 	tag = PALETTE1;
				else if (!strcmp("PALETTE2", nodeName)) 	tag = PALETTE2;
				else if (!strcmp("PALETTE3", nodeName)) 	tag = PALETTE3;
				else if (!strcmp("PALETTE4", nodeName)) 	tag = PALETTE4;
				else if (!strcmp("PALETTE5", nodeName)) 	tag = PALETTE5;
				else if (!strcmp("PALETTE6", nodeName)) 	tag = PALETTE6;
				else if (!strcmp("PALETTE7", nodeName)) 	tag = PALETTE7;
				else if (!strcmp("PALETTE8", nodeName)) 	tag = PALETTE8;
				else if (!strcmp("VIEWERMARQUEE", nodeName))	tag = VIEWERMARQUEE;
				else if (!strcmp("VIEWERSHOWFREQ", nodeName))	tag = VIEWERSHOWFREQ;
				else if (!strcmp("VIEWERSTART", nodeName))		tag = VIEWERSTART;
				else if (!strcmp("VIEWERCHANNELS", nodeName))	tag = VIEWERCHANNELS;
				else if (!strcmp("VIEWERSQUELCH", nodeName))	tag = VIEWERSQUELCH;
				else if (!strcmp("VIEWERTIMEOUT", nodeName))	tag = VIEWERTIMEOUT;
				else if (!strcmp("WFAVERAGING", nodeName))	tag = WFAVERAGING;
				else if (!strcmp("USEGROUPCOLORS", nodeName)) tag = USEGROUPCOLORS;
				else if (!strcmp("FKEYGROUP1", nodeName)) tag = FKEYGROUP1;
				else if (!strcmp("FKEYGROUP2", nodeName)) tag = FKEYGROUP2;
				else if (!strcmp("FKEYGROUP3", nodeName)) tag = FKEYGROUP3;
				else if (!strcmp("FKEYTEXTCOLOR", nodeName)) tag = FKEYTEXTCOLOR;
				else if (!strcmp("RXFONTNBR", nodeName)) tag = RXFONTNBR;
				else if (!strcmp("RXFONTSIZE", nodeName)) tag = RXFONTSIZE;
				else if (!strcmp("RXFNTCOLOR", nodeName)) tag = RXFNTCOLOR;
				else if (!strcmp("TXFONTNBR", nodeName)) tag = TXFONTNBR;
				else if (!strcmp("TXFONTSIZE", nodeName)) tag = TXFONTSIZE;
				else if (!strcmp("TXFNTCOLOR", nodeName)) tag = TXFNTCOLOR;
				else if (!strcmp("RXFONTCOLOR", nodeName)) tag = RXFONTCOLOR;
				else if (!strcmp("TXFONTCOLOR", nodeName)) tag = TXFONTCOLOR;
				else if (!strcmp("XMITCOLOR", nodeName)) tag = XMITCOLOR;
				else if (!strcmp("CTRLCOLOR", nodeName)) tag = CTRLCOLOR;
				else if (!strcmp("SKIPCOLOR", nodeName)) tag = SKIPCOLOR;
				else if (!strcmp("ALTRCOLOR", nodeName)) tag = ALTRCOLOR;
				else if (!strcmp("WATERFALLFONTNBR", nodeName)) tag = WATERFALLFONTNBR;
				else if (!strcmp("WATERFALLFONTSIZE", nodeName)) tag = WATERFALLFONTSIZE;
				else if (!strcmp("UISCHEME", nodeName)) tag = UISCHEME;
				else if (!strcmp("RSIDWIDESEARCH", nodeName)) tag = RSIDWIDESEARCH;
				else if (!strcmp("TRANSMITRSID", nodeName)) tag = TRANSMITRSID;
				else if (!strcmp("SLOWCPU", nodeName)) tag = SLOWCPU;
				else if (!strcmp("MT638BIT", nodeName)) tag = MT638BIT;
				else if (!strcmp("MT63INTERLEAVE", nodeName)) tag = MT63INTERLEAVE;
				else tag = IGNORE;
				}
				break;

			case EXN_NONE:
			case EXN_COMMENT:
			case EXN_UNKNOWN:
				break;
		}
	}
// delete the xml parser after usage
	delete xml;
	return true;
}

void configuration::loadDefaults() {
	FL_LOCK_D();
	
// RTTY
	selShift->value(rtty_shift);
	selBaud->value(rtty_baud);
	selBits->value(rtty_bits);
	selParity->value(rtty_parity);
//	chkMsbFirst->value(rtty_msbfirst);
	selStopBits->value(rtty_stop);
	btnCRCRLF->value(rtty_crcrlf);
	btnAUTOCRLF->value(rtty_autocrlf);
	cntrAUTOCRLF->value(rtty_autocount);
	chkPseudoFSK->value(PseudoFSK);
	chkUOSrx->value(UOSrx);
	chkUOStx->value(UOStx);
	chkXagc->value(Xagc);
	
	for (int i = 0; i < 3; i++)
		if (rtty_afcspeed == i)
			btnRTTYafc[i]->value(1);
		else
			btnRTTYafc[i]->value(0);
	btnPreferXhairScope->value(PreferXhairScope);
// OLIVIA
	mnuOlivia_Tones->value(oliviatones);
	mnuOlivia_Bandwidth->value(oliviabw);
	cntOlivia_smargin->value(oliviasmargin);
	cntOlivia_sinteg->value(oliviasinteg);
	btnOlivia_8bit->value(olivia8bit);

	chkDominoEX_FEC->value(DOMINOEX_FEC);

	btnmt63_interleave->value(mt63_interleave == 64);

	FL_UNLOCK_D();
}

void configuration::storeDefaults() { }

void configuration::saveDefaults() {
	FL_LOCK();
// strings
	myCall = inpMyCallsign->value();
	myName = inpMyName->value();
	myQth  = inpMyQth->value();
	myLocator = inpMyLocator->value();
	secText = txtSecondary->value();
	THORsecText = txtTHORSecondary->value();
	PTTdev = inpTTYdev->value();

	for (int i = 0; i < 9; i++) {
		cfgpal[i].R =  palette[i].R;
		cfgpal[i].G =  palette[i].G;
		cfgpal[i].B =  palette[i].B;
	}
	FL_UNLOCK();
	
	writeDefaultsXML();
	changed = false;
}

int configuration::setDefaults() {
#if USE_HAMLIB	
	getRigs();
#endif	
	FL_LOCK();
	inpMyCallsign->value(myCall.c_str());
	inpMyName->value(myName.c_str());
	inpMyQth->value(myQth.c_str());
	inpMyLocator->value(myLocator.c_str());
	UseLeadingZeros = btnUseLeadingZeros->value();
	ContestStart = (int)nbrContestStart->value();
	ContestDigits = (int)nbrContestDigits->value();
		
	txtSecondary->value(secText.c_str());

	txtTHORSecondary->value(THORsecText.c_str());
	valTHOR_BW->value(THOR_BW);
	valTHOR_FILTER->value(THOR_FILTER);
	valTHOR_PATHS->value(THOR_PATHS);
	valTHOR_SOFT->value(THOR_SOFT);
	valThorCWI->value(ThorCWI);
		
	valDominoEX_BW->value(DOMINOEX_BW);
	valDominoEX_FILTER->value(DOMINOEX_FILTER);
	chkDominoEX_FEC->value(DOMINOEX_FEC);
	valDominoEX_PATHS->value(DOMINOEX_PATHS);
	valDomCWI->value(DomCWI);
				
	for (int i = 0; i < 5; i++) {
		btnPTT[i]->value(0);
		btnPTT[i]->activate();
	}
	btnPTT[btnPTTis]->value(1);
#if !USE_HAMLIB
	btnPTT[1]->deactivate();
	chkUSEHAMLIB->deactivate();
    inpRIGdev->hide();
    mnuBaudRate->hide();
    cboHamlibRig->hide();
#else
    btnPTT[1]->activate();
	chkUSEHAMLIB->activate();
	inpRIGdev->show();
	mnuBaudRate->show();
    cboHamlibRig->show();
	cboHamlibRig->value(HamRigName.c_str());
#endif
	btnRTSptt->value(RTSptt);
	btnDTRptt->value(DTRptt);
	btnRTSplusV->value(RTSplus);
	btnDTRplusV->value(DTRplus);

	inpTTYdev->value(PTTdev.c_str());

	if(chkUSEMEMMAPis) {
		chkUSEMEMMAP->value(1); 
		chkUSEHAMLIB->value(0); chkUSERIGCAT->value(0); chkUSEXMLRPC->value(0);
		cboHamlibRig->deactivate();
		inpRIGdev->deactivate();
		mnuBaudRate->deactivate();
		btnPTT[1]->deactivate(); btnPTT[2]->activate(); btnPTT[3]->deactivate();
	} else if (chkUSEHAMLIBis) {
		chkUSEHAMLIB->value(1);
		chkUSEMEMMAP->value(0); chkUSERIGCAT->value(0);  chkUSEXMLRPC->value(0);
		cboHamlibRig->activate();
		inpRIGdev->activate();
		mnuBaudRate->activate();
		btnPTT[1]->activate(); btnPTT[2]->deactivate(); btnPTT[3]->deactivate();
	} else if (chkUSERIGCATis) {
		chkUSERIGCAT->value(1);
		chkUSEMEMMAP->value(0); chkUSEHAMLIB->value(0); chkUSEXMLRPC->value(0);
		cboHamlibRig->deactivate();
		inpRIGdev->deactivate();
		mnuBaudRate->deactivate();
		btnPTT[1]->deactivate(); btnPTT[2]->deactivate(); btnPTT[3]->activate();
	} else if (chkUSEXMLRPCis) {
		chkUSEXMLRPC->value(1);
		chkUSEMEMMAP->value(0); chkUSEHAMLIB->value(0); chkUSERIGCAT->value(0);
		cboHamlibRig->deactivate();
		inpRIGdev->deactivate();
		mnuBaudRate->deactivate();
		btnPTT[1]->deactivate(); btnPTT[2]->deactivate(); btnPTT[3]->deactivate();
	} else {
		chkUSEMEMMAP->value(0); chkUSEHAMLIB->value(0); 
		chkUSERIGCAT->value(0);	chkUSEHAMLIB->value(0); chkUSEXMLRPC->value(0);
		btnPTT[1]->deactivate(); btnPTT[2]->deactivate(); btnPTT[3]->deactivate();
	}

	inpRIGdev->value(HamRigDevice.c_str());
	mnuBaudRate->value(HamRigBaudrate);

	valCWsweetspot->value(CWsweetspot);
	valRTTYsweetspot->value(RTTYsweetspot);
	valPSKsweetspot->value(PSKsweetspot);
	btnWaterfallHistoryDefault->value(WaterfallHistoryDefault);
	btnWaterfallQSY->value(WaterfallQSY);
	inpWaterfallClickText->input_type(FL_MULTILINE_INPUT);
	inpWaterfallClickText->value(WaterfallClickText.c_str());

	mnuWaterfallWheelAction->add(waterfall::wf_wheel_action);
	mnuWaterfallWheelAction->value(WaterfallWheelAction);

	btnStartAtSweetSpot->value(StartAtSweetSpot);
	btnPSKmailSweetSpot->value(PSKmailSweetSpot);
	cntSearchRange->value(SearchRange);
	cntServerOffset->value(ServerOffset);
	cntACQsn->value(ACQsn);
			
	btnCursorBWcolor->color(
		fl_rgb_color(cursorLineRGBI.R, cursorLineRGBI.G, cursorLineRGBI.B) );
	btnCursorCenterLineColor->color(
		fl_rgb_color(cursorCenterRGBI.R, cursorCenterRGBI.G, cursorCenterRGBI.B) );
	btnBwTracksColor->color(
		fl_rgb_color(bwTrackRGBI.R, bwTrackRGBI.G, bwTrackRGBI.B) );
				
	cntCWweight->value(CWweight);
	sldrCWxmtWPM->value(CWspeed);
	cntCWdefWPM->value(defCWspeed);
	sldrCWbandwidth->value(CWbandwidth);
	btnCWrcvTrack->value(CWtrack);
	cntCWrange->value(CWrange);
	cntCWlowerlimit->value(CWlowerlimit);
	cntCWupperlimit->value(CWupperlimit);
	cntCWlowerlimit->maximum(CWupperlimit - 20);
	cntCWupperlimit->minimum(CWlowerlimit + 20);
	cntCWrisetime->value(CWrisetime);
	cntCWdash2dot->value(CWdash2dot);
	sldrCWxmtWPM->minimum(CWlowerlimit);
	sldrCWxmtWPM->maximum(CWupperlimit);
	btnQSK->value(QSK);
	cntPreTiming->maximum((int)(2400/CWspeed)/2.0); 
	cntPreTiming->value(CWpre);
	cntPostTiming->maximum((int)(2400/CWspeed)/2.0);
	cntPostTiming->value(CWpost);
	btnCWID->value(CWid);
			
	selHellFont->value(feldfontnbr);
	btnFeldHellIdle->value(HellXmtIdle);
			
	chkTransmitRSid->value(TransmitRSid);
	chkRSidWideSearch->value(rsidWideSearch);
	chkSlowCpu->value(slowcpu);
	
	string bandsfname = HomeDir;
	bandsfname.append("frequencies.def");
	ifstream bandsfile(bandsfname.c_str(), ios::in);
	if (bandsfile) {
		string sBand;
		cboBand->add(" ");
		while (!bandsfile.eof()) {
			sBand = "";
			bandsfile >> sBand; bandsfile.ignore();
			if (sBand.length() > 0)
				cboBand->add(sBand.c_str());
		}
		bandsfile.close();
	} else {
		int i = 0;
		while (szBands[i]) {
			cboBand->add((char *)szBands[i]);
			i++;
		}
	}
	btnQRZnotavailable->value(0);
	btnQRZsocket->value(0);
	btnQRZcdrom->value(0);
	btnHAMCALLsocket->value(0);
	if (QRZ == 0)
		btnQRZnotavailable->value(1);
	else if (QRZ == 1)
		btnQRZsocket->value(1);
	else if (QRZ == 2)
		btnQRZcdrom->value(1);
	else if (QRZ == 3)
		btnHAMCALLsocket->value(1);
	txtQRZpathname->value(QRZpathname.c_str());
			
//	btnRTTY_USB->value(RTTY_USB);
	btnsendid->value(sendid);
	btnsendvideotext->value(sendtextid);
	chkID_SMALL->value(ID_SMALL);
				
	FL_UNLOCK();

	wf->setPrefilter(wfPreFilter);
	valLatency->value(latency);
	btnWFaveraging->value(WFaveraging);
	
	for (int i = 0; i < 9; i++) {
		palette[i].R = (uchar)cfgpal[i].R;
		palette[i].G = (uchar)cfgpal[i].G;
		palette[i].B = (uchar)cfgpal[i].B;
	}
	wf->setcolors();
	setColorButtons();

	return 1;
}

void configuration::initOperator() {
	FL_LOCK();
		myCall = inpMyCallsign->value();
		myName = inpMyName->value();
		myQth  = inpMyQth->value();
		myLocator = inpMyLocator->value();
		UseLeadingZeros = btnUseLeadingZeros->value();
		ContestStart = (int)nbrContestStart->value();
		ContestDigits = (int)nbrContestDigits->value();
	FL_UNLOCK();
}

#include "rigio.h"

void configuration::initInterface() {
	initOperator();


// close down any possible rig interface threads
#if USE_HAMLIB
		hamlib_close();
		MilliSleep(100);
#endif
		rigMEM_close();
		MilliSleep(100);
		rigCAT_close();
		MilliSleep(100);

	FL_LOCK();
		btnPTTis = (btnPTT[0]->value() ? 0 :
					btnPTT[1]->value() ? 1 :
					btnPTT[2]->value() ? 2 :
					btnPTT[3]->value() ? 3 :
					btnPTT[4]->value() ? 4 : 0); // default is None
					
		RTSptt = btnRTSptt->value();
		DTRptt = btnDTRptt->value();
		RTSplus = btnRTSplusV->value();
		DTRplus = btnDTRplusV->value();
		
		PTTdev = inpTTYdev->value();

#if USE_HAMLIB
		chkUSEHAMLIBis = chkUSEHAMLIB->value();
#endif		
		chkUSEMEMMAPis = chkUSEMEMMAP->value();
		chkUSERIGCATis = chkUSERIGCAT->value();

#if USE_HAMLIB
		HamRigName = cboHamlibRig->value();
		HamRigDevice = inpRIGdev->value();
		HamRigBaudrate = mnuBaudRate->value();
#else
		cboHamlibRig->hide();
		inpRIGdev->hide();
		mnuBaudRate->hide();
#endif		
	FL_UNLOCK();
		
	if (chkUSEMEMMAPis) {// start the memory mapped i/o thread
		btnPTT[2]->activate();
		rigMEM_init();
		wf->setQSY(1);
		activate_rig_menu_item(false);
	} else if (chkUSERIGCATis) { // start the rigCAT thread
		if (rigCAT_init() == false) {
			wf->USB(true);
			cboBand->show();
			btnSideband->show();
			wf->rfcarrier(atoi(cboBand->value())*1000L);
			wf->setQSY(0);
			activate_rig_menu_item(false);
		} else {
			cboBand->hide();
			btnSideband->hide();
			wf->setQSY(1);
			activate_rig_menu_item(true);
		}
#if USE_HAMLIB
	} else if (chkUSEHAMLIBis) { // start the hamlib thread
		if (hamlib_init(btnPTTis == 1 ? true : false) == false) {
			wf->USB(true);
			cboBand->show();
			btnSideband->show();
			wf->rfcarrier(atoi(cboBand->value())*1000L);
			wf->setQSY(0);
			activate_rig_menu_item(false);
		} else {
			cboBand->hide();
			btnSideband->hide();
			wf->setQSY(1);
			activate_rig_menu_item(true);
		}
#endif		
	} else if (chkUSEXMLRPCis) {
		cboBand->hide();
		btnSideband->hide();
		wf->USB(true);
		wf->setXMLRPC(1);
		activate_rig_menu_item(false);
	} else {
		wf->USB(true);
		cboBand->show();
		btnSideband->show();
		wf->rfcarrier(atoi(cboBand->value())*1000L);
		wf->setQSY(0);
		activate_rig_menu_item(false);
	}
	
	push2talk->reset(btnPTTis);
	wf->setRefLevel();
	wf->setAmpSpan();
	cntLowFreqCutoff->value(LowFreqCutoff);
		
}

string configuration::strBaudRate()
{
	return (szBaudRates[HamRigBaudrate + 1]);
}

#if USE_HAMLIB
void configuration::getRigs() {
list<string>::iterator pstr;
	xcvr->get_rignames();
	pstr = (xcvr->rignames).begin();
FL_LOCK();
	while (pstr != (xcvr->rignames).end()) {
		cboHamlibRig->add((*pstr).c_str());
		++pstr;
	}
FL_UNLOCK();
}
#endif

void configuration::testCommPorts()
{
#ifndef PATH_MAX
#  define PATH_MAX 1024
#endif
#ifndef __CYGWIN__
	struct stat st;
#endif
#ifndef __APPLE__
	char ttyname[PATH_MAX + 1];
#endif

#ifdef __linux__
	bool ret = false;
	DIR* sys = NULL;
	char cwd[PATH_MAX] = { '.', '\0' };
	if (getcwd(cwd, sizeof(cwd)) == NULL || chdir("/sys/class/tty") == -1 ||
	    (sys = opendir(".")) == NULL)
		goto out;

	ssize_t len;
	struct dirent* dp;
	while ((dp = readdir(sys))) {
#  ifdef _DIRENT_HAVE_D_TYPE
		if (dp->d_type != DT_LNK)
			continue;
#  endif
		if ((len = readlink(dp->d_name, ttyname, sizeof(ttyname)-1)) == -1)
			continue;
		ttyname[len] = '\0';
		if (!strstr(ttyname, "/devices/virtual/")) {
			snprintf(ttyname, sizeof(ttyname), "/dev/%s", dp->d_name);
			if (stat(ttyname, &st) == -1 || !S_ISCHR(st.st_mode))
				continue;
			LOG_INFO("Found serial port %s", ttyname);
			inpTTYdev->add(ttyname);
#if USE_HAMLIB
			inpRIGdev->add(ttyname);
#endif
		}
	}
	ret = true;

out:
	if (sys)
		closedir(sys);
	chdir(cwd);
	if (ret) // do we need to fall back to the probe code below?
		return;
#endif // __linux__


	const char* tty_fmt[] = {
#if defined(__linux__)
		"/dev/ttyS%u",
		"/dev/ttyUSB%u",
		"/dev/usb/ttyUSB%u"
#elif defined(__FreeBSD__)
		"/dev/ttyd%u"
#elif defined(__CYGWIN__)
		"/dev/ttyS%u"
#elif defined(__APPLE__)
		"/dev/cu.*",
		"/dev/tty.*"
#endif
	};

#if defined(__CYGWIN__)
	int fd;
#  define TTY_MAX 255
#elif defined(__APPLE__)
	glob_t gbuf;
#else
#  define TTY_MAX 8
#endif

	for (size_t i = 0; i < sizeof(tty_fmt)/sizeof(*tty_fmt); i++) {
#ifndef __APPLE__
		for (unsigned j = 0; j < TTY_MAX; j++) {
			snprintf(ttyname, sizeof(ttyname), tty_fmt[i], j);
#  ifndef __CYGWIN__
			if ( !(stat(ttyname, &st) == 0 && S_ISCHR(st.st_mode)) )
				continue;
#  else // __CYGWIN__
			if ((fd = open(ttyname, O_RDWR | O_NOCTTY | O_NDELAY)) == -1)
				continue;
			snprintf(ttyname, sizeof(ttyname), "COM%u", j+1);
			close(fd);
#  endif // __CYGWIN__

			LOG_INFO("Found serial port %s", ttyname);
			inpTTYdev->add(ttyname);
#  if USE_HAMLIB
			inpRIGdev->add(ttyname);
#  endif
		}
#else // __APPLE__
		glob(tty_fmt[i], 0, NULL, &gbuf);
		for (size_t j = 0; j < gbuf.gl_pathc; j++) {
			if ( !(stat(gbuf.gl_pathv[j], &st) == 0 && S_ISCHR(st.st_mode)) ||
			     strstr(gbuf.gl_pathv[j], "modem") )
				continue;
			LOG_INFO("Found serial port %s", gbuf.gl_pathv[j]);
			inpTTYdev->add(gbuf.gl_pathv[j]);
#  if USE_HAMLIB
			inpRIGdev->add(gbuf.gl_pathv[j]);
#  endif

		}
		globfree(&gbuf);
#endif // __APPLE__
	}
}
