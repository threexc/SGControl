#ifndef SG_BINENC_H
#define SG_BINENC_H

int sg_binenc(double *i_data, double *q_data, int num_points, short int *waveform_array);

int write_encoded(char *filename, short int *waveform_array, int num_points);

int sg_bindec(void *binary_file);

int read_encoded(char *filename, short int *waveform_array, int num_points);

#endif
