#include <stdio.h>
#include <windows.h> // for Sleep function
#include <ncurses/ncurses.h>

int stopwatch(WINDOW* game, int* min, int* sec) {
    while (1) {
        box(game, 0, 0); // Redraw the box around the window
        mvwprintw(game, 1, 1, "%02d:%02d", *min, *sec);
        wrefresh(game);
        Sleep(1000); // Sleep for 1000 milliseconds (1 second)

        (*sec)++;
        if (*sec == 60) {
            (*min)++;
            *sec = 0;
        }

        if (*min == 5) {
            break;
        }

        // Check for user input to quit or pause
        int ch = getch();
        if (ch == 'q') { // Press 'q' to quit
            return 0; // Return 0 to indicate quitting
        }
        if (ch == 'p') { // Press 'p' to pause
            while (1) {
                int x = getch();
                if (x == 'p') { // Press 'p' again to resume
                    break;
                }
                if (x == 'q') { // Press 'q' to quit
                    return 0; // Return 0 to indicate quitting
                }
                if (x == 'r') {
					*min = 0;
					*sec = 0;
					werase(game); // Clear window content
					box(game, 0, 0); // Redraw the box around the window
					wrefresh(game);
					break; // Restart stopwatch
	            }
                Sleep(100); // Sleep for a short period to reduce CPU usage
            }
        }
    }
    return 1; // Return 1 to indicate continuation
}

int main() {
    int min = 0;
    int sec = 0;
    initscr(); // Initialize ncurses
    noecho(); // Don't echo keys to screen
    curs_set(0); // Hide cursor
    nodelay(stdscr, TRUE); // Set getch to non-blocking

    WINDOW* game = newwin(10, 30, 1, 1); // Create a new window
    box(game, 0, 0); // Draw a box around the window
    wrefresh(game);

    while (1) {
        int result = stopwatch(game, &min, &sec);
        if (result == 0) {
            break; // Quit if stopwatch returns 0
        }

        while (1) {
            int c = getch();
            if (c == 'r') {
                min = 0;
                sec = 0;
                werase(game); // Clear window content
                box(game, 0, 0); // Redraw the box around the window
                wrefresh(game);
                break; // Restart stopwatch
            }
            if (c == 'q') {
                result = 0; // Set result to 0 to indicate quitting
                break;
            }
        }

        if (result == 0) {
            break; // Quit if result is 0
        }
    }

    delwin(game); // Delete the window
    endwin(); // Properly end the ncurses session
    return 0;
}