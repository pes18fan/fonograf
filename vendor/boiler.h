/* Convenience single-header library for some useful macros and functions.
 *
 * You MUST define `BOILER_IMPL` before including `boiler.h`. It must only be
 * defined ONCE in ONLY ONE FILE.
 */
#ifndef boiler_h
#define boiler_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <unistd.h>
#endif

/* Macros to handle terminal colors with ease */
#ifdef _WIN32
/**
 * Start coloring terminal colors in red at stdout.
 * It is recommended to call RESET() to stop coloring.
 */
#define RED()                                                                  \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED)

/**
 * Start coloring terminal colors in green at stdout.
 * It is recommended to call RESET() to stop coloring.
 */
#define GREEN()                                                                \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN)

/**
 * Reset the set terminal coloring at stdout.
 * It is recommended to call this after calling one of the coloring functions.
 */
#define RESET()                                                                \
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),                   \
                            FOREGROUND_RED | FOREGROUND_GREEN |                \
                                FOREGROUND_BLUE)
#else
/**
 * Start coloring terminal colors in red at stdout.
 * It is recommended to call RESET() to stop coloring.
 */
#define RED() printf("\033[1;31m")

/**
 * Start coloring terminal colors in green at stdout.
 * It is recommended to call RESET() to stop coloring.
 */
#define GREEN() printf("\033[1;32m")

/**
 * Reset the set terminal coloring at the provided `stream`.
 * It is recommended to call this after calling one of the coloring functions.
 */
#define RESET() printf("\033[0m")
#endif // _WIN32

/** @brief Read an entire file from the provided filename.
 *
 * This function allocates memory that must be freed after use.
 * Returns NULL if the read fails.
 *
 * @param filename The name of the file to read
 * @return The content of the file
 *
 * Example:
 *     const char* content = read_entire_file_from_filename("a.txt");
 *     if (content == NULL) return 1; // Don't forget this!
 *     printf("%s", content);
 *     free(content); // Don't forget this either!
 */
const char* read_entire_file_from_filename(const char* filename);

typedef void (*WithCallback)(FILE* f);

/** @brief Function that emulates Python's `with` statement.
 *
 * @param char* file_name           The name of the file to open
 * @param char* mode                The mode to open the file in
 * @param void (*callback)(FILE* f) A function called by `with` and where
 * the opened file is passed in.
 * @return 0 on success, 1 on failure
 *
 * Example:
 *
 * void callback(FILE* f) {
 *     fprintf(f, "Sup");
 * }
 *
 * int main() {
 *     with("a.txt", "w", callback);
 * }
 */
int with(const char* filename, const char* mode, WithCallback callback);

/* Some terminal goodies. */

/** @brief Print out the text passed into it, centered in the terminal.
 *
 * @param char* text    The text to be centered
 */
void center_text(const char* text);

/** @brief Clear the terminal. */
void clrscr();

/** @brief Wait for a keypress and return instantly once the key is pressed.
 *
 * @return The ASCII code of the pressed key.
 */
int __getch();

/** @brief Return a value based on whether a key was pressed.
 *
 * @return 1 if a key was pressed, 0 otherwise.
 */
int __kbhit();

#ifdef BOILER_IMPL
const char* read_entire_file_from_filename(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        /* Most likely, file doesn't exist */
        return NULL;
    }

    fseek(file, 0L, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(file_size + 1);
    if (buffer == NULL) {
        /* Not enough memory */
        return NULL;
    }

    size_t bytes_read = fread(buffer, sizeof(char), file_size, file);
    if (bytes_read < file_size) {
        /* Failed to read */
        return NULL;
    }

    buffer[bytes_read] = '\0';

    fclose(file);
    return buffer;
}

int with(const char* filename, const char* mode, WithCallback callback) {
    FILE* f = fopen(filename, mode);
    if (f == NULL)
        return 1;
    callback(f);
    fclose(f);

    return 0;
}

void clrscr() {
#ifdef _WIN32
    HANDLE h_console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord_screen = {0, 0};
    DWORD c_chars_written;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dw_con_size;

    // get number of character cells in the current buffer
    if (!GetConsoleScreenBufferInfo(h_console, &csbi))
        return;
    dw_con_size = csbi.dwSize.X * csbi.dwSize.Y;

    // fill the entire buffer with spaces
    if (!FillConsoleOutputCharacter(h_console, (TCHAR)' ', dw_con_size,
                                    coord_screen, &c_chars_written)) {
        return;
    }

    // reset the attributes of every character cell
    if (!FillConsoleOutputAttribute(h_console, csbi.wAttributes, dw_con_size,
                                    coord_screen, &c_chars_written)) {
        return;
    }

    // move the cursor back to the top left corner
    SetConsoleCursorPosition(h_console, coord_screen);
#else
    printf("\033[2J\033[H");
#endif /* _WIN32 */
}

int __kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    int cnt = 0;
    int error;
    static struct termios Otty, Ntty;

    tcgetattr(0, &Otty);
    Ntty = Otty;
    Ntty.c_iflag = 0;
    Ntty.c_oflag = 0;
    Ntty.c_lflag &= ~ICANON;
    Ntty.c_cc[VMIN] = 1;
    Ntty.c_cc[VTIME] = 1;

    if (0 == (error = tcsetattr(0, TCSANOW, &Ntty))) {
        struct timeval tv;
        error += ioctl(0, FIONREAD, &cnt);
        error += tcsetattr(0, TCSANOW, &Otty);
        tv.tv_sec = 0;
        tv.tv_usec = 100;
        select(1, NULL, NULL, NULL, &tv);
    }

    return (error == 0 ? cnt : -1);
#endif
}

int __getch() {
#ifdef _WIN32
    return _getch();
#else
    char ch;
    int error;
    static struct termios Otty, Ntty;

    fflush(stdout);
    tcgetattr(0, &Otty);
    Ntty = Otty;
    Ntty.c_iflag = 0;
    Ntty.c_oflag = 0;
    Ntty.c_lflag &= ~ICANON;
    Ntty.c_lflag &= ~ECHO;
    Ntty.c_cc[VMIN] = 1;
    Ntty.c_cc[VTIME] = 1;

    if (0 == (error = tcsetattr(0, TCSANOW, &Ntty))) {
        error = read(0, &ch, 1);
        error += tcsetattr(0, TCSANOW, &Otty);
    }

    return (error == 1 ? (int)ch : -1);
#endif
}

void center_text(const char* text) {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
#endif

#ifdef _WIN32
    int console_width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    int console_width = w.ws_col;
#endif

    int text_length = strlen(text);
    int padding = (console_width - text_length) / 2;

    // print spaces to center the text
    for (int i = 0; i < padding; i++) {
        putchar(' ');
    }

    // print the text
    printf("%s", text);
}
#endif

#ifdef __cplusplus
}
#endif

#endif // boiler_h
