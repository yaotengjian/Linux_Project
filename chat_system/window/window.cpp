#include "window.h"

window::window()
{
	initscr();
	curs_set(0);
	pthread_mutex_init(&lock, NULL);
}

void window::drawTitle()
{
	int y = 0;
	int x = 0;
	int h = LINES/5;
	int w = COLS;
	pthread_mutex_lock(&lock);
	title=newwin(h, w, y, x);
	box(title, 0, 0);
	wrefresh(title);
	pthread_mutex_unlock(&lock);
}

void window::drawOutput()
{
	int y = LINES/5;
	int x = 0;
	int h = (LINES*3)/5;
	int w = (COLS*3)/4;
	pthread_mutex_lock(&lock);
	output = newwin(h, w, y, x);
	box(output, 0, 0);
	wrefresh(output);
	pthread_mutex_unlock(&lock);
}

void window::drawFlist()
{
    int	y = LINES/5;
    int	x = (COLS*3)/4;
    int	h = (LINES*3)/5;
    int	w = COLS/4;
	pthread_mutex_lock(&lock);
	flist = newwin(h, w, y, x);
	box(flist, 0, 0);
	wrefresh(flist);
	pthread_mutex_unlock(&lock);
}

void window::drawInput()
{
	int y = (LINES*4)/5;
	int x = 0;
	int h = LINES/5;
	int w = COLS;
	pthread_mutex_lock(&lock);
	input = newwin(h, w, y, x);
	box(input, 0, 0);
	wrefresh(input);
	pthread_mutex_unlock(&lock);
}

void window::putStringToWin(WINDOW *w, int y, int x, const std::string &s)
{
	pthread_mutex_lock(&lock);
	mvwaddstr(w, y, x, s.c_str());
	wrefresh(w);
	pthread_mutex_unlock(&lock);
}

void window::getStringFromWin(WINDOW *w, std::string &s)
{
	char buf[1024];
	wgetnstr(w, buf, sizeof(buf)-1);
	s = buf;
}

void window::clearLines(WINDOW *w, int begin, int num)
{

	pthread_mutex_lock(&lock);
	while(num-- > 0){
		wmove(w, begin++, 0); //y,x
		wclrtoeol(w);
	}
	box(w, 0, 0);
	wrefresh(w);
	pthread_mutex_unlock(&lock);
}

void window::titleMove()
{
	const std::string s = "Welcome To My Chat System";
	int y, x;
	int i = 1;
	int dir = 0;
	for(; ;){
		getmaxyx(title, y, x);
		putStringToWin(title, y/2, i, s);
		int len = x - s.size();
		len = len > 0 ? len : x;
		if( i == len - 1 ){
			dir = 1;
		}
		if( i == 1 ){
			dir = 0;
		}

		if(dir == 0){
			i++;
		}
		if(dir == 1){
			i--;
		}
		//i %= len;
		box(title, 0, 0);
		wrefresh(title);
		usleep(200000);
		clearLines(title, y/2, 1);
	}
}

void window::inputGetMsg(std::string &outString)
{
	int y, x;
	const std::string s = "Enter# ";
	getmaxyx(input, y, x);
	putStringToWin(input, y/2, 1, s);
	wrefresh(input);
	getStringFromWin(input, outString);
	clearLines(input, y/2, 1);
}

window::~window()
{
	delwin(title);
	delwin(output);
	delwin(flist);
	delwin(input);
	curs_set(1);
	pthread_mutex_destroy(&lock);
	endwin();
}


//
//int main()
//{
//	window w;
//	w.drawTitle();
//	w.drawOutput();
//	w.drawFlist();
//	w.drawInput();
////	w.titleMove();
//
//	std::string s;
//	int i = 1;
//	while(1){
//		w.inputGetMsg(s);
//		w.putStringToWin(w.output, i++, 1, s);
//
//	}
//	return 0;
//}














