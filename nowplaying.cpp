#ifdef _WIN32
#include <windows.h>
#endif


#include "nowplaying.h"
#include <shellapi.h>
#include "song.h"
#include <fstream>
#include <iomanip>
#include <wininet.h>
#include <vector>

using namespace std;


Songstack allSongs;


//since i'm only using 1 extra thread, i have decided NOT to use dynamic memory 
//and just make this global, it's a cheap way out, but oh well
//This is my global allow to write bool, anything that deals with file i/o goes thought this bad boy
bool can_write = true;


int no_songs=0;
int GENERAL_HISTORY_int=1;

/* And create the WAComponentClient class */
static NowPlaying wac;
WAComponentClient *the = &wac;

/* Enable all the XML based options and set their default values */

//GENERAL STUFF
_bool	GENERAL_ENABLED("GENERAL ENABLED", TRUE);
_string GENERAL_FILE("GENERAL FILE", "C:/NowPlaying.html");
_int	GENERAL_HISTORY("GENERAL HISTORY",10);
_int	GENERAL_UPDATE("GENERAL UPDATE",1);
_string GENERAL_PATH("GENERAL PATH","");  //path to root mp3 files folder
_bool	GENERAL_STAY("GENERAL STAY",FALSE); //For this to be true GENERAL_PATH can't be ""
_bool	GENERAL_SINGLEQ("GENERAL SINGLEQ", FALSE); //' to &#39;
_bool	GENERAL_SAVE("GENERAL SAVE", FALSE);
//End GENERAL STUFF

//OUTPUT STUFF
//SONG STUFF
_bool	SONG_ARTIST("SONG ARTIST",TRUE);
_bool	SONG_TITLE("SONG TITLE",TRUE);
_bool	SONG_ALBUM("SONG ALBUM",FALSE);
_bool	SONG_GENRE("SONG GENRE",FALSE);
_bool	SONG_YEAR("SONG YEAR",FALSE);
_bool	SONG_COMMENT("SONG COMMENT",FALSE);
_bool	SONG_TRACK("SONG TRACK",FALSE);
_bool	SONG_LENGTH("SONG LENGTH",FALSE);

//FILE STUFF
_bool	FILE_RELATIVE("FILE RELATIVE",FALSE);
_bool	FILE_SIZE("FILE SIZE",FALSE);
_bool	FILE_KBPS("FILE KBPS",FALSE);
//_bool	FILE_CHANNELS("FILE CHANNELS",FALSE);
_bool	FILE_FREQUENCY("FILE FREQUENCY",FALSE);
_bool	FILE_QUALITY("FILE QUALITY",FALSE);
_bool	FILE_NAME("FILE NAME",FALSE);

//WINAMP STUFF
_bool	WINAMP_PLSIZE("WINAMP PLSIZE",FALSE);
_bool	WINAMP_POSITION("WINAMP POSITION",FALSE);
_bool	WINAMP_VERSION("WINAMP VERSION",FALSE);
_bool	WINAMP_HISTORY("WINAMP HISTORY", TRUE);
//END OUTPUT STUFF

//FTP STUFF
_bool	FTP_ENABLED("FTP ENABLED",FALSE);
_string FTP_SERVER("FTP SERVER","");
_string FTP_LOGON("FTP LOGON","");
_string FTP_PASSWORD("FTP PASSWORD","");
_int    FTP_PORT("FTP PORT",21);
_string FTP_FILENAME("FTP FILENAME","");
_string FTP_DIRECTORY("FTP DIRECTORY","");
_int	FTP_UPDATE("FTP UPDATE",4);
_bool	FTP_MT("FTP MT",TRUE);
//End FTP STUFF


//COMMAND STUFF
_bool	COMMAND_ENABLED("COMMAND ENABLED",FALSE);
_string COMMAND_PATH("COMMAND PATH","");
_string COMMAND_ARGUEMENTS("COMMAND ARGUEMENTS","");
_bool   COMMAND_HIDDEN("COMMAND HIDDEN",FALSE);
//end COMMAND STUFF

//TEMPLATE STUFF
_bool	TEMPLATE_ENABLED("TEMPLATE ENABLED",FALSE);
_string TEMPLATE_HEADER("TEMPLATE HEADER","");
_string TEMPLATE_DATA("TEMPLATE DATA","");
_string TEMPLATE_FOOTER("TEMPLATE FOOTER","");
_bool	TEMPLATE_HPARSE("TEMPLATE HPARSE",FALSE);
_bool	TEMPLATE_FPARSE("TEMPLATE FPARSE",FALSE);
//End TEMPLATE STUFF



// {3C59C05C-723F-49db-BCC4-757D2DEE8AEE}
static const GUID guid = 
{ 0x3c59c05c, 0x723f, 0x49db, { 0xbc, 0xc4, 0x75, 0x7d, 0x2d, 0xee, 0x8a, 0xee } };




DWORD WINAPI FtpThread( LPVOID lpParam ) { 

	if (can_write == false) { //i can't access the file, oh well
		return 0;
	}

	can_write = false;//i'm gonna use the file, close access

	HINTERNET myFTP = InternetOpen("NowPlaying",INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
	myFTP = InternetConnect(myFTP, 
		FTP_SERVER,
		FTP_PORT,
		FTP_LOGON,
		FTP_PASSWORD,
		INTERNET_SERVICE_FTP,
		FALSE,NULL);

	FtpCreateDirectory(myFTP, FTP_DIRECTORY);
	FtpSetCurrentDirectory(myFTP, FTP_DIRECTORY);

	FtpPutFile(myFTP,
		GENERAL_FILE,
		FTP_FILENAME,
		FTP_TRANSFER_TYPE_ASCII,NULL);
	
	InternetCloseHandle(myFTP);
	
	//this thread is dead, let me write again
	can_write = true;
    return 0; 
} 



NowPlaying::NowPlaying() : WAComponentClient("NowPlaying"){


	/* Regsiter and get the values of the options set by the user*/
	registerSkinFile("wacs/xml/NowPlaying/NowPlaying.xml");
	

	//GENERAL STUFF
	registerAttribute(&GENERAL_ENABLED);
	registerAttribute(&GENERAL_FILE);
	registerAttribute(&GENERAL_HISTORY);
	registerAttribute(&GENERAL_UPDATE);
	registerAttribute(&GENERAL_PATH);
	registerAttribute(&GENERAL_STAY);
	registerAttribute(&GENERAL_SINGLEQ);
	//end GENERAL STUFF


	//Output Stuff
	//song stuff
	registerAttribute(&SONG_ARTIST);
	registerAttribute(&SONG_ALBUM);
	registerAttribute(&SONG_TITLE);
	registerAttribute(&SONG_GENRE);
	registerAttribute(&SONG_YEAR);
	registerAttribute(&SONG_COMMENT);
	registerAttribute(&SONG_TRACK);
	registerAttribute(&SONG_LENGTH);
	//end song stuff
	//file stuff
	registerAttribute(&FILE_RELATIVE);
	registerAttribute(&FILE_SIZE);
	registerAttribute(&FILE_KBPS);
	//registerAttribute(&FILE_CHANNELS);
	registerAttribute(&FILE_FREQUENCY);
	registerAttribute(&FILE_QUALITY);
	registerAttribute(&FILE_NAME);
	//end file stuff
	//Winamp stuff
	registerAttribute(&WINAMP_POSITION);
	registerAttribute(&WINAMP_PLSIZE);
	registerAttribute(&WINAMP_VERSION);
	registerAttribute(&WINAMP_HISTORY);
	//end winampstuff
	//end options stuff

	//FTP STUFF
	registerAttribute(&FTP_ENABLED);
	registerAttribute(&FTP_SERVER);
	registerAttribute(&FTP_LOGON);
	registerAttribute(&FTP_PASSWORD);
	registerAttribute(&FTP_PORT);
	registerAttribute(&FTP_DIRECTORY);
	registerAttribute(&FTP_FILENAME);
	registerAttribute(&FTP_UPDATE);
	registerAttribute(&FTP_MT);
	//END FTP STUFF

	//COMMAND STUFF
	registerAttribute(&COMMAND_ENABLED);
	registerAttribute(&COMMAND_PATH);
	registerAttribute(&COMMAND_ARGUEMENTS);
	registerAttribute(&COMMAND_HIDDEN);
	//end COMMAND STUFF

	//TEMPLATE STUFF
	registerAttribute(&TEMPLATE_ENABLED);
	registerAttribute(&TEMPLATE_HEADER);
	registerAttribute(&TEMPLATE_DATA);
	registerAttribute(&TEMPLATE_FOOTER);
	registerAttribute(&TEMPLATE_HPARSE);
	registerAttribute(&TEMPLATE_FPARSE);
	//END TEMPLATE STUFF

}

// Destructor: nothing here :)
NowPlaying::~NowPlaying() {
}

GUID NowPlaying::getGUID() {
  return guid;
}


void NowPlaying::onCreate() {
	api->core_addCallback(0, this);  // CoreHandle core; core.addCallback(this); doesnt work!
	api->preferences_registerGroup("NowPlaying.Main", "NowPlaying", getGUID()); //Register in preferences screen
	//Begin Retrieving Data



	//End Retrieving Data
}

// And this is the opposite of onCreate :)
void NowPlaying::onDestroy() {
	//Begin Saveing Data





	//End Saveing Data
	api->core_delCallback(0, this);  // CoreHandle core; core.delCallback(this); doesnt work!
}

/* Called when the song being played has changed */
int NowPlaying::corecb_onTitleChange(const char *title){
	Song STemp;
	/* Don't waste enery if you are not needed. 
	 * 
	 * Problem occurs when What's Playing is enabled
	 * between a playlist. Since WP was not enabled before,
	 * it does not have any info on previous history.
	 * That is better than keeping all the info and using
	 * memory and CPU when WP may never be enabled.
	 *
	 * Simialrly problem exists when the history is changed
	 * from a smaller value (5) to a larger value (10) within 
	 * a Winamp session.
	 *
	 */
	if (GENERAL_ENABLED != TRUE) {
	
		return 0;
	}

		

	if ((GENERAL_FILE == NULL) || (GENERAL_FILE == "")) {
		//Output to Error Log
		return 0;
	}
	
	CoreHandle core;
	
	const char * songfile = core.getCurrent();
	char buffer[200] = "\0";
	//strings
	if(1 > api->metadb_getMetaData(songfile, MT_ARTIST, buffer, 200, MDT_STRINGZ)){
	}
	else{
			STemp.Artist = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_ALBUM, buffer,  200, MDT_STRINGZ)){
	}
	else{
		STemp.Album = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_TITLE, buffer,  200, MDT_STRINGZ)){
	}
	else{
		STemp.Title = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_GENRE, buffer,  200, MDT_STRINGZ)){
	}
	else{
		STemp.Genre = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_YEAR,  buffer,  200,  MDT_STRINGZ)){
	}
	else{
		STemp.Year = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_COMMENT, buffer, 200, MDT_STRINGZ)){
	}
	else{
		STemp.Comment = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_TRACK, buffer, 200, MDT_STRINGZ)){
	}
	else{
		STemp.Track = buffer;
	}

	if(1 > api->metadb_getMetaData(songfile, MT_INFO, buffer, 200, MDT_STRINGZ)){
	}
	else{
		STemp.Quality = buffer;
	}



	//Lengths
	api->metadb_getMetaData(songfile, MT_LENGTH, (char *)&STemp.Length, sizeof(STemp.Length),  MDT_TIME);
	
	//ints
	//Following Not Working
	char buff[20];
	api->metadb_getMetaData(songfile, MT_BITRATE, (char *)&STemp.Bitrate, sizeof(STemp.Bitrate), MDT_INT);
	STemp.Bitrate = atoi((char*)&STemp.Bitrate);
	//api->metadb_getMetaData(songfile, MT_NUMCHANNELS, buff, sizeof(buff), MDT_INT);
	//STemp.Channels = atoi(buff);
	api->metadb_getMetaData(songfile, MT_SAMPLERATE, buff, sizeof(buff), MDT_INT);
	STemp.Frequency = atoi(buff);
	STemp.Frequency /= 100;
	//api->metadb_getMetaData( playstring, MT_SIZE, (char*) &iSize, sizeof(int), MDT_INT ); 

	//Using STL for File size
		
	string file = core.getCurrent();
	ifstream ifile(file.substr(5,file.size() -5).c_str(), ios::in | ios::binary);

    if (ifile) { // if the file was opened correctly

        // get file size, by setting the pointer in the end and tell the position
        ifile.seekg(0,ios::end);
		double hm = 1000000;
		sprintf(buffer,"%.2f MB",(ifile.tellg())/hm);
		STemp.Size = buffer;
	}
	else {
		STemp.Size = "";
	}

	ifile.close();
	ifile.clear();

	int find;


	
	if(GENERAL_STAY && GENERAL_PATH != ""){ //if i have a relative path set up
		string path = GENERAL_PATH;
		file = file.substr( 5, file.size()-5 );
		find = 0;
		find = file.find(path,0);
		if (find > -1){
				if(GENERAL_SINGLEQ){
					find = 0;
					find = file.find("'",4);
					while(find > -1){
						file.replace(find,1,"&#39;");
						find = file.find("'",find);
					}
				}
			STemp.Filename = file.substr(find + path.size(), file.size()-find-path.size()+1);
		}
		else{
			return 0; //if they want to stay in specific directory, than this shouldn't be on here
		}
	}
	else {
		if(GENERAL_SINGLEQ){
			find = 0;
			find = file.find("'",4);
			while(find > -1){
				file.replace(find,1,"&#39;");
				find = file.find("'",find);
			}
		}
		STemp.Filename = file.substr(5,file.size() -5); //get rid of file:
	}

	STemp.PLSize = core.getNumTracks();
    STemp.Pos = core.getCurPlaybackNumber();


	//Find the file name
	for(int location = STemp.Filename.size()-1; location > -1; location--){
		if (STemp.Filename[location] == '\\'){
			STemp.FilenameShort = STemp.Filename.substr(location+1,STemp.Filename.size() - location);
			break;
		}
	}
	
	/*Dont' Duplicate at the top (pause-play)*/
	if(allSongs.size() != 0 && STemp.Title == allSongs.top().Title ) {
		return 0;
	}
	
	
	int len_sec=int(STemp.Length / 1000);
	int len_mnts=int(len_sec/60);
	int len_sec_rem=len_sec % 60;
	if(len_sec_rem>9){
		sprintf(buffer,"%d:%2d",len_mnts,len_sec_rem);
	}
	else{
		sprintf(buffer,"%d:0%1d",len_mnts,len_sec_rem);
	}
	STemp.LengthString = buffer;


	allSongs.push(STemp);

	
	//Update on every GENERAL_UPDATE songs, and on the first one
	if(allSongs.size() % GENERAL_UPDATE == 0){
		if(TEMPLATE_ENABLED == TRUE){
			template_write();
		}
		else{
            write_file();
		}
	}


	if (COMMAND_ENABLED == TRUE && COMMAND_PATH != NULL && COMMAND_PATH != "") {
		if (COMMAND_HIDDEN == TRUE){
			ShellExecute(NULL,"open",COMMAND_PATH,COMMAND_ARGUEMENTS.getValue(),NULL,SW_HIDE);
		}
		else {
			ShellExecute(NULL,"open",COMMAND_PATH,COMMAND_ARGUEMENTS.getValue(),NULL,SW_SHOW);
		}
	}
	
	if(FTP_ENABLED == TRUE && (allSongs.size() % FTP_UPDATE == 0)){
		FTP_update();
	}
	
	
    return 0;
}


/* WP doesn't distinguish between Play, Pause and Stop */
int NowPlaying::corecb_onStopped() {
	return 0;
}



//Writes XML Output
int NowPlaying::write_file(){
	
	//check to see if i can write
	if (can_write == false) {
		return 0;
	}
	else {
		can_write = false; //i'm using the file, leave it alone
	}

	Song STemp;
	ofstream outFile;
	outFile.clear();

	outFile.open(GENERAL_FILE,ios::out);

	if (!outFile.is_open()) {
		api->messageBox("Unable to open info file for writing", "OutputFile", MB_OK, NULL/*"{D1D94FE0-A13B-4b23-9553-1726A9C79A44}"*/, api->getModalWnd());
		return 0;
	}
	outFile << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" << endl;

    outFile << "<NowPlaying>" << endl;

	//start data loop
	for(unsigned i = 0;i < allSongs.size() && i < GENERAL_HISTORY; i++){
		//STemp = allSongs.top();


		//Song DATA
		outFile << "\t<SONG position=\"" << i << "\">" << endl;

		outFile << "\t\t<ARTIST>" << allSongs[i].Artist << "</ARTIST>" << endl;
		
		outFile << "\t\t<TITLE>" << allSongs[i].Title << "</TITLE>" << endl;

		outFile << "\t\t<ALBUM>" << allSongs[i].Album << "</ALBUM>" << endl;

		outFile << "\t\t<TRACK>" << allSongs[i].Track << "</TRACK>" << endl;

		outFile << "\t\t<GENRE>" << allSongs[i].Genre << "</GENRE>" << endl;
		
		outFile << "\t\t<YEAR>" << allSongs[i].Year << "</YEAR>" << endl;
		
		outFile << "\t\t<COMMENT>" << allSongs[i].Comment << "</COMMENT>" << endl;

		outFile << "\t\t<LENGTH>" << allSongs[i].LengthString << "</LENGTH>" << endl;

		//File DATA

		outFile << "\t\t<QUALITY>" << allSongs[i].Quality << "</QUALITY>" << endl;

		outFile << "\t\t<SIZE>" << setprecision(2) << fixed << allSongs[i].Size << "</SIZE>" << endl;

		//if(FILE_CHANNELS){
		//	outFile << "<td>" << allSongs[i].Channels << "</td>" << endl;
		//}

		outFile << "\t\t<BITRATE>" << allSongs[i].Bitrate << "</BITRATE>" << endl;

		outFile << "\t\t<FREQUENCY>" << setprecision(1) << allSongs[i].Frequency << "</FREQUENCY>" << endl;

		outFile << "\t\t<RELATIVE>" << allSongs[i].Filename << "</RELATIVE>" << endl;

		outFile << "\t\t<FILENAME>" << allSongs[i].FilenameShort << "</FILENAME>" << endl;

		//WINAMP STUFF

		outFile << "\t\t<POSITION>" << allSongs[i].Pos << "</POSITION>" << endl;

		outFile << "\t\t<PLAYLIST>" << allSongs[i].PLSize << "</PLAYLIST>" << endl;

		outFile << "\t</SONG>" << endl;		
	}//end data loop


	//FOOTER
	outFile << "\t<VERSION>" << api->main_getVersionString() << "</VERSION>" << endl;
	
	outFile << "</NowPlaying>" << endl;
	
	
	outFile.close();
	

	can_write = true; //i'm done with the file
	
	return 1;
}


int NowPlaying::template_write(){
	if (can_write == false) {
		return 0;
	}
	else {
		can_write = false; //i'm using the file, leave it alone
	}

	ofstream outFile;
	ifstream inFile;
	string temp;

	outFile.open(GENERAL_FILE, ios::out);

	if(!outFile.is_open()){
		return 0;
	}

	
	//HEADER STUFF
	inFile.open(TEMPLATE_HEADER, ios::in);
	if(!inFile.is_open()){
		inFile.clear();
		outFile.close();
		outFile.clear();
		return 0;
	}
	while(inFile.good()){
		getline(inFile,temp);
		if(TEMPLATE_HPARSE){
			parse(temp,0);
		}
		outFile << temp << endl;
	}//end output loop


	inFile.close();
	inFile.clear();


	//DATA STUFF
	inFile.open(TEMPLATE_DATA, ios::in);
	if(!inFile.is_open()){
		inFile.clear();
		outFile.close();
		outFile.clear();
		return 0;
	}

	vector<string> lines;

	unsigned i;
	if(TEMPLATE_HPARSE){
		i = 1;
	}
	else{
		i = 0;
	}

	while(inFile.good()){
		getline(inFile,temp);
		lines.push_back(temp);
	}

	//all lines of data file are lines
	for(;i<allSongs.size() && i < GENERAL_HISTORY;i++){//Do every song up to the last one, or the number set in general history.
		for(unsigned j = 0; j < lines.size(); j++){
			temp = lines[j];
			parse(temp,i);
			outFile << temp << endl;
		}
	}

	inFile.close();
	inFile.clear();

	//FOOTERSTUFF
	inFile.open(TEMPLATE_FOOTER, ios::in);
	if(!inFile.is_open()){
		inFile.clear();
		outFile.close();
		outFile.clear();
		return 0;
	}

	while(inFile.good()){
		getline(inFile,temp);
		if(TEMPLATE_FPARSE){
			parse(temp,0);
		}

		outFile << temp << endl;
	}//end output loop




	inFile.close();
	inFile.clear();


	outFile.close();
	outFile.clear();

	can_write = true;
	return 0;
}

int NowPlaying::FTP_update(){
	

	if(FTP_MT == TRUE){
		//set shit to pass

		DWORD dwThreadId;
		HANDLE hThread; 

		hThread = CreateThread( 
			NULL,						// no security attributes 
			0,							// use default stack size  
			FtpThread,					// thread function 
			NULL,						// argument to thread function 
			0,							// use default creation flags 
			&dwThreadId);				// returns the thread identifier 
			
		// Check the return value for success. 
			
		if (hThread == NULL){//in case the thread fails, do it the old way
			FTP_MT = FALSE; //it failed, use normal way
		}
		else {
			CloseHandle( hThread );
		}
	}

	if(FTP_MT == FALSE){
			if (can_write == false){//no writing till i say so
				return 0;
			}

			HINTERNET myFTP = InternetOpen("NowPlaying",INTERNET_OPEN_TYPE_DIRECT,NULL,NULL,0);
			myFTP = InternetConnect(myFTP,FTP_SERVER,FTP_PORT,FTP_LOGON,FTP_PASSWORD,INTERNET_SERVICE_FTP,FALSE,NULL);

			FtpCreateDirectory(myFTP, FTP_DIRECTORY);
			FtpSetCurrentDirectory(myFTP, FTP_DIRECTORY);

			FtpPutFile(myFTP,GENERAL_FILE,FTP_FILENAME,FTP_TRANSFER_TYPE_ASCII,NULL);
			
			InternetCloseHandle(myFTP);

			can_write = true; //i'm done accessing
	}
	

	return 0;
}

#include "template.cpp" //Here for now, one day i'll make this all look right



