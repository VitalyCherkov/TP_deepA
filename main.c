#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>
#include <stddef.h>

void print(char** strings);
char* read_line();
char* grow_line_buffer(char* line, size_t* cur_buffer_size);
char** input(size_t* out_number);
char** grow_input_buffer(char** string_list, size_t* cur_buffer_size);
char** filter(char** from, size_t number);
int check_cur(char* cur);
void free_strings(char** strings, size_t number);

int main() {
    size_t number = 0;
    char** strings = input(&number);
    char** filtered_strings = filter(strings, number);
    print(filtered_strings);
    free_strings(strings, number);
    free(filtered_strings);
    return 0;
}

void print(char** strings) {
    if(!strings) {
        printf("[error]\n");
        return;
    }

    for(size_t i = 0; strings[i]; i++) {
        printf("%s\n", strings[i]);
    }
}

char* read_line() {
    ptrdiff_t cur_len = 0;
    size_t cur_buffer = 11;

    char* line = (char*) malloc(cur_buffer);
    if(!line)
        return NULL;

    line[0] = '\0';
    for( char* cur_line_ptr = line; fgets(cur_line_ptr, (cur_buffer - cur_len), stdin); cur_line_ptr = line + cur_len) {
        cur_len += strlen(cur_line_ptr);
        if(line[cur_len - 1] == '\n') {
            line[cur_len - 1] = '\0';
            return line;
        }

        line = grow_line_buffer(line, &cur_buffer);
        if(!line)
            return NULL;
    }

    if(strlen(line) == 0) {
        free(line);
        line = NULL;
    }

    return line;
}

char* grow_line_buffer(char* line, size_t* cur_buffer_size) {
    if(!line || !cur_buffer_size)
        return NULL;

    char* temp_line = (char*) realloc(line, *cur_buffer_size * 2);
    if(!temp_line) {
        free(line);
        return NULL;
    }
    *cur_buffer_size *= 2;
    return temp_line;
}

char** input(size_t* out_number) {
    if(!out_number)
        return NULL;

    size_t number = 0;
    size_t max_number = 10;
    char** string_list = (char**) malloc(max_number * sizeof(char*));
    if(!string_list)
        return NULL;

    while((string_list[number++] = read_line())) {
        if(number == max_number) {
            string_list = grow_input_buffer(string_list, &max_number);
            if(!string_list)
                return NULL;
        }
    }

    *out_number = number - 1;
    return string_list;
}

char** grow_input_buffer(char** string_list, size_t* cur_buffer_size) {
    if(!string_list || !cur_buffer_size)
        return NULL;

    char** temp_string_list = (char**) realloc(string_list, sizeof(char *) * (*cur_buffer_size * 2));
    if(!temp_string_list) {
        for(size_t i = 0; i < *cur_buffer_size; i++) {
            free(string_list[i]);
        }
        free(string_list);
        return NULL;
    }
    *cur_buffer_size *= 2;
    return temp_string_list;
}

char** filter(char** from, size_t number) {
    if(!from)
        return NULL;

    char** result = (char**) malloc((number + 1) * sizeof(char*));
    if(result == NULL)
        return NULL;

    size_t ok_number = 0;
    for(size_t i = 0; i < number; i++) {
        if(!from[i]) {
            free(result);
            return NULL;
        }

        if(check_cur(from[i]) == EXIT_SUCCESS)
            result[ok_number++] = from[i];
    }

    result[ok_number++] = NULL;
    char** cut_result = realloc(result, ok_number * sizeof(char*));
    if(cut_result == NULL)
        free(result);

    return cut_result;
}

int check_cur(char* cur) {
    const size_t OK_NUMBER = 4;
    const char* OK_STARTS[] = {"To:", "From:", "Date:", "Subject:"};
    const size_t OK_LENS[] = {3, 5, 5, 8};

    if(!cur)
        return EXIT_FAILURE;

    for(size_t i = 0; i < OK_NUMBER; i++) {
        if(strncmp(cur, OK_STARTS[i], OK_LENS[i]) == 0)
            return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}

void free_strings(char** strings, size_t number) {
    if(!strings)
        return;

    for(size_t i = 0; i < number; i++) {
        free(strings[i]);
        strings[i] = NULL;
    }
    free(strings);
}