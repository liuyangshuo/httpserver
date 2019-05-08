#include "config.h"

using namespace std;

char *conf_path(const char *path, const char *file)
{
    char *buf;
    buf = (char *)malloc(64);
    sprintf(buf, "%s/%s", path, file);
    return buf;
}

string get_val(string str)
{
    string::iterator str_beg = str.begin();
    int pos = str.find(' ');
    advance(str_beg, pos + 1);
    string value(str_beg, str.end());
    return value;
}

void parse_conf(string str)
{
    if (str.find("work") != string::npos)
    {
        string val = get_val(str);
        work_num = stoi(val);
    }
    if (str.find("threads") != string::npos)
    {
        string val = get_val(str);
        thread_num = stoi(val);
    }
}

void conf_init(const char *path, const char *file)
{
    struct stat sb;
    int fd;
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
        parse_conf(line);
    }
    infile.close();
}
