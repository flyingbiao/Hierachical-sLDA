// (C) Copyright 2009, Chong Wang, David Blei and Li Fei-Fei

// written by Chong Wang, chongw@cs.princeton.edu

// This file is part of slda.

// slda is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or (at your
// option) any later version.

// slda is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA

#include <stdio.h>
#include <string.h>
#include "corpus.h"
#include "utils.h"
#include "slda.h"

#include <sys/times.h>
#include <unistd.h>

void help( void ) {
    printf("usage: slda [est] [data] [data_feature] [label] [settings] [alpha] [k] [random/seeded/model_path] [directory]\n");
    printf("       slda [inf] [data] [data_feature] [label] [settings] [model] [directory]\n");
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        help();
        return 0;
    }
    char filename[100];
    FILE * Time_file = NULL;
    int sc_clk_tck;
    sc_clk_tck = sysconf(_SC_CLK_TCK);

    struct tms begin_tms, end_tms;
    clock_t begin, end;
    begin = times(&begin_tms);



    if (strcmp(argv[1], "est") == 0)
    {
        corpus c;
        char * data_filename = argv[2];
        char * data_feature_filename = argv[3];
        char * label_filename = argv[4];

        c.read_data(data_filename, label_filename,1);  //读取特征值
        c.read_data(data_feature_filename, label_filename,2);   //读取特征


        settings setting;
        char * setting_filename = argv[5];
        setting.read_settings(setting_filename);

        double alpha = atof(argv[6]);
        int num_topics = atoi(argv[7]);
        printf("number of topics is %d\n", num_topics);
        char * init_method = argv[8];
        char * directory = argv[9];
        printf("models will be saved in %s\n", directory);
        make_directory(directory);

        slda model;
        model.init(alpha, num_topics, &c);
        model.v_em(&c, &setting, init_method, directory);
   

   end = times(&end_tms);
   //printf("Time Consuming = %lf\n", (end - begin) / (double)sc_clk_tck);

   sprintf(filename, "%s/Time_Consuming.dat", ".");
   Time_file = fopen(filename, "a");
   fprintf(Time_file, "Time_train Consuming = %5.5f\n", (end - begin) / (double)sc_clk_tck);


    }

    if (strcmp(argv[1], "inf") == 0)
    {
        corpus c;
        char * data_filename = argv[2];
        char * data_feature_filename = argv[3];
        char * label_filename = argv[4];

        c.read_data(data_filename, label_filename,1);  //读取特征值
        c.read_data(data_feature_filename, label_filename,2);   //读取特征



        settings setting;
        char * setting_filename = argv[5];
        setting.read_settings(setting_filename);

        char * model_filename = argv[6];
        char * directory = argv[7];
        printf("\nresults will be saved in %s\n", directory);
        make_directory(directory);

        slda model;
        model.load_model(model_filename);
        model.infer_only(&c, &setting, directory);

	end = times(&end_tms);
	sprintf(filename, "%s/Time_Consuming.dat", ".");
	Time_file = fopen(filename, "a");
	fprintf(Time_file, "Time_test Consuming = %5.5f\n", (end - begin) / (double)sc_clk_tck);

    }

    return 0;
}
