#ifndef NOWPLAYING
#define NOWPLAYING


#include <windows.h>
#include <string>
using namespace std;

#include "C:\Wasabi SDK\studio\studio\wac.h" // Basic components stuff
#include "C:\Wasabi SDK\studio\common\corehandle.h"	// Core handle stuff
#include "C:\Wasabi SDK\studio\studio\corecb.h"	// Core callback stuff
#include "C:\Wasabi SDK\studio\attribs\attribs.h" // Config attribute stuff
#include "C:\Wasabi SDK\studio\common\metatags.h" // MetaDB stuff
#include "song.h"



class NowPlaying : public WAComponentClient, public CoreCallbackI {
public:
	NowPlaying();
	virtual ~NowPlaying();
	virtual void onCreate();
	virtual void onDestroy();
	virtual GUID getGUID();
	virtual int corecb_onStopped();
	virtual int corecb_onTitleChange(const char *title);
	void push_down();
	void shutdown();
	int write_file();
	int FTP_update();
	int template_write();
	void parse(string &,int);

};

#endif
