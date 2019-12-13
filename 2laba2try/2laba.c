#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
//#include <sys/sysmacros.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

void printFileType(pid_t);
void printPerms(pid_t);
void printUserOwner(pid_t);
void printGroupOwner(pid_t);
void printSize(pid_t);

int main(void)
{
    struct dirent **namelist;
    int n;

    n = scandir(".", &namelist, NULL, NULL);
    if (n == -1) {
        perror("scandir");
        exit(EXIT_FAILURE);
    }

    
    // Prints dirent fields: $type\t$inode\t$size\t$name
    printf("total %i\n", n);
    while (n--) {
        static struct stat tmp_stat;
        // TODO: Add checking stat structure for all files and printing like ls -al

        //printf("%hhu\t%lu\t%hu\t%s\n", namelist[n]->d_type, namelist[n]->d_ino,namelist[n]->d_reclen, namelist[n]->d_name);
        if(stat(namelist[n]->d_name, &tmp_stat) == 0)
        {
            printFileType(tmp_stat.st_mode);
            printPerms(tmp_stat.st_mode);
            printf(" %lu", tmp_stat.st_nlink);
            printf(" ");
            printUserOwner(tmp_stat.st_uid);
            printf(" ");
            printGroupOwner(tmp_stat.st_gid);
            printf(" ");
            printSize(tmp_stat.st_size);
            printf(" ");
            printf("%s", ctime(&tmp_stat.st_mtime));
            printf("\t%s", namelist[n]->d_name);
            printf("\n");
        }
        free(namelist[n]);
    }
    free(namelist);

    exit(EXIT_SUCCESS);
}

void printFileType(pid_t _mode)
{
            switch (_mode & S_IFMT)
            {
            case __S_IFBLK:
                printf("b");
                break;
            case __S_IFCHR:
                printf("c");
                break;
            case __S_IFDIR:
                printf("d");
                break;
            case __S_IFIFO:
                printf("p");
                break;
            case __S_IFLNK:
                printf("l");
                break;
            case __S_IFSOCK:
                printf("s");
                break;
            // case __S_IFREG:
            //     printf("f");
            //     break;
            default:
                printf("-");
                break;
            }
}

void printPerms(pid_t _mode)
{
    if(_mode & 0400)
        printf("r");
    else
        printf("-");
    if(_mode & 0200)
        printf("w");
    else
        printf("-");
    if(_mode & 0100)
        printf("x");
    else
        printf("-");
    if(_mode & 0040)
        printf("r");
    else
        printf("-");
    if(_mode & 0020)
        printf("w");
    else
        printf("-");
    if(_mode & 0010)
        printf("x");
    else
        printf("-");
    if(_mode & 0004)
        printf("r");
    else
        printf("-");
    if(_mode & 0002)
        printf("w");
    else
        printf("-");
    if(_mode & 0001)
        printf("x");
    else
        printf("-");
}

void printUserOwner(pid_t _uid)
{
    struct passwd *pws;
    pws = getpwuid(_uid);
    printf("%s", pws->pw_name);
}

void printGroupOwner(pid_t _gid)
{
    struct group *gp;
    gp = getgrgid(_gid);
    printf("%s", gp->gr_name);
}

void printSize(pid_t _size)
{
    printf("%5d", _size);
}
