#include "config.h"

using namespace std;

char *conf_path(const char *path, const char *file)
{
    char *buf;
    buf = (char *)malloc(64);
    sprintf(buf, "%s/%s", path, file);
    return buf;
}

void conf_init(const char *path, const char *file)
{
    struct stat sb;
    int fd;
    struct flock lock;
    char *conf_file = conf_path(path, file);
    string line;
    if (file == NULL)
    {
        perror("file is NULL!");
        exit(EXIT_FAILURE);
    }
    if (stat(conf_file, &sb) == -1)
    {
        perror("file find/open failed!");
        exit(EXIT_FAILURE);
    }
    fstream infile;
    infile.open(conf_file, ios::in);
    while(getline(infile, line))
    {
        cout << line << endl;
    }
    infile.close();
}
