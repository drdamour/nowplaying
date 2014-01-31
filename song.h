#ifndef songclasses
#define songclasses
#include <string>


//song defined as class incase i ever switch to having member funcions
class Song {
public:
	Song(){}
	~Song(){}
	//Song & operator=( Song & source );

	void clear();

	string Filename;
	string FilenameShort; //everything after the last "\"
	string Artist;
	string Album;
	string Title;
	string Genre;
	string Year;
	string Comment;
	string LengthString;
	string Track;
	string Quality;
	string Size;			  //uses STL fstream in MBS
	
	int Length;
	int Pos;
	int PLSize;

	
	int Bitrate;		//in Kbps
	int Channels;       //not working
	double Frequency;      //in kilhertz

};



class Songstack {
public:
	Songstack(unsigned max = 100);
	~Songstack();

	unsigned size();
	Song top();
	void push(Song);
	void clear();
	Song operator[](unsigned);
	Songstack & operator=( Songstack & source );
	void pop();
	unsigned maxsize();

private:
	Song ** allSongs;
	unsigned Top;
	unsigned Size;
	unsigned Maxsize;
};



Songstack::Songstack(unsigned max):Maxsize(max),Size(0),Top(max+1){
	allSongs = new Song*[Maxsize];
	for(unsigned i = 0; i < Maxsize; i++){
		allSongs[i] = NULL;
	}
}

Songstack::~Songstack(){
	delete [] allSongs;
}

Song Songstack::top(){
	//is top valid?
	if (Top > Maxsize) {
		Song temp;
		temp.clear();
		return temp;
	}
	
	return *allSongs[Top];
}

void Songstack::push(Song newsong){
	Top = (Top + 1) % Maxsize;
	
	Song * temp = new Song; 	//allocate new memmory
	*temp = newsong;             //store there
	if (allSongs[Top] != NULL){
        delete allSongs[Top];		//delete what was there
	}
	allSongs[Top] = temp;		//place something there

	//change size if it's not too big
	if (Size < Maxsize){
		Size++;
	}
}

void Songstack::pop(){
	if (allSongs[Top] != NULL){
		delete allSongs[Top];   //delete what's at the top
	}
	Top--;
	if (Top < 0){
		Top += 100;
	}
	Size--;
}

unsigned Songstack::size(){
	return Size;
}

void Songstack::clear(){
	for(unsigned i = 0; i< Size;i++){
		delete allSongs[i]; //delete
		allSongs[i] = NULL; //Set to Null
	}
	//delete [] allSongs;
	//allSongs = new Song[Maxsize];
	Size = 0;
	Top = 0;
}

Song Songstack::operator [](unsigned i){
	if (i > (Size - 1)){
		Song temp;
		temp.clear();
		return temp;
	}

	i = Top - i;

	if (i > Maxsize){
		i += 100;
	}

	return *allSongs[i];
}

//ret-type operatorop( arg1, arg2 )
Songstack & Songstack::operator=( Songstack & source ){
	for(unsigned i = 0;i < source.size(); i++){
		push(source[i]);
	}

	return *this;
}

unsigned Songstack::maxsize(){
	return Maxsize;
}


void Song::clear(){
	Filename.clear();
	FilenameShort.clear();
	Artist.clear();
	Album.clear();
	Title.clear();
	Genre.clear();
	Year.clear();
	Comment.clear();
	LengthString.clear();
	Length = 0;
	Size.clear();
	Pos = 0;
	PLSize = 0;
	Track.clear();
	Quality.clear();
}

//char * Song::Length(){
//		char ret[15];
//		int len_sec=int(Length / 1000);
//		int len_mnts=int(len_sec/60);
//		int len_sec_rem=len_sec % 60;
//		if(len_sec_rem>9)
//			sprintf(ret,"%d:%2d",len_mnts,len_sec_rem);
//		else
//			sprintf(ret,"%d:0%1d",len_mnts,len_sec_rem);
//		if(!strcmp(ret,"")||ret==NULL) 
//			strcpy(ret,"");
//
//		return ret;
//}
//Song & Song::operator=( Song & source ){
//	Filename = source.Filename;
//	Artist = source.Artist;
//	Album = source.Album;
//	Title = source.Title;
//	Genre = source.Genre;
//	Year = source.Year;
//	Comment = source.Comment;
//	LengthString = source.LengthString;
//	Length = source.Length;
//	Size = source.Size;
//	Pos = source.Pos;
//	PLSize = source.PLSize;
//	Track = source.Track;
//	Info = source.Info;	
//
//	return *this;
//}

#endif
