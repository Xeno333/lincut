#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define Version "v0.1 (alpha)"
#define ROOT "sudo "
#define copy "cp -r "


typedef enum {
    false = 0,
    true = 1
} bool;





int init_lincut() {
    char username[32]; // Declare a character buffer to store the username
    getlogin_r(username, sizeof(username)); // Get the username using getlogin_r()
    char path[1024];
    strcpy(path, "/home/");
    strcat(path, username);
    strcat(path, "/.lincut");

    if (mkdir(path, 0777) == -1)
        printf("Already setup: %s\n", path);

    //plugins
    char conf[1024];
    strcpy(conf, path);
    strcat(conf, "/plugins");
    if (mkdir(conf, 0777) == -1)
        printf("Already setup: %s\n", conf);
        

    //conf
    strcpy(conf, path);
    strcat(conf, "/lincut.conf");
    FILE* f = fopen(conf, "r");
    if (f == NULL) {
        FILE* f = fopen(conf, "w");
        fprintf(f, "Version: %s", Version);
        fclose(f);
    }
    else {
        fclose(f);
        printf("Already setup: %s\n", conf);
    }

}


int addplugin(char* pn) {
    char username[32]; // Declare a character buffer to store the username
    getlogin_r(username, sizeof(username)); // Get the username using getlogin_r()
    char path[1024];
    strcpy(path, "/home/");
    strcat(path, username);
    strcat(path, "/.lincut");
    strcat(path, "/plugins/");

    char cmd[2048];
    strcpy(cmd, copy);
    strcat(cmd, pn);
    strcat(cmd, " ");
    strcat(cmd, path);

    system(cmd);
}


int main(int ac, char** args) {
    if (ac < 2) {
        printf("Usage: lincut <cmd> <args>\n\tDo \"lincut help\" for help.\n");
        return 1;
    }

    else if (strcmp(args[1], "help") == 0) {
        printf("limcut %s\n\
            Commands:\n\
                \thelp\tDisplays this help menu.\n\
                \tinit\tInitilizes limcut and makes config files.\n\
                \tip\tInstall plugin, usage\"lincut ip <plugin to install>\"\n\
                \t<none>\tRun plugin.\n\
            \
            \n\n\
            \
            ", Version
        );
        puts("");
    }


    else if (strcmp(args[1], "ip") == 0) {
        if (ac < 3) {
            printf("No input specified.\n");
            return 1;
        }
        addplugin(args[2]);
    }

    else if (strcmp(args[1], "init") == 0) {
        return init_lincut();
    }

    else {
        char cmd[2048];
        char path[1024];
        char conf[1024];
        char username[32]; // Declare a character buffer to store the username
        char runwith[1024];
        char run[1024];
        char line[1024];
        char parsed_line[1024];

        //get path
        strcpy(path, "/home/");
        getlogin_r(username, sizeof(username)); // Get the username using getlogin_r()
        strcat(path, username);
        strcat(path, "/.lincut/plugins/");
        strcat(path, args[1]);

        //get conf
        strcpy(conf, path);
        strcpy(conf, path);
        strcat(conf, "/plugin.conf");
        
        strcpy(run, "");
        strcpy(runwith, "");
        FILE* pconf = fopen(conf, "r");
        if (pconf == NULL) {
            printf("Bad plugin.conf @ %s\n", conf);
            return 1;
        }

        while (true) {
            char* r = fgets(line, sizeof(line), pconf);
            if (r == NULL) {
                break;
            }
            
            int i = 0;
            while ((line[i] != ' ') && (line[i] != '\t') && (line[i] != '=')) {
                parsed_line[i] = line[i];
                i++;
            }
            parsed_line[i] = 0;

            int i2 = 0;
            if (strcmp(parsed_line, "run") == 0) {
                while ((line[i] == ' ') || (line[i] == '=') || (line[i] == '\t')) i++;
                strcpy(run, "");
                while ((line[i] != '\n') && (line[i] != 0)) 
                    run[i2++] = line[i++];
                run[i2++] = 0;
            }
            else if (strcmp(parsed_line, "runwith") == 0) {
                while ((line[i] == ' ') || (line[i] == '=') || (line[i] == '\t')) i++;
                strcpy(runwith, "");
                while ((line[i] != '\n') && (line[i] != 0)) 
                    runwith[i2++] = line[i++];
                runwith[i2++] = 0;
            }
        }

        strcpy(cmd, runwith);
        strcat(cmd, " ");
        strcat(cmd, path);
        strcat(cmd, run);

        system(cmd);
    }



    return 0;
}