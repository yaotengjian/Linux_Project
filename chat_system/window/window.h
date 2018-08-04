#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

class window{
	public:
		window();
		void drawTitle();
		void drawOutput();
		void drawFlist();
		void drawInput();
		void putStringToWin(WINDOW *w, int y, int x,\
				const std::string &s);
		void clearLines(WINDOW *w, int begin, int num);
		void titleMove();
		void inputGetMsg(std::string &outString);
		void getStringFromWin(WINDOW *w, std::string &s);
		~window();
	public:
		WINDOW *title;
		WINDOW *output;
		WINDOW *flist;
		WINDOW *input;
		pthread_mutex_t lock;
};

#endif



