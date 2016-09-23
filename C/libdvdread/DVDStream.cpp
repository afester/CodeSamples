#include <fcntl.h>	// SEEK_SET
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <dvdnav/dvdnav.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <strings.h>

#include "Socket.h"


/* shall we use libdvdnav's read ahead cache? */
#define DVD_READ_CACHE 1

/* which is the default language for menus/audio/subpictures? */
#define DVD_LANGUAGE "en"

using namespace std;

class DVDProgressListener {
public:
    virtual void progress(int event) = 0;
    virtual void sendBlock(uint8_t* buf, int len) = 0;
};

class DVD {
   dvdnav_t* handle;
   DVDProgressListener* progressListener;

public:
   DVD();

   void setProgressListener(DVDProgressListener* listener);

   void open(const std::string& path);

   vector<string> getContents();

   void close();

   void reset();

   int getNumberOfTitles();

   void play(int32_t title);

   void stop();

   void pause();

   string getLastError();
};

DVD::DVD() : handle(0), progressListener(0) {
}

void DVD::setProgressListener(DVDProgressListener* listener) {
   progressListener = listener;
}

void DVD::open(const std::string& path) {
    dvdnav_status_t result = dvdnav_open(&handle, path.c_str());
    if (result == DVDNAV_STATUS_ERR) {
        cerr << "SEVERE: Could not open \"" << path << "\"" << endl;
        exit(2);
    }
}


int DVD::getNumberOfTitles() {
   int32_t count = 0;
   dvdnav_status_t result = dvdnav_get_number_of_titles(handle, &count);
   if (result == DVDNAV_STATUS_ERR) {
       cerr << "SEVERE: Could not get number of titles" << endl;
       exit(2);
   }
   return count;
}


vector<string> DVD::getContents() {
   vector<string> result;

   result.push_back("Chapter 1");
   result.push_back("Chapter 2");
   result.push_back("Chapter 3");
   result.push_back("Chapter 4");
   result.push_back("Chapter 5");

   return result;
}

void DVD::reset() {
   dvdnav_status_t result = dvdnav_reset(handle);
   if (result == DVDNAV_STATUS_ERR) {
     cerr << "SEVERE: Could not reset" << endl;
     exit(2);
   }
}

void DVD::close() {
   if (handle != 0) {
      dvdnav_status_t result = dvdnav_close(handle);
      if (result == DVDNAV_STATUS_ERR) {
        cerr << "SEVERE: Could not close" << endl;
        exit(2);
      }
      handle = 0;
   }
}


void DVD::play(int32_t title) {
   uint8_t mem[DVD_VIDEO_LB_LEN];
   cerr << "Buffer size: " << DVD_VIDEO_LB_LEN << endl;

   if (dvdnav_set_readahead_flag(handle, DVD_READ_CACHE) != DVDNAV_STATUS_OK) {
        cerr << "SEVERE: Could not set readahead flag (" << getLastError() << ")" << endl;
        return;
   }

    if (dvdnav_menu_language_select(handle, DVD_LANGUAGE) != DVDNAV_STATUS_OK ||
        dvdnav_audio_language_select(handle, DVD_LANGUAGE) != DVDNAV_STATUS_OK ||
        dvdnav_spu_language_select(handle, DVD_LANGUAGE) != DVDNAV_STATUS_OK) {
        cerr << "SEVERE: Could not set Language (" << getLastError() << ")" << endl;
        return ;
    }

    if (dvdnav_set_PGC_positioning_flag(handle, 1) != DVDNAV_STATUS_OK) {
        cerr << "SEVERE: Could not set PGC (" << getLastError() << ")" << endl;
        return ;
    }


//   dvdnav_status_t result = dvdnav_title_play(handle, title);
   dvdnav_status_t result = dvdnav_part_play(handle, 1, 1);
   if (result == DVDNAV_STATUS_ERR) {
      cerr << "SEVERE: Could not play (" << getLastError() << ")" << endl;
      exit(2);
   }

   int state = -1; // HACK
   bool finished = false;
   bool m_stopped = false;
   int32_t m_idxLect = 0xFFFFFFFF;
   int32_t streamed = 0;
   while (!finished && !m_stopped) { //  && qApp!=NULL) {

       int result, event, len;
       uint8_t *buf = mem;

        if (m_idxLect !=0xFFFFFFFF) {
            cerr << "  SEEK" << endl;
  	    dvdnav_sector_search(handle, m_idxLect,SEEK_SET);
	    m_idxLect=0xFFFFFFFF;
	}

        /* the main reading function */
//#ifdef DVD_READ_CACHE
//
//        result = dvdnav_get_next_cache_block(dvdnav, &buf, &event, &len);
//#else

        result = dvdnav_get_next_cache_block(handle, &buf, &event, &len);
//        result = dvdnav_get_next_block(handle, buf, &event, &len);
//#endif
        if (result == DVDNAV_STATUS_ERR) {
            cerr << "SEVERE: Could not read (" << getLastError() << ")" << endl;
            exit(2);
        }

       if (progressListener != 0) {
          progressListener->progress(event);
       }

   int32_t tt = 0,ptt=0;
   uint32_t pos, lgr;
        switch (event) {
           case DVDNAV_NAV_PACKET:{
                dvdnav_current_title_info(handle, &tt, &ptt);
		dvdnav_get_position(handle, &pos, &lgr);
               // cerr << "DVDNAV_NAV_PACKET " << tt << ", " << ptt << ", " << pos << ", " << lgr << endl;
               //  if (state != 0) {
                //     cerr << endl;
                 //    state = 0;
               //  }
                cerr << "DVDNAV_NAV_PACKET (" << streamed << ";" <<  tt << ", " << ptt << ", " << pos << ", " << lgr << ")" << "\r";

		if (tt != title)
			finished=1;
	
		if (finished==0 && buf[17]==0xE0) {
		//	m_decoder.addData( buf,len);
                    cerr << "  SPECIAL CASE" << endl;
		}
                streamed = 0;
              }break;

           case DVDNAV_BLOCK_OK:
              streamed += len;
              //   if (state != 1) {
              //       cerr << endl;
              //       state = 1;
              //   }
              cerr << "DVDNAV_BLOCK_OK (" << streamed << ";" <<  tt << ", " << ptt << ", " << pos << ", " << lgr << ")" << "\r";
              progressListener->sendBlock(buf, len);
              break;

           case DVDNAV_NOP:
              cerr << "DVDNAV_NOP" << endl;
              break;

           case DVDNAV_STILL_FRAME:
              cerr << "DVDNAV_STILL_FRAME" << endl;
              break;

           case DVDNAV_WAIT:
              cerr << "DVDNAV_WAIT" << endl;
              break;

           case DVDNAV_SPU_CLUT_CHANGE:
              cerr << "DVDNAV_SPU_CLUT_CHANGE" << endl;
              break;

           case DVDNAV_SPU_STREAM_CHANGE:
              cerr << "DVDNAV_SPU_STREAM_CHANGE" << endl;
              break;

           case DVDNAV_AUDIO_STREAM_CHANGE:
              cerr << "DVDNAV_AUDIO_STREAM_CHANGE" << endl;
              break;

           case DVDNAV_HIGHLIGHT: {
              dvdnav_highlight_event_t *highlight_event = (dvdnav_highlight_event_t *)buf;
              cerr << "DVDNAV_HIGHLIGHT (" << highlight_event->sx << "," << highlight_event->sy << "/" << highlight_event->ex <<  "," << highlight_event->ey << ")"  << endl;
              }break;

           case DVDNAV_VTS_CHANGE:
              cerr << "DVDNAV_VTS_CHANGE" << endl;
              break;

           case DVDNAV_CELL_CHANGE:
              cerr << "DVDNAV_CELL_CHANGE" << endl;
              break;

           case DVDNAV_HOP_CHANNEL:
              cerr << "DVDNAV_HOP_CHANNEL" << endl;
              break;

           case DVDNAV_STOP:
              cerr << "DVDNAV_STOP" << endl;
              finished = true;
              break;

           default:
              cerr << "UNKNOWN: " << event << endl;
              break;
	}
    }

    cerr << "Playback done." << endl;
}

void DVD::stop() {
   if (dvdnav_stop(handle) != DVDNAV_STATUS_OK) {
        cerr << "SEVERE: Could not stop (" << getLastError() << ")" << endl;
   }
}

void DVD::pause() {
}

string DVD::getLastError() {
   return dvdnav_err_to_string(handle);
}


class DVDRemoteApp : public DVDProgressListener {
    vector<string> argv;

    string source;
    DVD dvdReader;
    ClientSocket client;
    long fileOffset;
    long streamSize;
    int outfile;

public:
    DVDRemoteApp(const vector<string>& args);

    int run();

    void usage();

    void startPlay();

    void stopPlay();

    void progress(int event);

    void sendBlock(uint8_t* data, int len);

    void startStream();

    void sendHttpResponse();
};


DVDRemoteApp::DVDRemoteApp(const vector<string>& args) : argv(args), fileOffset(0), streamSize(-1) {
}

int DVDRemoteApp::run() {
   if (argv.size() != 2) {
       usage();
   }
   source = argv[1];

   dvdReader.setProgressListener(this);

   ServerSocket server(1234);

   // Server's main loop.
   while(1) {
      client = server.accept();
      string cmd;
      do {
         cmd = client.readLine();
         cerr << "  " << cmd << endl;
         if (cmd == "START") {
            startPlay();
         } else if (cmd == "STOP") {
            stopPlay();
         } else if (cmd == "STREAM") {
            startStream();
         } else if (cmd == "\r") {
            sendHttpResponse();
         } else if (cmd.compare(0, 13, "Range: bytes=") == 0) {
            string range = cmd.substr(13);
            size_t pos = range.find('-');
            string fromStr = range.substr(0, pos);
            string toStr = range.substr(pos + 1);
            fileOffset = atol(fromStr.c_str());
            long to = atol(toStr.c_str());
            streamSize = to - fileOffset + 1;
         }
      }while(cmd != "");
      client.close();
   }

   server.close();

   return 0;
}

void DVDRemoteApp::startPlay() {
   cerr << "Start stream ..." << endl;

   outfile = open("out.vob", O_WRONLY | O_CREAT, 0777);
   dvdReader.open(source);
   cerr << "Number of titles:" << dvdReader.getNumberOfTitles() << endl;

   dvdReader.play(1); // title starts at 1 and proceeds up to getNumberOfTitles()

   dvdReader.close();
}

void DVDRemoteApp::sendHttpResponse() {
   cerr << "SEND HTTP RESPONSE ..." << endl;

   struct stat statResult;
   int result = stat("SampleVideo.mp4", &statResult);
   if (result == -1) perror("stat");

   char tbuf[100];
   strftime(tbuf, sizeof(tbuf), "%a, %d %b %Y %H:%M:%S %Z", localtime(&statResult.st_mtime));

   char cbuf[100];
   time_t now = time(NULL);
   strftime(cbuf, sizeof(cbuf), "%a, %d %b %Y %H:%M:%S %Z", localtime(&now));


   stringstream response;

   if (streamSize == -1) {
     response << "HTTP/1.1 200 OK\r\n";
//   response << "Date: " << cbuf << "\r\n";
     response << "Date: Tue, 22 Mar 2016 07:48:09 GMT\r\n";
//   response << "Server: DVDServer 0.1\r\n";
     response << "Server: Apache/2.4.7 (Ubuntu)\r\n";
//   response << "Last-Modified: " << tbuf << "\r\n";
     response << "Last-Modified: Mon, 21 Mar 2016 12:10:09 GMT\r\n";
     response << "ETag: \"a03275-52e8dfa5d4388\"\r\n";
//     response << "Accept-Ranges: bytes\r\n";
     response << "Accept-Ranges: none\r\n";	// defines whether the stream supports ranges of data (Seeking)
//    response << "Transfer-Encoding: chunked\r\n";
//     response << "Transfer-coding: chunked\r\n";
     response << "Content-Length: " << statResult.st_size << "\r\n";
//     response << "Content-Length: " << 0 << "\r\n";
     response << "Keep-Alive: timeout=5, max=100\r\n";
     response << "Connection: Keep-Alive\r\n";
     response << "Content-Type: video/mp4\r\n\r\n";
   } else {
     response << "HTTP/1.1 206 Partial Content\r\n";
//   response << "Date: " << cbuf << "\r\n";
     response << "Date: Tue, 22 Mar 2016 07:48:09 GMT\r\n";
//   response << "Server: DVDServer 0.1\r\n";
     response << "Server: Apache/2.4.7 (Ubuntu)\r\n";
//   response << "Last-Modified: " << tbuf << "\r\n";
     response << "Last-Modified: Mon, 21 Mar 2016 12:10:09 GMT\r\n";
     response << "ETag: \"a03275-52e8dfa5d4388\"\r\n";
//     response << "Accept-Ranges: bytes\r\n";
     response << "Accept-Ranges: none\r\n";
     response << "Content-Length: " << streamSize << "\r\n";
     response << "Content-Range: bytes " << fileOffset << "-" << fileOffset + streamSize - 1 << "/"<< fileOffset + streamSize << "\r\n";
     response << "Keep-Alive: timeout=5, max=100\r\n";
     response << "Connection: Keep-Alive\r\n";
     response << "Content-Type: video/mp4\r\n\r\n";
   }

   cerr << response.str() << endl;
   client.writeData((uint8_t*) response.str().c_str(), response.str().length());

   //startStream();
   startPlay();
}



void DVDRemoteApp::stopPlay() {
   cerr << "Stop stream ..." << endl;
   dvdReader.stop();
}

void DVDRemoteApp::startStream() {
   cerr << "Starting to stream ..." << fileOffset << "," << streamSize << endl;
   int source = open("SampleVideo.mp4", O_RDONLY);
   if (source < 0) perror("open");

   int error = lseek(source, fileOffset, SEEK_SET);
   if (error < 0) perror("lseek");

   uint8_t buf[2048];
   size_t count = 0;
   int connreset = 0;
   int numBytes = 0;
   do {
     count = read(source, buf, sizeof(buf));
     connreset = client.writeData(buf, count);
     numBytes += count;
   } while(connreset == 0 && 
           count == sizeof(buf) &&
           (streamSize == -1 || numBytes < streamSize));
   close(source);
   cerr << "Streaming done." << endl;
}

void DVDRemoteApp::progress(int event) {
   string cmd = client.checkLine();
   if (cmd != "") {
      cerr << endl << "COMMAND: " << cmd << endl;
       if (cmd == "START") {
         //startPlay();
       } else if (cmd == "STOP") {
         stopPlay();
       }
   }
}


void DVDRemoteApp::sendBlock(uint8_t* data, int len) {
   ::write(outfile, data, len);
//   client.writeData(data, len);
}


void DVDRemoteApp::usage() {
   cerr << "Usage: " << argv[0] << " path" << endl;
   exit(1);
}

int main(int argc, char** argv) {
   vector<string> args;
   for (int i = 0;  i < argc;  i++) {
       args.push_back(argv[i]);
   }

   DVDRemoteApp app(args);
   return app.run();
}

