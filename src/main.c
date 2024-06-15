#include <stdio.h>
#include <windows.h> // for Sleep function
#include <ncurses/ncurses.h>

#include <ncurses/ncurses.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

int stopwatch(WINDOW* win, int* min, int* sec, int start, int stop, int reset, int lap, int quit) {
    while (1) {
        mvwprintw(win, 12, 25, "%02d:%02d", *min, *sec);
        if(start==1){
			wrefresh(win);
			Sleep(1000);

			(*sec)++;
			if (*sec == 60) {
				(*min)++;
				*sec = 0;
			}

			if (stop==1) {
				while (1) {
					if (start == 1) {
						break;
					}
					if (quit==1) {
						return 0;
					}
					if (reset==1) {
						*min = 0;
						*sec = 0;
						werase(win);
						box(win, 0, 0);
						wrefresh(win);
						break;
					}
				}
			}
			
			if(lap==1){
				int y=0;
				WINDOW* lapwin=newwin(25, 10, 5, 101);
				mvwprintw(lapwin, y+1, 102, "%02d:%02d", *min, *sec);
				y++;
			}
		}
	}
    return 1;
}



int main() {
    int height = 25; // Adjusted height to fit choices comfortably
    int width = 50;
	int min = 0;
	int sec = 0;
	int start = 0;
	int stop = 0;
	int reset = 0;
	int lap = 0;
	int quit = 0;
	
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
	curs_set(0);
	
    WINDOW* win = newwin(height, width, 5, 50);
    refresh();
    box(win, 0, 0);
    wrefresh(win);

    keypad(win, TRUE);
    wrefresh(win);

    char *choices[] = {"Start", "Reset", "Lap", "Quit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int choice;
    int highlight = 0;

    while (1) {
        for (int i = 0; i < n_choices; i++) {
            if (i == highlight) {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, 20, 7+i*10, "%s", choices[i]);
            wattroff(win, A_REVERSE);
        }
        wrefresh(win);
        
        choice = wgetch(win);
        switch (choice) {
            case KEY_LEFT:
                highlight--;
                if (highlight == -1) {
                    highlight = n_choices - 1;
                }
                break;
            case KEY_RIGHT:
                highlight++;
                if (highlight == n_choices) {
                    highlight = 0;
                }
                break;
            default:
                break;
        }
        if (choice == 10) { // Enter key
            if(highlight==0){
				if(strcmp(choices[0],"Start")==0){
					choices[0]="Stop";
					start=1;
					stop=0;
				}
				else if(strcmp(choices[0],"Stop")==0){
					choices[0]="Start";
					start=0;
					stop=1;
				}
				reset=0;
				quit=0;
			}
			else if(highlight==1){
				start=0;
				stop=0;
				reset=1;
				quit=0;
			}
			else if(highlight==2){
				start=0;
				stop=0;
				reset=0;
				lap++;
				quit=0;
			}
			else if(highlight==3){
				start=0;
				stop=0;
				reset=0;
				quit=1;
			}
			
			int result=stopwatch(win, &min, &sec, start, stop, reset, lap, quit);
			while (1) {
				if (reset == 1) {
					min = 0;
					sec = 0;
					werase(win);
					box(win, 0, 0);
					wrefresh(win);
					break;
				}
				if (quit == 1) {
					result = 0;
					break;
				}
			}

			if (result == 0) {
				break;
			}
        }
    }
	
	

    delwin(win);
    endwin();
    return 0;
}
