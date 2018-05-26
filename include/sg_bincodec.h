
#ifndef SG_BINENC_H
#define SG_BINENC_H

int sg_binenc(void *i_data, void *q_data, char *filename);

int write_encoded(char *filename, short int *waveform_array, int num_points);

int read_encoded(char *filename, short int *waveform_array, int num_points);

int sg_bindec(void *binary_file);

#endif
