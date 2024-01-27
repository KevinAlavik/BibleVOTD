#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define URL "https://labs.bible.org/api/?passage=votd&type=json"
#define WIDTH 95

struct MemoryStruct {
    char *memory;
    size_t size;
};

size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    mem->memory = realloc(mem->memory, mem->size + realsize + 1);
    if (mem->memory == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 0;
    }

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void extractJsonValue(const char *json, const char *key, char *value, size_t maxLen) {
    const char *start = strstr(json, key);
    if (start) {
        start += strlen(key) + 3;
        const char *end = strchr(start, '"');
        if (end) {
            size_t len = end - start;
            if (len < maxLen) {
                strncpy(value, start, len);
                value[len] = '\0';
            }
        }
    }
}

void printStyledVerse(const char *book, const char *chapter, const char *verse, const char *text) {
    size_t textLen = strlen(text);
    size_t borderWidth = WIDTH;

    size_t infoSpaces = 2 + strlen(book) + strlen(chapter) + strlen(verse);

    printf("\033[1;37m╭");
    for (size_t i = 0; i < borderWidth - 2; ++i) {
        printf("─");
    }
    printf("╮\033[0m\n");

    size_t remaining = textLen;
    const char *current = text;
    while (remaining > 0) {
        size_t toPrint = (remaining > (WIDTH - 4)) ? (WIDTH - 4) : remaining;
        printf("\033[1;37m│\033[0m\033[1;32m ");
        fwrite(current, 1, toPrint, stdout);
        for (size_t i = 0; i < WIDTH - 4 - toPrint; ++i) {
            printf(" ");
        }
        printf("\033[0m\033[1;37m │\033[0m\n");

        current += toPrint;
        remaining -= toPrint;
    }

    printf("\033[1;37m│\033[0m\033[1;37m");
    for (size_t i = 0; i < WIDTH - 2; ++i) {
        printf(" ");
    }
    printf("\033[1;37m│\033[0m\n");

    printf("\033[1;37m│\033[0m\033[1;37m ✝ %s %s:%s\033[0m", book, chapter, verse);
    for (size_t i = 0; i < WIDTH - infoSpaces - 5; ++i) {
        printf(" ");
    }
    printf("\033[1;37m│\033[0m\n");

    printf("\033[1;37m╰");
    for (size_t i = 0; i < borderWidth - 2; ++i) {
        printf("─");
    }
    printf("╯\033[0m\n");
}

void getVerse() {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        curl_easy_cleanup(curl);
    }

    char book[256], chapter[256], verse[256], text[4096];
    extractJsonValue(chunk.memory, "bookname", book, sizeof(book));
    extractJsonValue(chunk.memory, "chapter", chapter, sizeof(chapter));
    extractJsonValue(chunk.memory, "verse", verse, sizeof(verse));
    extractJsonValue(chunk.memory, "text", text, sizeof(text));

    if (*book && *chapter && *verse && *text) {
        printStyledVerse(book, chapter, verse, text);
    } else {
        fprintf(stderr, "Error: Incomplete or missing JSON structure.\n");
    }

    free(chunk.memory);
}

int main() {
    getVerse();
    return 0;
}
