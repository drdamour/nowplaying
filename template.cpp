
//parser:	Goes through a string looking for a match, and then replaces it with desired string
//Gets:		to_parse - string to parse returned via reference
//			to_match - string to match against
//			to_replace - string that is inserted in place of matches

void parser(string & to_parse, string to_match, string to_replace){

	//set my search position
	int position = 0;

	//while the patch is found
	while( (position = to_parse.find(to_match, position)) > -1){
		to_parse.replace(position, to_match.size(), to_replace);
	}

}


void NowPlaying::parse(string & temp,int index){
	char buff[6];

		if(WINAMP_HISTORY){
			itoa(index,buff,10);
			parser(temp, "%H", buff);	
		}

		if(SONG_ARTIST)	{
			parser(temp, "%A", allSongs[index].Artist);
		}

		if(SONG_TITLE){
			parser(temp, "%T", allSongs[index].Title);
		}

		if(SONG_ALBUM){
			parser(temp, "%a", allSongs[index].Album);
		}

		if(SONG_GENRE){
			parser(temp, "%G", allSongs[index].Genre);
		}
		
		if(SONG_YEAR){
			parser(temp, "%Y", allSongs[index].Year);
		}
		
		if(SONG_COMMENT){
			parser(temp, "%C", allSongs[index].Comment);
		}

		if(SONG_TRACK){
			parser(temp, "%t", allSongs[index].Track);
		}

		if(SONG_LENGTH){
			parser(temp, "%L", allSongs[index].LengthString);
		}

		//File DATA

		if(FILE_QUALITY){
			parser(temp, "%Q", allSongs[index].Quality);
		}

		if(FILE_SIZE){
			parser(temp, "%S", allSongs[index].Size);
		}

		//if(FILE_CHANNELS){
		//	outFile << "<td>Channels</td>" << endl;
		//}

		if(FILE_KBPS){
			itoa(allSongs[index].Bitrate,buff,10);
			parser(temp, "%K", buff);
		}

		if(FILE_FREQUENCY){
			sprintf(buff,"%.1f",allSongs[index].Frequency);
			parser(temp, "%f", buff);
		}

		if(FILE_RELATIVE){
			parser(temp, "%F", allSongs[index].Filename);
		}

		if(FILE_NAME){
			parser(temp, "%n", allSongs[index].FilenameShort);
		}

		//WINAMP STUFF

		if(WINAMP_POSITION){
			itoa(allSongs[index].Pos,buff,10);
			parser(temp, "%p", buff);
		}

		if(WINAMP_PLSIZE){
			itoa(allSongs[index].PLSize,buff,10);
			parser(temp, "%P", buff);
		}


		if(WINAMP_VERSION){
			parser(temp, "%V", api->main_getVersionString());
		}

};