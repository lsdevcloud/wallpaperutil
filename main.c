#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
  return fwrite(ptr, size, nmemb, stream);
}

int main(int argc, char *argv[]) {
  if (argc < 2 || argc > 6) {
    fprintf(stderr,
            "Usage: %s [-o output_path] [-s symlink_path] <image_url>\n",
            argv[0]);
    return 1;
  }

  const char *url = NULL;
  char filepath[512] = "";
  char symlinkpath[512] = "";

  const char *home = getenv("HOME");
  if (home == NULL) {
    fprintf(stderr, "Could not determine home directory.\n");
    return 1;
  }

  snprintf(symlinkpath, sizeof(symlinkpath),
           "%s/.local/share/wallpapers/latest.png", home);

  char wallpaper_folder[600];
  snprintf(wallpaper_folder, sizeof(wallpaper_folder),
           "%s/.local/share/wallpapers", home);

  mkdir(wallpaper_folder, 0777);

  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
      strncpy(filepath, argv[++i], sizeof(filepath) - 1);
      filepath[sizeof(filepath) - 1] = '\0';
    } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
      strncpy(symlinkpath, argv[++i], sizeof(symlinkpath) - 1);
      symlinkpath[sizeof(symlinkpath) - 1] = '\0';
    } else {
      url = argv[i];
    }
  }

  if (strlen(filepath) == 0) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(
        filepath, sizeof(filepath),
        "%s/.local/share/wallpapers/wallpaper_%04d%02d%02d_%02d%02d%02d.png",
        home, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour,
        t->tm_min, t->tm_sec);
  }

  CURL *curl = curl_easy_init();
  if (!curl) {
    fprintf(stderr, "Failed to initialize curl.\n");
    return 1;
  }

  FILE *fp = fopen(filepath, "wb");
  if (!fp) {
    fprintf(stderr, "Failed to open file for writing: %s\n", filepath);
    curl_easy_cleanup(curl);
    return 1;
  }

  curl_easy_setopt(curl, CURLOPT_URL, url);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

  CURLcode res = curl_easy_perform(curl);
  fclose(fp);
  curl_easy_cleanup(curl);

  if (res != CURLE_OK) {
    fprintf(stderr, "Failed to download image: %s\n", curl_easy_strerror(res));
    return 1;
  }

  printf("Image downloaded to: %s\n", filepath);

  remove(symlinkpath);
  if (symlink(filepath, symlinkpath) != 0) {
    fprintf(stderr, "Failed to create symlink: %s\n", symlinkpath);
  }

  char command[600];
  snprintf(command, sizeof(command), "feh --bg-scale %s", filepath);
  int ret = system(command);
  if (ret != 0) {
    fprintf(stderr, "Failed to set wallpaper with feh.\n");
    return 1;
  }

  printf("Wallpaper set successfully!\n");
  return 0;
}