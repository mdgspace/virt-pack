#ifndef PARSER_H
#define PARSER_H

#include <jansson.h>
#include <stdbool.h>

#define INPUT_FILE "data/events.jsonl"
#define OUTPUT_FILE "data/libs.json"

json_t *collect_pkg_config_libs(const char *json_path);
bool write_json_to_file(json_t *array, const char *filename);
int parser_main();

#endif