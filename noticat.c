#include "cnoti.h"

#include <stdio.h>
#include <string.h>

#ifdef USE_CJSON
#include <cjson/cJSON.h>
#include <stdlib.h>

void print_json(char const *appname, uint32_t id, char const *icon, char const *summary, char const *body,
                int32_t timeout) {
  cJSON *object = cJSON_CreateObject();
  cJSON_AddStringToObject(object, "application", appname);
  cJSON_AddNumberToObject(object, "id", id);
  cJSON_AddStringToObject(object, "icon", icon);
  cJSON_AddStringToObject(object, "summary", summary);
  cJSON_AddStringToObject(object, "body", body);
  cJSON_AddNumberToObject(object, "timeout", timeout);
  char *json_str = cJSON_PrintUnformatted(object);
  printf("%s\n", json_str);
  free(json_str);
  cJSON_free(object);
}
#endif

#ifdef DWLB_STRLEN

void print_dwlb(char const *appname, uint32_t id, char const *icon, char const *summary,
                        char const *body, int32_t timeout) {  
  char *_summary = (char*)summary;
  char *_body = (char*)body;

  uint32_t len = DWLB_STRLEN;
  
  printf("[%s] ", appname);

  while (*_summary != '\0') {
    if (len != 0)
      len--;
    else 
      break;
    if (*_summary != '\n' && *_summary != '\r')
      putchar(*_summary);
    else 
      putchar(' ');      
    _summary++;
  }

  if (*_body != '\0' && len != 0) {
    printf(":");
    len--;
  }
   
  while (*_body != '\0') {
    if (len != 0)
      len--;
    else 
      break;
    if (*_body != '\n' && *_body != '\r')
      putchar(*_body);
    else 
      putchar(' ');
    _body++;
  }

  if (len == 0)
    printf("+");  
  putchar('\n');
  fflush(stdout);
}
#endif

void print_notification(char const *appname, uint32_t id, char const *icon, char const *summary,
                        char const *body, int32_t timeout) {
  printf("\"%s\" %u \"%s\" \"%s\" \"%s\" %d\n", appname, id, icon, summary, body, timeout);
  fflush(stdout);
}

int main(int argc, char *argv[]) {
  cnoti_callback_type *callback = print_notification;

  if (argc == 2 ) {
    if (strcmp(argv[1], "-json") == 0) {
#ifdef USE_CJSON
      callback = print_json;
#else
      printf("JSON option not available in this build, please build with USE_CJSON=YES to use it.\n");
      return 1;
#endif // CJSON
    } else if (strcmp(argv[1], "-dwlb") == 0) {
#ifdef DWLB_STRLEN
      callback = print_dwlb;
#else
      printf("DWLB option not available in this build, please build with DWLB_STRLEN=64 to use it.\n");
      return 1;
#endif // DWLB
    } else {
      printf("Usage: %s [-json] [-dwlb]\n", argv[0]);
      return 0;
    }
  }

  const char *errstr = cnoti(callback);
  if (errstr != NULL) {
    fprintf(stderr, "%s", errstr);
    return 1;
  }
  return 0;
}
