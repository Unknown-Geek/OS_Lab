#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void print_permissions(mode_t mode)
{
    printf("%c", (S_ISDIR(mode)) ? 'd' : '-');
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

int main(int argc, char *argv[])
{
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    struct passwd *pwd;
    char path[1024];
    char time_str[20];

    if (argc != 2) {
        printf("Usage: listdir <directory_path>\n");
        return 1;
    }

    if ((dir = opendir(argv[1])) == NULL) {
        perror("Error opening directory");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        snprintf(path, sizeof(path), "%s/%s", argv[1], entry->d_name);

        if (stat(path, &file_stat) == -1)
            continue;

        print_permissions(file_stat.st_mode);

        pwd = getpwuid(file_stat.st_uid);
        printf(" %-8s ", (pwd != NULL) ? pwd->pw_name : "unknown");

        strftime(time_str, 20, "%b %d %H:%M", localtime(&file_stat.st_atime));
        printf("%s ", time_str);

        strftime(time_str, 20, "%b %d %H:%M", localtime(&file_stat.st_mtime));
        printf("%s %s\n", time_str, entry->d_name);
    }

    closedir(dir);
    return 0;
}
