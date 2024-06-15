#include <ncurses/ncurses.h> //If this doesn't work, then try changing to #include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

void update(WINDOW* win, int min, int sec) { //Function to update the time shown
    mvwprintw(win, 12, 22, "%02d:%02d", min, sec);
    wrefresh(win);
}

void handle_input(WINDOW* win, char **choices, int n_choices, int *highlight, int *start, int *stop, int *reset, int *lap, int *quit) { //Function to handle input
    int choice = wgetch(win);
    switch (choice) {
        case KEY_LEFT:
            (*highlight)--;
            if (*highlight == -1) {
                *highlight = n_choices - 1;
            }
            break;
        case KEY_RIGHT:
            (*highlight)++;
            if (*highlight == n_choices) {
                *highlight = 0;
            }
            break;
        case 10: 
            if (*highlight == 0) {
                if (strcmp(choices[0], "Start") == 0) {
                    choices[0] = "Stop ";
                    *start = 1;
                    *stop = 0;
                } else if (strcmp(choices[0], "Stop ") == 0) {
                    choices[0] = "Start";
                    *start = 0;
                    *stop = 1;
                }
            } else if (*highlight == 1) {
                *reset = 1;
            } else if (*highlight == 2) {
                *lap = 1;
            } else if (*highlight == 3) {
                *quit = 1;
            }
            break;
        default:
            break;
    }
}

void stopwatch(WINDOW* win, WINDOW* lapwin, int* min, int* sec, int *start, int *stop, int *reset, int *lap) { //Function that operates Stopwatch
    static int elapsed = 0;
	static int lap_no = 1;
    if (*start == 1) {
        Sleep(100); 
        elapsed += 100;
        if (elapsed >= 1000) {
            (*sec)++;
            elapsed = 0;
            if (*sec == 60) {
                (*min)++;
                *sec = 0;
            }
            update(win, *min, *sec);
        }
    }

    if (*stop == 1) {
        *start = 0;
        *stop = 0;
    }

    if (*reset == 1) {
        *min = 0;
        *sec = 0;
        werase(win);
        box(win, 0, 0);
        wrefresh(win);
        update(win, *min, *sec);
        *reset = 0;
		lap_no = 1;
    }

    if (*lap == 1) {
        mvwprintw(lapwin, lap_no, 1, "Lap %d: %02d:%02d", lap_no, *min, *sec);
        wrefresh(lapwin);
        lap_no++;
        *lap = 0;
    }
}

int main() {
    int height = 25;
    int width = 50;
	int startx = 5;
	int starty = 50;
	
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

    WINDOW* win = newwin(height, width, startx, starty); //Creates window that holds stopwatch and menu
    refresh();
    box(win, 0, 0);
    wrefresh(win);
	mvwprintw(win, 1, 20, "Stopwatch");
	wrefresh(win);

    WINDOW* lapwin = newwin(height, 15, startx, 101); //Creates window that holds lap times
    box(lapwin, 0, 0);
    mvwprintw(lapwin, 1, 5, "Laps");
	wrefresh(lapwin);

    keypad(win, TRUE); 
    nodelay(win, TRUE); 
    wrefresh(win);
	

    char *choices[] = {"Start", "Reset", "Lap", "Quit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 0;

    while (1) {										//Game loop
        for (int i = 0; i < n_choices; i++) {
            if (i == highlight) {
                wattron(win, A_REVERSE);
            }
            mvwprintw(win, 20, 7 + i * 10, "%s", choices[i]);
            wattroff(win, A_REVERSE);
        }
        wrefresh(win);

        handle_input(win, choices, n_choices, &highlight, &start, &stop, &reset, &lap, &quit);
        stopwatch(win, lapwin, &min, &sec, &start, &stop, &reset, &lap);

        if (quit == 1) {
            break;
        }
    }

    delwin(win);
    delwin(lapwin);
    endwin();
    return 0;
}
