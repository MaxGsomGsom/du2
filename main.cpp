#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>

using namespace std;

long sumdirsize(char* name)
{
    long sum = 0;
    struct stat file;

    if (stat(name, &file) != -1)
    {
        sum += DEV_BSIZE * file.st_blocks;
        DIR* dir = opendir(name);

        if (dir)
        {
            struct dirent* entity;
            char* fullpath = new char[256];

            while ((entity = readdir(dir)) != NULL)
            {
                strcpy(fullpath, name);
                strcat(fullpath, "/");
                strcat(fullpath, entity->d_name);

                if (entity->d_type == DT_DIR)
                {
                    if (!(entity->d_name[0] == '.' && (entity->d_name[1] == '\0' || entity->d_name[1] == '.')))
                        sum += sumdirsize(fullpath);
                }
                else if (lstat(fullpath, &file) != -1)
                    sum += DEV_BSIZE * file.st_blocks;
            }
            free(fullpath);
            closedir(dir);
        }
        printf("%-10ld %s\n", (sum / 1024), name);
    }

    return sum;
}

int main(int argc, char* argv[])
{
    if (argc >= 2)
    {
        char* fullpath = NULL;
        fullpath = realpath(argv[1], fullpath);
        sumdirsize(fullpath);
    }

    return 0;
}
