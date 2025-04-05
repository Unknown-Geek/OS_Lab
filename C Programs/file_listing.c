#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void print_permissions(mode_t mode);
void print_file_info(const char *path, const char *filename);

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    
    // Determine which directory to list
    const char *path = ".";  // Default to current directory
    if (argc > 1) {
        path = argv[1];
    }
    
    // Open the directory
    dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return 1;
    }
    
    printf("Listing for directory: %s\n", path);
    printf("\nPermissions  Links  Owner   Group   Size       Last Modified       Name\n");
    printf("--------------------------------------------------------------------------------\n");
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." if desired
        // if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        //    continue;
        
        print_file_info(path, entry->d_name);
    }
    
    closedir(dir);
    return 0;
}

// Print file permissions in the format "-rwxrwxrwx"
void print_permissions(mode_t mode) {
    // File type
    if (S_ISREG(mode)) printf("-");
    else if (S_ISDIR(mode)) printf("d");
    else if (S_ISLNK(mode)) printf("l");
    else if (S_ISFIFO(mode)) printf("p");
    else if (S_ISSOCK(mode)) printf("s");
    else if (S_ISCHR(mode)) printf("c");
    else if (S_ISBLK(mode)) printf("b");
    else printf("?");
    
    // User permissions
    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    
    // Group permissions
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    
    // Other permissions
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');
}

// Print file information in a format similar to ls -l
void print_file_info(const char *path, const char *filename) {
    struct stat file_stat;
    char filepath[1024];
    
    // Create full file path
    snprintf(filepath, sizeof(filepath), "%s/%s", path, filename);
    
    // Get file information
    if (lstat(filepath, &file_stat) < 0) {
        perror("Error getting file info");
        return;
    }
    
    // Print file permissions
    print_permissions(file_stat.st_mode);
    
    // Number of links
    printf("   %-5lu", file_stat.st_nlink);
    
    // Owner name
    struct passwd *pw = getpwuid(file_stat.st_uid);
    if (pw) {
        printf(" %-7s", pw->pw_name);
    } else {
        printf(" %-7d", file_stat.st_uid);
    }
    
    // Group name
    struct group *gr = getgrgid(file_stat.st_gid);
    if (gr) {
        printf(" %-7s", gr->gr_name);
    } else {
        printf(" %-7d", file_stat.st_gid);
    }
    
    // File size
    printf(" %-10lld", (long long)file_stat.st_size);
    
    // Last modified time
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&file_stat.st_mtime));
    printf(" %-20s", time_str);
    
    // File name
    printf(" %s\n", filename);
}