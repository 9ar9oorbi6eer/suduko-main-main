#ifndef MSSV_UTILS_H
#define MSSV_UTILS_H

int check_arguments(int argc, char *argv[], int *delay);
int initialize_stat_messages(char **stat_messages);
void process_validation_results(char **stat_messages);
void cleanup_resources(char **stat_messages);
void final_message(int total_valid);
void finalize_messages(char *stat_messages[], pthread_t thread_ids[]);



#endif