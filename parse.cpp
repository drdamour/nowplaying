#include "nowplaying.h"



void NowPlaying::parse(string & temp,int index){
	int find;
	char buff[6];

		if(WINAMP_HISTORY){
			find = 0;
			itoa(index,buff,10);
			find = temp.find("%H",0);
			while(find > -1){
				temp.replace(find,2,buff);
				find = temp.find("%H",find);
			}
		}

		if(SONG_ARTIST)	{
			find = 0;
			find = temp.find("%A",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Artist);
				find = temp.find("%A",find);
			}
		}

		if(SONG_TITLE){
			find = 0;
			find = temp.find("%T",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Title);
				find = temp.find("%T",find);
			}
		}

		if(SONG_ALBUM){
			find = 0;
			find = temp.find("%a",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Album);
				find = temp.find("%a",find);
			}
		}

		if(SONG_GENRE){
			find = 0;
			find = temp.find("%G",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Genre);
				find = temp.find("%G",find);
			}
		}
		
		if(SONG_YEAR){
			find = 0;
			find = temp.find("%Y",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Year);
				find = temp.find("%Y",find);
			}
		}
		
		if(SONG_COMMENT){
			find = 0;
			find = temp.find("%C",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Comment);
				find = temp.find("%C",find);
			}
		}

		if(SONG_TRACK){
			find = 0;
			find = temp.find("%t",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Track);
				find = temp.find("%t",find);
			}
		}
		if(SONG_LENGTH){
			find = 0;
			find = temp.find("%L",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].LengthString);
				find = temp.find("%L",find);
			}
		}

		//File DATA

		if(FILE_QUALITY){
			find = 0;
			find = temp.find("%Q",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Quality);
				find = temp.find("%Q",find);
			}
		}

		if(FILE_SIZE){
			find = 0;
			find = temp.find("%S",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Size);
				find = temp.find("%S",find);
			}
		}

		//if(FILE_CHANNELS){
		//	outFile << "<td>Channels</td>" << endl;
		//}

		if(FILE_KBPS){
			find = 0;
			find = temp.find("%K",0);
			itoa(allSongs[index].Bitrate,buff,10);
			while(find > -1){
				temp.replace(find,2,buff);
				find = temp.find("%K",find);
			}
			
		}

		if(FILE_FREQUENCY){
			find = 0;
			find = temp.find("%f",0);
			sprintf(buff,"%.1f",allSongs[index].Frequency);
			while(find > -1){
				temp.replace(find,2,buff);
				find = temp.find("%f",find);
			}
		}

		if(FILE_RELATIVE){
			find = 0;
			find = temp.find("%F",0);
			while(find > -1){
				temp.replace(find,2,allSongs[index].Filename);
				find = temp.find("%F",find);
			}
		}

		//WINAMP STUFF

		if(WINAMP_POSITION){
			find = 0;
			find = temp.find("%p",0);
			itoa(allSongs[index].Pos,buff,10);
			while(find > -1){
				temp.replace(find,2,buff);
				find = temp.find("%p",find);
			}
		}

		if(WINAMP_PLSIZE){
			find = 0;
			find = temp.find("%P",0);
			itoa(allSongs[index].PLSize,buff,10);
			while(find > -1){
				temp.replace(find,2,buff);
				find = temp.find("%P",find);
			}
		}


		if(WINAMP_VERSION){
			find = 0;
			find = temp.find("%V",0);
			while(find > -1){
				temp.replace(find,2,api->main_getVersionString());
				find = temp.find("%V",find);
			}
		}



};