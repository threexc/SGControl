import numpy as np

def write_iq_cosine():
    cos_range = np.arange(0, 2*np.pi, 2*np.pi/60)

    with open("iq_cos", 'w') as f:
        for value in range(0, len(cos_range)):
            f.write("{0}, {1}\n".format(np.cos(cos_range[value]), 0))
    f.close()

def write_bpsk():
    pass


if __name__ == '__main__':
    write_iq_cosine()
